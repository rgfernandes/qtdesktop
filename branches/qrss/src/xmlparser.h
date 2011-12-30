#ifndef XMLPARSER_H
#define XMLPARSER_H
//includes
#include <QObject>
#include <QXmlDefaultHandler>
#include <QXmlParseException>
#include <QXmlAttributes>
#include <QString>

#include "rssitem.h"
//
class XmlParser : public  QObject, public QXmlDefaultHandler
{
Q_OBJECT

public:
	enum ItemType
	{
		_rssGetChannel=1,
		_rssGetItem,
		_rssGetChannelDate
	};

public:
	XmlParser();

	void setItemType( ItemType types );

	~XmlParser();
	
public:
	bool startDocument();
	bool endDocument();
	bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
	bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
	bool characters( const QString &chars );
	
	bool fatalError( const QXmlParseException &exception );

private:
	void emitItem();
signals:
	void newItemParsed( QString& itemTitle, QString& itemDesc, QString& itemDate, QString& link );
	void channelTitleParsed( QString& title );
	void channelDateParsed( QString& date );
	void documentEnd();

private:
	ItemType 	m_get;
	bool 		m_item;
	bool 		m_getTitle;
	bool 		m_getDesc;
	bool 		m_getDate;
	bool 		m_getLink;
	QString 	m_title;
	QString 	m_txt;
	QString 	m_date;
	QString 	m_link;
};
#endif
