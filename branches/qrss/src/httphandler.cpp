#include <QDebug>
#include "httphandler.h"
#include "qrssglobals.h"

HttpHandler::HttpHandler( HttpObserver& observer, QObject* parent )
																:QObject( parent )
																, m_observer( &observer )
{
	m_http = new QHttp();
	
	connect( m_http, SIGNAL( readyRead( const QHttpResponseHeader & )),
			this, SLOT( httpReadyRead( const QHttpResponseHeader & )));

	connect( m_http, SIGNAL( requestFinished( int , bool )),
			this, SLOT( httpRequestFinished( int, bool )));

	connect( m_http, SIGNAL( dataReadProgress( int, int )),
			this, SLOT( httpDataReadProgress( int, int )));

	connect( m_http, SIGNAL( done( bool )),
			this, SLOT( httpDone( bool )));
}

HttpHandler::~HttpHandler()
{
	delete m_http;
}


void HttpHandler::httpReadyRead( const QHttpResponseHeader & resp )
{
	qDebug() << __LOG_FNAME__
	int status = resp.statusCode();

	if( status != 200 )
	{
		m_http->abort();
		m_observer->error( status );
	}
	else
	{
		m_observer->handleResponse( m_http->readAll( ));
	}

}

void HttpHandler::httpRequestFinished( int id, bool error )
{
	m_observer->requestFinished( error );
}

void HttpHandler::httpDataReadProgress( int done, int total )
{
	m_observer->progress( done, total );
}

void HttpHandler::httpDone( bool error )
{
	if( error )
	{
		m_observer->error( m_http->error( ));
	}
	else
	{
		m_observer->httpDone();
	}
}

void HttpHandler::get( const QUrl& url )
{
	m_http->setHost(url.host( ));
	m_http->get(url.path( ));
}

void HttpHandler::abort()
{
	m_http->abort();
}
//eof
