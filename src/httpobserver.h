#ifndef __HTTPOBSERVER_H__
#define __HTTPOBSERVER_H__

class HttpObserver
{

public:
	virtual void handleResponse( QByteArray data ) = 0;
	virtual void error( int errCode ) = 0;
	virtual void requestFinished( bool error ) = 0;
	virtual void progress( int done, int total ) = 0;
	virtual void httpDone() = 0;
};//__HTTPOBSERVER_H__

#endif 
//eof
