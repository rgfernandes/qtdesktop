#include <QDebug>

#include "rsschannel.h"
#include "rssitem.h"
#include "qrssglobals.h"


RssChannel::RssChannel( QString name, QUrl url ) 
{
	//TODO: add date
	m_name = name;
	m_url = url;
	m_unreaded = 0;
}

RssChannel::RssChannel()
{
	m_unreaded = 0;
}

RssChannel::~RssChannel()
{
	while( !m_itemsList.isEmpty( ))
		delete m_itemsList.takeFirst();
}

void RssChannel::setName(QString& name)
{
	m_name = name;
}

void RssChannel::setUrl( QUrl& url )
{
	m_url = url;
}

void RssChannel::addItem( RssItem& item )
{
	m_itemsList.append( &item );
}

void RssChannel::setDate( QDateTime& date )
{
	m_date = date;
}

void RssChannel::addItem( QString& title, QString& desc, QDateTime& date, QUrl& link )
{
	RssItem* item = new RssItem( date, title, desc, link, eNew );
	m_itemsList.append( item);
	m_unreaded++;
}

QString& RssChannel::itemDescription( int ind )
{
	Q_ASSERT( ind < m_itemsList.count( ));
	//qDebug() << m_itemsList.count();

	return m_itemsList.at( ind )->txt();
}

QString& RssChannel::itemTitle( int ind )
{
	Q_ASSERT( ind < m_itemsList.count( ));
	
	return m_itemsList.at( ind )->title();
}
QUrl& RssChannel::itemLink( int ind )
{
	Q_ASSERT( ind < m_itemsList.count( ));
	
	return m_itemsList.at( ind )->link();
}

void RssChannel::internalize( QTextStream& stream )
{
	//qDebug() << __LOG_FNAME__;
	
	QString line = stream.readLine();
	int i = 0,j =0;
	int cnt = -1;
	
	while(( j = line.indexOf( ITEMSEPARATOR, j )) != -1 )
	{
		if( m_url.isEmpty( ))
		{
			m_url.setUrl( line.mid( i, j-i ));
			//qDebug() << "url: " << m_url.toString();
		}
		else if( m_name.isEmpty( ))
		{
			m_name = line.mid( i, j-i );
			//qDebug() << "name: " << m_name;
		}
		else if( cnt == -1 )
		{
			bool ok = false;
			cnt = ( line.mid( i, j-i )).toInt( &ok );
			if( !ok )
			{
				//qDebug() << "toInt failed ";
				cnt = 0;
			}
			else
				//qDebug() << "cnt: " << cnt;
				;
		}
		//qDebug() << i << j;
		j += ITEMSEPARATORLENGTH;
		i = j;
		//qDebug() << i << j;
	}

	RssItem* item;
	for( i = 0; i < cnt; i++ )
	{
		item = new RssItem();
		item->internalize( stream );
		if( item->state() != eReaded )
			m_unreaded++;
		m_itemsList.append( item );
	}
	
	line = stream.readLine();
	//if( line.indexOf(CHANNELSEPARATOR) != -1 )
	//	qDebug() << "</channel>";
	//else
	//	qDebug() << "channel internalize error";
}

void RssChannel::externalize( QTextStream& stream )
{
	//qDebug() << __LOG_FNAME__;
	
	stream << m_url.toString() << ITEMSEPARATOR;
	//qDebug() << m_url.toString() << ITEMSEPARATOR << "saved";
	
	stream << m_name << ITEMSEPARATOR;
	//qDebug() << m_name << ITEMSEPARATOR << "saved";

	stream << m_itemsList.count() << ITEMSEPARATOR;

	//qDebug() << m_itemsList.count() << ITEMSEPARATOR << "saved";
	stream << "\n";
	
	foreach( RssItem* item, m_itemsList )
		item->externalize( stream );

	//qDebug() << CHANNELSEPARATOR << "saved";
	stream << CHANNELSEPARATOR << "\n";
}

int RssChannel::itemInd( QDateTime& date, QString* title )
{
	//TODO: compare itemst date, if date == iDate return ind, if date > iDate return 0, if date
	return -1;
}

bool RssChannel::isItemAdded( QDateTime& date, QString* title )
{	
	foreach( RssItem* item, m_itemsList )
	{
		if ( date == item->date() )
		{
			if( title != NULL )
			{
				if( *title == item->title( ))
					return true;
				else
					return false;
			}
			else
				return true;
		}
		else if ( date < item->date( ))
		{
			return true; //item should be sorted by datess
		}
	}
	return false;
}

bool RssChannel::setItemState( int ind, ItemState state )
{
	Q_ASSERT( ind < m_itemsList.count( ));
	RssItem* item = m_itemsList.at( ind );
	bool res = false;
	if( state == eReaded && item->state() != eReaded )
	{
		m_unreaded--;
		res = true;
	}

	item->setState( state );
	
	//true if unreaded counter changed
	return res;
}

int RssChannel::countUnreaded()
{
	/*
	//FIXME:
	foreach( RssItem* item, m_itemsList)
	{
		if( item->state() != eReaded )
			m_unreaded++;
	}
	*/
	return m_unreaded;
}

void RssChannel::readAll()
{
	foreach( RssItem* item, m_itemsList )
		item->setState( eReaded );

	m_unreaded = 0;
}
//EOF
