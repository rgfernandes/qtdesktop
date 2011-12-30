#ifndef RSSITEM_H
#define RSSITEM_H
//includes
#include <QString>
#include <QDate>
#include <QTextStream>
#include <QUrl>

#include "qrssglobals.h"

class RssItem  
{

public:
	
	RssItem( QDateTime date, QString title, QString txt, QUrl link, ItemState state = eIdle );
	RssItem();
	
	~RssItem();

	void setTitle( QString& title );
	void setDate( QDateTime& date );
	void setTxt( QString& txt );
	void setLink( QUrl& link );
	void setState( ItemState state );
	
	
	QString& title () {
		return m_title;
	}
	
	QString& txt(){
		return m_txt;
	}
	
	QDateTime& date(){
		return m_date;
	}
	
	ItemState state(){
		return m_state;
	}
	
	QUrl& link(){
		return m_link;
	}

	void internalize( QTextStream& stream );
	void externalize( QTextStream& stream );

	void operator<<( QTextStream& stream );
	void operator>>( QTextStream& stream );
	
private:
	QString 	m_title;
	QDateTime 	m_date;
	QString 	m_txt;
	QUrl 		m_link;
	ItemState 	m_state;

};
#endif //RSSITEM_H
//eof