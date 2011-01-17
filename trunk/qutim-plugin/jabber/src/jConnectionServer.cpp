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

#include "jConnectionServer.h"
#include "jConnection.h"
#include "utils.h"

jConnectionServer::jConnectionServer(ConnectionHandler* connection_handler)
    : QObject(0), m_tcp_server(0), ConnectionBase(0), m_connection_handler(connection_handler)
{
}

jConnectionServer::~jConnectionServer()
{
    if(m_tcp_server)
	delete m_tcp_server;
}

ConnectionError jConnectionServer::recv(int )
{
    if(m_tcp_server->isListening())
	return ConnNoError;
    else
	return ConnNotConnected;
}

ConnectionError jConnectionServer::connect()
{
    if(!m_tcp_server)
    {
	m_tcp_server = new QTcpServer();
	QObject::connect( m_tcp_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    }

    if(m_tcp_server->isListening())
	return ConnNoError;

    bool result;
    if(m_server.empty())
	result = m_tcp_server->listen(QHostAddress::Any, m_port);
    else
	result = m_tcp_server->listen(QHostAddress(utils::fromStd(m_server)), m_port);
    qDebug() << "server connect" << utils::fromStd(m_server) << m_port << result << m_tcp_server->errorString();
    if(result)
	return ConnNoError;
    else
	return ConnIoError;
}

ConnectionBase* jConnectionServer::newInstance() const
{
    jConnectionServer *server = new jConnectionServer(m_connection_handler);
    server->setServer(m_server, m_port);
    return server;
}

void jConnectionServer::newConnection()
{
    jConnection *conn = new jConnection();
    QTcpSocket *socket = m_tcp_server->nextPendingConnection();
    conn->setSocket(socket);
    conn->setServer(utils::toStd(socket->peerAddress().toString()), socket->peerPort());
	m_connection_handler->handleIncomingConnection(this, conn);
    qDebug() << "new connection" << socket->bytesAvailable() << socket->state();
    conn->read();
}

bool jConnectionServer::send(const std::string&)
{
    qDebug() << "jConnectionServer::send";
    return false;
}

ConnectionError jConnectionServer::receive()
{
    return recv();
}
void jConnectionServer::disconnect()
{
    m_tcp_server->close();
}

void jConnectionServer::getStatistics(long int &in, long int &out)
{
    in = 0;
    out = 0;
}

int jConnectionServer::localPort() const
{
	return m_tcp_server ? m_tcp_server->serverPort() : -1;
}

const std::string jConnectionServer::localInterface() const
{
	return m_tcp_server ? utils::toStd(m_tcp_server->serverAddress().toString()) : EmptyString;
}
