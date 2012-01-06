#include "QDebug"

#include "rssitem.h"
#include "qrssglobals.h"


RssItem::RssItem( QDateTime date, QString title, QString txt, QUrl link,  ItemState state )
{
	m_date = date;
	m_title = title;
	m_txt =txt;
	m_state = state;
	m_link = link;
}

RssItem::RssItem()
{
	m_state = eIdle;
}

RssItem::~RssItem()
{
	
}

void RssItem::setTitle( QString& title )
{
	m_title = title;	
}

void RssItem::setDate( QDateTime& date )
{
	m_date = date;
}

void RssItem::setTxt( QString& txt )
{
	m_txt = txt;
}

void RssItem::setLink( QUrl& link )
{
	m_link = link;
}

void RssItem::setState( ItemState state )
{
	m_state = state;
}

void RssItem::internalize( QTextStream& stream )
{
	//qDebug() << __LOG_FNAME__;
	
	QString line = stream.readLine();
	int i = 0, j =0;
	bool ok = false;
	
	while(( j = line.indexOf( ITEMSEPARATOR, j )) != -1 )
	{
		if( m_title.isEmpty( ))
		{
			m_title = line.mid( i, j-i );
			//qDebug() << "title " << m_title;
		}
		else if( m_txt.isEmpty( ))
		{
			m_txt = line.mid( i, j-i );
			//qDebug() << "desc " << m_txt;
		}
		else if( m_link.isEmpty( ))
		{
			m_link = line.mid( i, j-i );
			//qDebug() << "link " << m_link;
		}
		else if( m_state == 0 )
		{
			m_state = ( ItemState )line.mid( i, j-i ).toInt( &ok );
			if( !ok )
			{
				//qDebug() << "state:: incorrect state passed";
				m_state = eNew;
			}
			else
			{
				ok = false;
				//qDebug() << "state::" << m_state;
				if( m_state == eNew )
					m_state = eUnreaded;
			}
				
		}
		else if( m_date.isNull( ))
		{
			m_date = QDateTime::fromString( line.mid( i, j-i ), DATEFORMAT ); 
			//qDebug() << "__date:"  << m_date.toString( DATEFORMAT );
		}
		
		j+=ITEMSEPARATORLENGTH;
		i = j;
	}
}

void RssItem::externalize( QTextStream& stream )
{
	//qDebug() << __LOG_FNAME__;
	
	stream << m_title << ITEMSEPARATOR;
	//qDebug() << m_title << ITEMSEPARATOR << "saved";

	stream << m_txt << ITEMSEPARATOR;
	//qDebug() << m_txt << ITEMSEPARATOR << "saved";

	stream << m_link.toString() << ITEMSEPARATOR;
	//qDebug() << m_link.toString() << ITEMSEPARATOR << "saved";
	
	stream << ( int ) m_state << ITEMSEPARATOR;
	//qDebug() << ( int ) m_state << ITEMSEPARATOR << "state saved";
	
	if( m_date.isValid( ))
		stream << m_date.toString( DATEFORMAT ) << ITEMSEPARATOR << "\n";
	else
	{
		//qDebug() << "invalid date";
		stream << ITEMSEPARATOR << "\n";
	}
	
	//qDebug() << m_date.toString( DATEFORMAT ) << ITEMSEPARATOR << "saved";
}

void RssItem::operator>>( QTextStream& stream )
{
	externalize( stream );
}

void RssItem::operator<<( QTextStream& stream )
{
	internalize( stream );
}
