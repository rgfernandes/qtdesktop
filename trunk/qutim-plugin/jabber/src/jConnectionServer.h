/*****************************************************************************
    jConnection

    Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef JCONNECTIONSERVER_H
#define JCONNECTIONSERVER_H

#include <gloox/gloox.h>
#include <gloox/connectionbase.h>
#include <gloox/connectionhandler.h>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

using namespace gloox;

class jConnectionServer : public QObject, public ConnectionBase
{
    Q_OBJECT
public:
    jConnectionServer(ConnectionHandler* connection_handler);
    virtual ~jConnectionServer();
    virtual ConnectionError recv(int timeout=-1);
    virtual ConnectionError connect();
    virtual ConnectionBase* newInstance() const;
    virtual bool send(const std::string&);
    virtual ConnectionError receive();
    virtual void disconnect();
	virtual void getStatistics(long int&, long int&);
	virtual int localPort() const;
	virtual const std::string localInterface() const;
public slots:
    void newConnection();
private:
    QTcpServer *m_tcp_server;
    ConnectionHandler *m_connection_handler;
};

#endif // JCONNECTIONSERVER_H
