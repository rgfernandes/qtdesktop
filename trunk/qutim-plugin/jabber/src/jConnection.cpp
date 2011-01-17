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

#include "jConnection.h"
#include <gloox/dns.h>
#include <QNetworkProxy>
#include <QTimer>
#include <QSettings>
#include "utils.h"

#if 0
# define DEBUG qDebug
#else
# define DEBUG if(1) {} else qDebug
#endif

jConnection::jConnection()
	: ConnectionBase(0), QObject(0)
{
	m_profile_name = "";
	m_account_name = "";
	m_error = ConnNotConnected;
	//    m_mutex_locker = 0;
	m_is_connecting = false;
	m_current_host = 0;
	m_socket = 0;
	m_use_dns_srv = false;
}

jConnection::jConnection( ConnectionDataHandler* cdh, const QString &profile_name, const QString &account_name )
	: ConnectionBase(cdh), QObject(0)
{
	m_profile_name = profile_name;
	m_account_name = account_name;
	m_error = ConnNotConnected;
	m_is_connecting = false;
	//    m_mutex_locker = 0;
	m_current_host = 0;
	m_socket = 0;
	QObject::connect(this, SIGNAL(startConnection()), this, SLOT(atStartConnection()));
	m_use_dns_srv = false;
	loadProxySettings();
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	settings.beginGroup("main");
	m_is_reconnect = settings.value("reconnect",true).toBool();
	settings.endGroup();
	m_reconnect_timer = new QTimer();
	m_reconnect_timer->setInterval(5000);
	QObject::connect(m_reconnect_timer, SIGNAL(timeout()), this, SLOT(reconnect()));
}

jConnection::~jConnection()
{
//	if(m_mutex_locker)
//	{
//		delete m_mutex_locker;
//		m_mutex_locker = 0;
//	}
	if(m_socket)
		m_socket->deleteLater();
	if (m_reconnect_timer)
		delete m_reconnect_timer;
}

void jConnection::createSocket()
{
	if(m_socket)
		m_socket->deleteLater();
	setSocket(new QTcpSocket());
}

void jConnection::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
	QObject::connect(m_socket,SIGNAL(connected()),this,SLOT(connected()));
    QObject::connect(m_socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    QObject::connect(m_socket,SIGNAL(hostFound()),this,SLOT(hostFound()));
    QObject::connect(m_socket,SIGNAL(readyRead()),this,SLOT(read()));
    QObject::connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(m_socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
}

void jConnection::setServer(const std::string& server, int port)
{
	m_server = server;
	m_port = port;
}

void jConnection::loadSettings()
{
	if(m_state != StateDisconnected)
	{
		DEBUG("Try to load settings in %d state", (int)m_state);
		return;
	}
	DEBUG() << Q_FUNC_INFO;
    loadProxySettings();
    m_hosts.clear();
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");

    settings.beginGroup("main");
	JID jid(utils::toStd(m_account_name));

    QString server = utils::fromStd(jid.server());
    int port = settings.value("port",5222).toInt();
    m_use_dns_srv = settings.value("usedns", true).toBool();
    TLSPolicy tls_policy = TLSOptional;
    if(server=="qutim.org")
    {
		server="jabber.qutim.org";
		port = 5222;
		m_use_dns_srv = false;
    }
    bool use_sasl = settings.value("usesasl", true).toBool();
    bool compress = settings.value("compress", true).toBool();
    server = settings.value("server",server).toString();
    m_server = utils::toStd(server);
    m_port = port;
    if(!m_use_dns_srv)
    {
		if(port==-1)
			port = 5222;
		m_hosts << qMakePair( server, port );
    }
    else
	{
		static LogSink logsink;
		DNS::HostMap hosts = DNS::resolve(m_server, logsink);
		DNS::HostMap::iterator h = hosts.begin();
		for(;h!=hosts.end();h++)
		{
			QPair<QString, int> host( utils::fromStd(h->first), h->second );
			static const QRegExp valid_hostname( "(\\w+\\.)+\\w+" );
			Q_ASSERT_X(valid_hostname.isValid(),"jConnection::loadSettings()","Regexp is not valid");
			if( valid_hostname.exactMatch( host.first ) || !QHostAddress( host.first ).isNull() )
				m_hosts.append(host);
		}
		if( m_hosts.isEmpty() )
			m_hosts << qMakePair( server, 5222 );
    }
    switch(settings.value("tlspolicy",1).toInt())
    {
	case 0:
	    tls_policy = TLSDisabled;
	    break;
	case 1:
	    tls_policy = TLSOptional;
	    break;
	case 2:
	    tls_policy = TLSRequired;
	    break;
    }
	settings.endGroup();

	ClientBase *jClient = dynamic_cast<ClientBase*>(m_handler);
	if(!jClient)
	{
		DEBUG("Try to load settings, but handler is not ClientBase. Error os %d", (int)m_error);
		m_handler->handleDisconnect(this, m_error);
		if(!(jClient = dynamic_cast<ClientBase*>(m_handler)))
		{
			DEBUG("Try lo load settings, but handle disconnect go wrong");
			return;
		}
	}

    jClient->setTls(tls_policy);
    jClient->setSasl(use_sasl);
	jClient->setCompression(compress);
}

void jConnection::loadProxySettings()
{
	DEBUG() << Q_FUNC_INFO;
	createSocket();
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
	settings.beginGroup("proxy");
	int proxy_type = settings.value("type",0).toInt();
	int proxy_port = settings.value("port",0).toInt();
	QString proxy_host = settings.value("host","").toString();
	bool proxy_auth = settings.value("auth",false).toBool();
	QString proxy_user = settings.value("user","").toString();
	QString proxy_pass = settings.value("password","").toString();
	settings.endGroup();
	switch(proxy_type)
	{
		case 0:{   // No Proxy
			m_proxy = QNetworkProxy(QNetworkProxy::NoProxy);
			m_socket->setProxy(m_proxy);
			break;
		}
		case 1:{    // HTTP
			m_proxy = QNetworkProxy(QNetworkProxy::HttpProxy, proxy_host, proxy_port);
			if (proxy_auth)
			{
				m_proxy.setUser(proxy_user);
				m_proxy.setPassword(proxy_pass);
			}
			m_socket->setProxy(m_proxy);
			break;}
		case 2:{    // SOCKS5
			m_proxy = QNetworkProxy(QNetworkProxy::Socks5Proxy, proxy_host, proxy_port);
			m_socket->setProxy(m_proxy);
			if (proxy_auth)
			{
				m_proxy.setUser(proxy_user);
				m_proxy.setPassword(proxy_pass);
			}
			break;
		}
		case 3:     // BOSH
			 // Really don not know what is it
			 // so it would be equal to no proxy
		default:{
			m_proxy = QNetworkProxy(QNetworkProxy::DefaultProxy);
			m_socket->setProxy(m_proxy);
			break;
		}
	}
}

ConnectionError jConnection::connect()
{
    if( !m_handler )
		return ConnNotConnected;
	if(m_socket && (m_socket->state() == QAbstractSocket::ConnectedState || m_socket->state() == QAbstractSocket::ConnectingState) )
		return ConnNoError;
	DEBUG() << Q_FUNC_INFO;
//    if(m_mutex_locker)
//    {
//		delete m_mutex_locker;
//		m_mutex_locker = 0;
//    }
//    m_mutex_locker = new QMutexLocker(&m_mutex);
    emit startConnection();
   // QMutexLocker locker(&m_mutex);
    return m_error;
}

typedef QPair<QString, int> QStringInt;

void jConnection::atStartConnection()
{
	DEBUG() << Q_FUNC_INFO;
	m_is_connecting = true;
    m_error = ConnNoError;
	 m_current_host = 0;
	if( m_use_dns_srv && m_hosts.size() )
	{
		m_current_host = 0;
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
		if(m_proxy.type() == QNetworkProxy::DefaultProxy)
		{
			QNetworkProxyQuery query(m_hosts[0].first, m_hosts[0].second, "xmpp");
			QList<QNetworkProxy> proxies = QNetworkProxyFactory::proxyForQuery(query);
			m_socket->setProxy(QNetworkProxy::NoProxy);
			foreach (const QNetworkProxy &p, proxies)
			{
				if((p.capabilities() & QNetworkProxy::TunnelingCapability) == 0)
					continue;
				m_socket->setProxy(p);
				break;
			}
		}
		else
#endif
			m_socket->setProxy(m_proxy);
		m_socket->connectToHost( m_hosts[0].first, m_hosts[0].second );
//		foreach(const QStringInt &host, m_hosts)
//		{
//	//      DEBUG() << m_account_name << "try to use:" << host.first << host.second;
//			m_socket->connectToHost( host.first, host.second);
//			if(m_socket->waitForConnected())
//			{
//				connected();
//				return;
//			}
//			else
//			error(m_socket->error());
//		}
	/*        DEBUG() << m_account_name << "try to use:" << m_hosts[m_current_host].first << m_hosts[m_current_host].second;
		m_socket->connectToHost( m_hosts[m_current_host].first, m_hosts[m_current_host].second);
		if(m_socket->waitForConnected())
			connected();
		else
			error(m_socket->error());*/
    }
    else
    {
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
		if(m_proxy.type() == QNetworkProxy::DefaultProxy)
		{
			QNetworkProxyQuery query(utils::fromStd(m_server), m_port, "xmpp");
			QList<QNetworkProxy> proxies = QNetworkProxyFactory::proxyForQuery(query);
			m_socket->setProxy(QNetworkProxy::NoProxy);
			foreach (const QNetworkProxy &p, proxies)
			{
				if((p.capabilities() & QNetworkProxy::TunnelingCapability) == 0)
					continue;
				m_socket->setProxy(p);
				break;
			}
		}
		else
#endif
			m_socket->setProxy(m_proxy);
//      DEBUG() << m_account_name << "try to use (no dns):" << utils::fromStd(m_server) << m_port;
		m_socket->connectToHost( utils::fromStd(m_server), m_port);
//		if(m_socket->waitForConnected())
//			connected();
//		else
//			error(m_socket->error());

    }
}

ConnectionError jConnection::recv( int /*timeout*/ )
{
//      qint64 max_size = m_socket->bytesAvailable();
//	if(max_size<=0)
	read( true );
	return m_error;
//      char *data = (char *)qMalloc(max_size*sizeof(char));
//	qint64 size = m_socket->read(data, max_size);
//	m_total_bytes_in+=size;
//	m_handler->handleReceivedData( this, std::string( data, size ) );
//  qFree(data);
//	return ConnNoError;
}

bool jConnection::send( const std::string& data )
{
	if(!m_socket)
		return false;
	int size = m_socket->write(data.c_str(),data.length());
	if(size==-1)
		return false;
	m_total_bytes_out+=size;
	m_socket->flush();
	return true;	
}

ConnectionError jConnection::receive()
{
	return m_error;
}

void jConnection::disconnect()
{
	DEBUG() << Q_FUNC_INFO;
	m_current_host = m_hosts.size();
	m_error = ConnUserDisconnected;
	if(m_socket)
		m_socket->disconnectFromHost();
}

ConnectionBase* jConnection::newInstance() const
{
	return new jConnection(m_handler, m_profile_name, m_account_name);
}

void jConnection::cleanup()
{
	DEBUG() << Q_FUNC_INFO;
	QTcpSocket *socket = m_socket;
	createSocket();
	socket->setProxy(socket->proxy());
}

int jConnection::localPort() const
{
	return m_socket ? m_socket->localPort() : -1;
}

const std::string jConnection::localInterface() const
{
	return m_socket ? utils::toStd(m_socket->localAddress().toString()) : EmptyString;
}

void jConnection::connected ()
{
	DEBUG() << Q_FUNC_INFO;
	DEBUG() << m_account_name << "connected";
	m_error = ConnNoError;
	m_handler->handleConnect( this );
	m_is_connecting = false;
	m_reconnect_timer->stop();
//	if(m_mutex_locker)
//	{
//		delete m_mutex_locker;
//		m_mutex_locker = 0;
//	}
}

void jConnection::disconnected ()
{
	/*DEBUG() << "not connected";
//    DEBUG() << m_account_name << "disconnected";
//	if(m_mutex_locker)
	if( m_is_connecting )
	{
		m_current_host++;
		if(m_current_host<m_hosts.size() && m_use_dns_srv)
		{
			m_error = ConnNotConnected;
			m_socket->connectToHost( m_hosts[m_current_host].first, m_hosts[m_current_host].second);
			return;
		}
//		delete m_mutex_locker;
//		m_mutex_locker = 0;
	}
	error(m_socket->error());
	m_handler->handleDisconnect( this, m_error );*/
	//m_error = ConnNotConnected;
}

ConnectionError toGlooxError(QTcpSocket::SocketError error)
{
	switch(error)
	{
	case QAbstractSocket::ConnectionRefusedError:
		return ConnConnectionRefused;
//	case QAbstractSocket::RemoteHostClosedError:
//		return ConnUserDisconnected;
	case QAbstractSocket::HostNotFoundError:
		return ConnDnsError;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		return ConnProxyAuthRequired;
	case QAbstractSocket::SslHandshakeFailedError:
		return ConnTlsFailed;
	case QAbstractSocket::SocketAddressNotAvailableError:
	case QAbstractSocket::AddressInUseError:
	case QAbstractSocket::NetworkError:
	case QAbstractSocket::SocketTimeoutError:
	case QAbstractSocket::SocketResourceError:
	case QAbstractSocket::SocketAccessError:
	case QAbstractSocket::UnsupportedSocketOperationError:
	case QAbstractSocket::DatagramTooLargeError:
	case QAbstractSocket::UnknownSocketError:
	case QAbstractSocket::UnfinishedSocketOperationError:
	default:
		return ConnIoError;
	}
}

void jConnection::atError()
{
	DEBUG() << Q_FUNC_INFO;
	DEBUG() << m_account_name << "atError" << m_socket_error;
	if( m_is_connecting && m_socket )
	{
		m_current_host++;
		if(m_current_host<m_hosts.size() && m_use_dns_srv)
		{
			m_error = ConnNotConnected;
			m_socket->connectToHost( m_hosts[m_current_host].first, m_hosts[m_current_host].second);
			return;
		}
	}
//	if (m_reconnect_timer->interval() == 5000)
//		m_handler->handleDisconnect(this, m_error);
	m_error = ConnNotConnected;
	if (m_is_reconnect && !m_reconnect_timer->isActive())
		m_reconnect_timer->start();
}

void jConnection::error ( QAbstractSocket::SocketError socketError )
{
	DEBUG() << Q_FUNC_INFO << socketError;
	if(m_error == ConnUserDisconnected)
		return;
	m_socket_error = socketError;
	m_error = toGlooxError(socketError);
	QTimer::singleShot(0, this, SLOT(atError()));
}

void jConnection::reconnect()
{
	if(!m_handler || m_socket && (m_socket->state() == QAbstractSocket::ConnectedState || m_socket->state() == QAbstractSocket::ConnectingState) )
		return;
	if (m_reconnect_timer->interval() == 5000)
		m_reconnect_timer->setInterval(10000);
	emit startConnection();
	emit reconnecting();
}

void jConnection::hostFound ()
{
	DEBUG() << Q_FUNC_INFO;
//    DEBUG() << m_account_name << "hostFound";
	m_error = ConnNoError;
}

void jConnection::stateChanged ( QAbstractSocket::SocketState socketState )
{
	DEBUG() << Q_FUNC_INFO;
	DEBUG() << m_account_name << "stateChanged" << socketState;
	 switch(socketState)
	 {
	 case QAbstractSocket::HostLookupState:
	 case QAbstractSocket::ConnectingState:
	 	m_state = StateConnecting;
	 	break;
	 case QAbstractSocket::ConnectedState:
	 	m_state = StateConnected;
	 	break;
	 case QAbstractSocket::ClosingState:
	 case QAbstractSocket::ListeningState:
	 case QAbstractSocket::UnconnectedState:
		if(m_state != StateDisconnected)
		{
			DEBUG() << "m_handler->handleDisconnect";
			m_handler->handleDisconnect(this, m_error);
		}
	 	m_state = StateDisconnected;
	 	break;
	 default:
		break;
	 }
}

void jConnection::read( bool force )
{
	if(!m_socket || ( !force && m_socket != sender()))
		return;
	qint64 max_size = m_socket->bytesAvailable();
	if(max_size<=0)
		return;
	if(!m_handler)
	{
		QTimer::singleShot(50, this, SLOT(read()));
		return;
	}
	char *data = (char *)qMalloc(max_size*sizeof(char));
	qint64 size = m_socket->read(data, max_size);
	m_total_bytes_in+=size;
	m_handler->handleReceivedData( this, std::string( data, size ) );
	qFree(data);
}

void jConnection::stopReconnect()
{
	m_reconnect_timer->stop();
}
