#include <QDebug>
#include <QXmlParseException>
#include <QTextCodec>

#include "xmlparser.h"
//
XmlParser::XmlParser() 
					: QObject(), QXmlDefaultHandler()
{
	m_get = _rssGetItem;
}

XmlParser::~XmlParser()
{
}

void XmlParser::setItemType( ItemType type )
{
	m_get = type;
}

bool XmlParser::startDocument ()
{
	//TODO:document stard here, do we have to do smth ?
	//m_cod = QTextCodec::codecForName("ISO 8859-2");
	//QTextCodec::setCodecForCStrings(m_cod);

	qDebug() << "start document";
	m_item = false;
	m_getTitle = false;
	m_getDesc = false;
	m_getDate = false;
	m_getLink = false;
	m_title.clear();
	m_txt.clear();
	m_date.clear();
	m_link.clear();
	return true;
}

bool XmlParser::startElement ( const QString & /*namespaceURI*/, const QString & /*localName*/, const QString & qName,
								 const QXmlAttributes & /*atts*/ )
{
	qDebug() << "startElement: " << qName;
	
	if( "channel" == qName || "item" == qName )		
	{
		m_item = true;
		
		if ( m_get == _rssGetChannelDate )
			m_get = _rssGetItem;
	}

	if( m_item )
	{
		if( "title" == qName )
		{
			m_getTitle = true;
		}
		else if( "description" == qName )
		{
			if ( _rssGetItem == m_get )
			{
				m_getDesc = true;
			}
		}
		else if( "pubDate" == qName )
		{
			m_getDate = true;
		}
		else if( "link" == qName)
		{
			m_getLink = true;
		}
	}
	
	if( "pubDate" == qName && m_get == _rssGetChannelDate )
	{
		m_getDate = true;
		m_item = true;
	}
	
	//pubDate: QDate::fromString(ddd, dd mm yyyy H:m:s) + 0200

	return true;
}

bool XmlParser::characters( const QString &chars )
{
	qDebug() << "characters: " << chars;

	if( m_getTitle )
		m_title += chars;
	else if( m_getDesc )
		m_txt += chars;
	else if (m_getDate )
		m_date += chars;
	else if( m_getLink )
		m_link += chars;
	
	return true;
}

bool XmlParser::endElement ( const QString & /*namespaceURI*/, const QString & /*localName*/, const QString & qName )
{
	qDebug() << "endElement: " << qName;
	if( m_item )
	{

		if( qName == "pubDate" &&_rssGetChannelDate == m_get )
		{
			emit(channelDateParsed(m_date));
			m_get = _rssGetItem;
			m_getDate = false;
			m_item = false;
			m_date.clear();
		}
		
		if( "title" == qName )
		{
			if ( _rssGetChannel == m_get )
			{
				m_title = m_title.simplified();
				emit(channelTitleParsed(m_title));
				m_title.clear();
				
				//if channel finished, then we have to get first item title and description
				m_get = _rssGetChannelDate;

				//we've got channel title so wait for the item field.
				m_item = false;
			}
			else
				emitItem();
			
			m_getTitle = false;
						
		}
		else if( "description" == qName )
		{
			emitItem();
			m_getDesc = false;
		}
		else if( "pubDate" == qName )
		{
			emitItem();
			m_getDate = false;	
		}
		else if( "link" == qName )
		{
			emitItem();
			m_getLink = false;
		}
	}

	if( "channel" == qName  || "item" == qName )
		m_item = false;
	
	return true;
}

bool XmlParser::endDocument()
{
	emit(documentEnd());
	m_get = _rssGetItem;
	return true;
}

void XmlParser::emitItem()
{
	if( !m_title.isEmpty() && !m_txt.isEmpty() && !m_date.isEmpty() && !m_link.isEmpty( ))
	{
		m_title = m_title.simplified();
		m_txt = m_txt.simplified();
		emit(newItemParsed(m_title, m_txt, m_date, m_link));
	}
	else
		return;
				
	m_title.clear();
	m_txt.clear();
	m_date.clear();
	m_link.clear();
				
	m_getTitle = false;
	m_getDesc = false;
	m_getDate = false;
	m_getLink = false;
}

bool XmlParser::fatalError( const QXmlParseException &exception )
{
	qDebug() << "Fatar xml error occurred:" << exception.message();
	return true;
}
//eof
