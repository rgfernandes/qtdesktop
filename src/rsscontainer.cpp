#include <QHttp>
#include <QDebug>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "rsscontainer.h"
#include "rsschannel.h"
#include "rssitem.h"
#include "xmlparser.h"
#include "qrssglobals.h"


RssContainer::RssContainer( QObject* parent ) 
	: QObject( parent )
	, m_state( EIDLE )
{
	m_httpHandler = new HttpHandler( *this, this );
	m_curChannel = NULL;
	
	m_xmlParser = new XmlParser();

	//FIXME
	QTextCodec* codec;
	codec = QTextCodec::codecForName( "ISO 8859-2" );
	QTextCodec::setCodecForCStrings(codec);

	m_xmlReader.setContentHandler( m_xmlParser );
	m_xmlReader.setErrorHandler( this );

	//xml
	connect( m_xmlParser, SIGNAL( newItemParsed( QString&, QString&,QString&,QString& )),
			this, SLOT( newItemParsed( QString&, QString&, QString&, QString& )));
	connect( m_xmlParser, SIGNAL( channelTitleParsed( QString& )),
			this, SLOT( channelTitleParsed( QString& )));
	connect( m_xmlParser, SIGNAL( channelDateParsed( QString& )),
			this, SLOT( channelDateParsed( QString& )));
	connect( m_xmlParser, SIGNAL( documentEnd( )),
			this, SLOT( documentEnd( )));
	
	m_firstData = true;
	m_isUpdating = false;
	m_monthsName << "Jan" << "Feb" << "Mar" <<  "Apr" <<  "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" <<  "Nov" << "Dec";

	update();

	m_timerId = startTimer(5*60000);// 5minutes
	
}

RssContainer::~RssContainer()
{
	while(!m_channelsList.isEmpty())
		delete m_channelsList.takeFirst();

	delete m_httpHandler;
}

void RssContainer::getFeeds( QUrl& /*url*/ )
{
	//TODO: get url. connect to http, then parse xml
	//check memory, file then http
}

void RssContainer::addChannel( RssChannel& /*channel*/ )
{
	//TODO
	//m_channelsList.append(&channel);
}

void RssContainer::addCurChannel()
{
	m_channelsList.append( m_curChannel );
	emit(channelAdded( m_curChannel->name(), m_curChannel->countUnreaded(), false ));
	getChannelFeeds( m_channelsList.count() - 1 );
}

void RssContainer::addChannel( QString& url )
{
	QUrl qurl( url );
	if( qurl.isValid( ))
	{
		qDebug() << "host" << qurl.host() << "path: " << qurl.path();
		
		m_curChannel = new RssChannel();
		m_curChannel->setUrl( qurl );

		m_xmlParser->setItemType( XmlParser::_rssGetChannel );

		//m_http->setHost(qurl.host());
		//m_currentContId = m_http->get(qurl.path());
		//TODO: set proper state - PARSING
		m_state = EXMLPARSING;
		m_httpHandler->get( qurl );
	}
	else
		qDebug() << "Invalid url given: " << qurl;
}

void RssContainer::deleteChannel( int ind )
{
	RssChannel* channel = m_channelsList.at( ind );
	m_channelsList.removeAt( ind );
	delete channel;
}

void RssContainer::newItemParsed( QString& itemTitle, QString& itemDesc, QString& itemDate, QString& link )
{
	QDateTime date =  parseDate( itemDate );

	//We don't have to add new item if it's already on the list
	//Check pubDate of items first
	bool onTheList = false;

	if( m_isUpdating )
	{
		onTheList = m_curChannel->isItemAdded( date, &itemTitle );
	}

	if( !onTheList )
	{
		QUrl url = QUrl( link );
		m_curChannel->addItem( itemTitle, itemDesc, date, url );
		
		//emit this signal only if channel is already on the list.
		if( m_channelsList.indexOf( m_curChannel ) != -1 )
			emit( itemAdded( m_channelsList.indexOf( m_curChannel ), eNew, itemTitle, date ));
	}
}

void RssContainer::channelTitleParsed( QString& title )
{
	Q_ASSERT( m_curChannel != NULL );
	m_curChannel->setName( title );
}

void RssContainer::channelDateParsed( QString& date )
{
	Q_ASSERT( m_curChannel != NULL );
	QDateTime dateTime =  parseDate( date );
	m_curChannel->setDate( dateTime );
}

void RssContainer::documentEnd()
{
	if( m_curChannel && !m_isUpdating )
		addCurChannel();
}

QDateTime RssContainer::parseDate( QString& date )
{
	//Tue, 24 Jun 2008 23:26:24 +0200
	qDebug() << __LOG_FNAME__
	qDebug() << date;
	date = date.trimmed();
	int i = date.indexOf(" ") + 1;

	//FIXME:'+' in some cases plus are not in the date
	int j = date.indexOf("+");
	if( j == -1 )
	{
		j = date.lastIndexOf( " " );
	}
	
	j = j- 1 - i;
	
	date = date.mid( i,j );
	
	int ind = 1;
	foreach( QString month, m_monthsName )
	{
		if(( i = date.indexOf( month, 0, Qt::CaseInsensitive )) != -1 )
			date.replace( i, 3, QDate::shortMonthName( ind ));
		ind++;
	}

	QDateTime dateTime = QDateTime::fromString( date, DATEFORMAT );
	qDebug() << dateTime.toString(DATEFORMAT);
	
	return dateTime;
}

QString RssContainer::getItemDescription( int channelId, int ind )
{
	Q_ASSERT( channelId < m_channelsList.count( ));
	RssChannel* channel = m_channelsList.at( channelId );
	QString desc;
	desc = "<p><a href=\"" + channel->itemLink( ind ).toString()
							+ "\">" + channel->itemTitle( ind )
							+ "</a></p><br>" + channel->itemDescription( ind );
	return desc;
}

void RssContainer::getChannelFeeds( int ind )
{
	QList<RssItem*>* list = m_channelsList.at( ind )->itemsList();

	foreach( RssItem* item, *list )
		emit( itemAdded( ind, item->state(), item->title(), item->date( )));
}

QUrl& RssContainer::getChannelUrl( int ind )
{
	Q_ASSERT( ind < m_channelsList.count( ));
	return m_channelsList.at( ind )->url();
}

void RssContainer::storeData()
{
	qDebug() << __LOG_FNAME__ ;
	QFile file( CNFNAME );

	if( file.open( QIODevice::WriteOnly | QIODevice::Truncate ))
	{
		QTextStream stream( &file );

		stream << m_channelsList.count() << "\n";
		qDebug() << m_channelsList.count() << "saved";

		foreach( RssChannel* channel, m_channelsList )
		{
			channel->externalize( stream );
		}

		stream << RSSCNFEND << "\n";
		qDebug() << RSSCNFEND << "\n saved";

		file.close();
	}
}

void RssContainer::loadData()
{
	qDebug() << __LOG_FNAME__;
	
	QFile file( CNFNAME );
	if( file.exists( ))
	{
		if( file.open( QIODevice::ReadOnly ))
		{
			QTextStream stream( &file );
			QString line = stream.readLine();
			bool ok = false;
			
			int count = line.toInt( &ok );
			if( ok )
			{
				RssChannel* channel;
				
				for( int i = 0; i < count; i++ )
				{
					channel = new RssChannel();
					channel->internalize( stream );
					m_channelsList.append( channel );
					emit( channelAdded( channel->name(), channel->countUnreaded(), true ));
				}
			}
			if( count > 0 )
				getChannelFeeds( 0 );

			file.close();
		}
	}
}

void RssContainer::update()
{
	qDebug() << __LOG_FNAME__;
	//we have to wait for the first channel refreshing before we hit the next one
	
	if( m_channelsList.count() > 0 )
	{
		m_isUpdating = true;
		m_channelsIterator = m_channelsList.begin();
		m_curChannel = *m_channelsIterator;
		refreshCurrentChannel();
		++m_channelsIterator;
	}
}

void RssContainer::refreshCurrentChannel()
{
	QUrl qurl = m_curChannel->url();
	m_xmlParser->setItemType( XmlParser::_rssGetChannel );
	m_state = EXMLPARSING;
	m_httpHandler->get( qurl );
//	m_http->setHost(qurl.host());
//	m_currentContId = m_http->get(qurl.path());
}

void RssContainer::setItemState( int channelId, int ind, ItemState state )
{
	RssChannel* channel = m_channelsList.at( channelId );
	if( channel->setItemState( ind, state ))
		emit( unreadedCount( channelId, channel->countUnreaded( )));
}

//FROM QXmlErrorHandler
bool RssContainer::error( const QXmlParseException & exception )
{
	qDebug() << __LOG_FNAME__;
	qDebug() << exception.message();
	return true;
}

bool RssContainer::fatalError( const QXmlParseException & exception )
{
	qDebug() << __LOG_FNAME__;
	qDebug() << exception.message();
	emit(displayError( exception.message( )));
	return false;
}
bool RssContainer::warning( const QXmlParseException & exception )
{
	qDebug() << __LOG_FNAME__;
	qDebug() << exception.message();
	return true;
}
QString RssContainer::errorString() const
{
	QString msg = tr( "Xml error occurred" );
	return msg;
}

void RssContainer::timerEvent( QTimerEvent *event )
{
	qDebug() << __LOG_FNAME__;
	update();
}

void RssContainer::readAll( int channelId )
{
	m_channelsList.at( channelId)->readAll( );
	emit( unreadedCount( channelId, 0 ));
}

QString	RssContainer::getChannelDescription( int ind )
{
	Q_ASSERT( ind < m_channelsList.count( ));
	QString desc;
	desc = "<b>" + m_channelsList.at(ind)->name( )
				+ "</b><br>" + m_channelsList.at(ind)->url().toString();
	return desc;
}

void RssContainer::handleResponse( QByteArray data )
{
	if( m_state == EXMLPARSING )
	{
		bool ok = true;
		m_xmlInput.setData( data );
		
		if( m_firstData )
		{
			ok = m_xmlReader.parse( &m_xmlInput, true );
			m_firstData = false;
		}
		else
			ok = m_xmlReader.parseContinue();
		
		if ( !ok )
		{
			m_httpHandler->abort();
			m_state = EIDLE;
		}
	}
}

void RssContainer::error( int errCode )
{
	QString note = QString( "HTTP ERROR %1" ).arg( errCode );
	emit( displayError( note ));
	m_state = EIDLE;
}

void RssContainer::requestFinished( bool error )
{
	if( m_state == EXMLPARSING )
	{
		if( !error )
		{
			if( !m_xmlReader.parseContinue( ))
				qWarning("Parse error at the end of input.");
		}
	}
}

void RssContainer::progress( int done, int total )
{
	//TODO: display progress bar on status bar
	qDebug() << done << "/" << total;
}

void RssContainer::httpDone()
{
	if( m_state == EXMLPARSING )
	{
		m_firstData = true;
		if( m_isUpdating )
		{
			if( m_channelsIterator != m_channelsList.end( ))
			{
				m_curChannel = *m_channelsIterator;
				refreshCurrentChannel();
				++m_channelsIterator;
			}
			else
				m_isUpdating = false;
		}
		m_state = EIDLE;
	}
}
//EOF
