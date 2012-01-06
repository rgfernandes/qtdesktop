#ifndef RSSCONTAINER_H
#define RSSCONTAINER_H

//includes
#include <QObject>
#include <QList>
#include <QString>
#include <QHttp>
#include <QHttpResponseHeader>
#include <QXmlSimpleReader>
#include <QXmlSimpleReader>
#include <QXmlDefaultHandler> 

#include "rsschannel.h"
#include "rssitem.h"
#include "qrssglobals.h"
#include "httphandler.h"
#include "httpobserver.h"

//forward declaration
class QHttp;
class XmlParser;

class RssContainer : public QObject, public QXmlDefaultHandler, public HttpObserver
{
Q_OBJECT

public:
	enum RssState
	{
		EIDLE = 0,
		EXMLPARSING
	};

	RssContainer( QObject* parent );
	virtual ~RssContainer();
	
	void 		update();
	void 		addChannel( RssChannel& channel );
	void 		addChannel( QString& url );
	void		deleteChannel( int ind );
	void		readAll( int channelId );

	void 		getFeeds( QUrl& url );
	void 		getChannelFeeds( int ind );
	QUrl&		getChannelUrl( int ind );
	QString 	getItemDescription( int channelId, int ind );
	void		setItemState( int channelId,int ind, ItemState state );
	QString		getChannelDescription( int ind );

	void		storeData();
	void		loadData();

public: //from QXmlErrorHandler
	bool		error ( const QXmlParseException & exception );
	QString		errorString() const;
	bool		fatalError( const QXmlParseException & exception );
	bool		warning( const QXmlParseException & exception );

public: // from HttpObserver
	void		handleResponse( QByteArray data );
	void		error( int errCode ) ;
	void		requestFinished( bool error );
	void		httpDone();
	void		progress( int done, int total );
	
private:
	void 		addCurChannel();
	QDateTime	parseDate( QString& date );
	void		refreshCurrentChannel();
	void		timerEvent( QTimerEvent *event );
	
//SLOTS
public slots:

	//from xmlParser
	void		newItemParsed( QString& itemTitle, QString& itemDesc, QString& itemDate, QString& link );
	void		channelTitleParsed( QString& name );
	void		channelDateParsed( QString& date );
	void		documentEnd();

//SIGNALS
signals:
	void 		itemAdded( int, ItemState, QString&, QDateTime& );
	void 		channelAdded( QString&, int, bool );
	void 		displayError( QString );
	void 		unreadedCount( int, int );

private:
	QList<RssChannel*>		m_channelsList;
	QList<RssChannel*>::iterator	m_channelsIterator;
	HttpHandler*			m_httpHandler;
	QXmlInputSource 		m_xmlInput;
	QXmlSimpleReader 		m_xmlReader;
	XmlParser* 			m_xmlParser;
	bool 				m_firstData;
	RssChannel* 			m_curChannel;
	QStringList			m_monthsName;
	bool				m_isUpdating;
	int				m_timerId;
	RssState			m_state;
};
#endif // RSSCONTAINER_H
//EOF

