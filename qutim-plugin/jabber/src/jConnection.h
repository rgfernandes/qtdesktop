/*****************************************************************************
    jConnection

    Copyright (c) 2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef jConnection_H
#define jConnection_H
#include <gloox/gloox.h>
#include <gloox/jid.h>
#include <gloox/clientbase.h>
#include <gloox/connectionbase.h>
#include <gloox/connectiondatahandler.h>
#include <QTcpSocket>
#include <QMutex>
#include <QMutexLocker>
#include <QHostAddress>
#include <QTimer>
#include <QNetworkProxy>

using namespace gloox;

class jConnection : public QObject, public ConnectionBase
{
	Q_OBJECT
	
public:
	jConnection();
	jConnection( ConnectionDataHandler* cdh, const QString &profile_name, const QString &account_name );
	virtual ~jConnection();
	
	void createSocket();
	void setSocket(QTcpSocket *socket);
	void setServer(const std::string& server, int port = -1);
	void loadSettings();
	void loadProxySettings();
	
	virtual ConnectionError connect();
	virtual ConnectionError recv( int timeout = -1 );
	virtual bool send( const std::string& data );
	virtual ConnectionError receive();
	virtual void disconnect();
	virtual ConnectionBase* newInstance() const;
	virtual void getStatistics( long int &totalIn, long int &totalOut ) { totalIn=m_total_bytes_in; totalOut=m_total_bytes_out; }
	virtual void cleanup();
	virtual int localPort() const;
	virtual const std::string localInterface() const;

	QTcpSocket *getSocket() { return m_socket; }
	void stopReconnect();

signals:
	void startConnection();
	void reconnecting();

public slots:
	void connected ();
	void reconnect();
	void disconnected ();
	void error ( QAbstractSocket::SocketError socketError );
	void hostFound ();
	void stateChanged ( QAbstractSocket::SocketState socketState );
	void read ( bool force = false );
	void atStartConnection();
	void atError();

protected:
	qint64 m_total_bytes_in;
	qint64 m_total_bytes_out;
	QTcpSocket *m_socket;
//	QMutex m_mutex;
	ConnectionError m_error;
//	QMutexLocker *m_mutex_locker;
	bool m_is_connecting;
	QList<QPair<QString, int> > m_hosts;
	int m_current_host;
	QString m_profile_name;
	QString m_account_name;
	bool m_use_dns_srv;
	QTimer *m_reconnect_timer;
	bool m_is_reconnect;
	QTcpSocket::SocketError m_socket_error;
	QNetworkProxy m_proxy;
};
#endif // CONNECTIONHTTPPROXY_H__
