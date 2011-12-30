#ifndef RSSCHANNEL_H
#define RSSCHANNEL_H

//includes 
#include <QList>
#include <QString>
#include <QUrl>
#include <QDate>
#include <QTextStream>
#include "qrssglobals.h"


//forward declaration
class RssItem;

class RssChannel  
{

public:
	RssChannel( QString name, QUrl ulr );
	RssChannel();
	~RssChannel();
	
	void 		setName( QString& name );
	void 		setUrl( QUrl& url );
	void 		setDate( QDateTime& date );
	void 		addItem( RssItem& item );
	void 		addItem( QString& title, QString& desc, QDateTime& date, QUrl& link );
	QString& 	itemDescription( int ind );
	QString& 	itemTitle( int ind );
	QUrl& 		itemLink( int ind );
	void 		readAll();
	bool 		setItemState( int ind, ItemState state );
	int			countUnreaded();
	int 		itemInd( QDateTime& date, QString* title = NULL );
	bool 		isItemAdded( QDateTime& date, QString* title = NULL );
	
	QUrl& 		url(){
			return m_url;
			};
	
	QString& 	name(){
			return m_name;
			};
	
	QDateTime& 	date(){
			return m_date;
			};

	QList<RssItem*>* itemsList(){
			return &m_itemsList;
			};
	
	void 		internalize( QTextStream& stream );
	void 		externalize( QTextStream& stream );
	
private:
	QList<RssItem*>		m_itemsList;
	QUrl				m_url;
	QString				m_name;
	QDateTime			m_date;
	int					m_unreaded;
	
};
#endif //RSSCHANNEL_H
//eof

