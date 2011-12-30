#ifndef HTTPOBSERVER_H
#define HTTPOBSERVER_H

#include <QHttp>
#include <QUrl>
#include <QObject>

#include "httpobserver.h"

class HttpHandler : public QObject
{
Q_OBJECT

public:
    HttpHandler( HttpObserver& observer, QObject* parent );
    ~HttpHandler( void );

public:
	void get( const QUrl& url );
	void abort();

public slots:
	//from http
	void httpReadyRead( const QHttpResponseHeader & resp );
	void httpRequestFinished( int id, bool error );
	void httpDataReadProgress( int done, int total );
	void httpDone( bool error );

private://data
    QHttp* 			m_http;
	HttpObserver*	m_observer;
};

#endif //HTTPOBSERVER_H
//eof
