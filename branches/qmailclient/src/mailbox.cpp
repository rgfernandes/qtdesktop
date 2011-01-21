#include <QDebug>

#include "mailbox.h"
#include <vmime/platforms/posix/posixHandler.hpp>

static bool handlerOK = false;

MailBox::MailBox(void) {
	setHandler();
}

bool	MailBox::setHandler(void) {
	if (!handlerOK) {
		vmime::platform::setHandler <vmime::platforms::posix::posixHandler>();
		handlerOK = true;
	}
	return true;
}

bool	MailBox::setSession(void) {
	session = vmime::create <vmime::net::session> ();
	return true;
}

bool	MailBox::setService(SERVICE service, QString &host, int port, QString &login, QString &password, CRYPTO crypto, AUTH sasl, AUTH apop) {
	// crypto: none/ssl/tls>none/tls
	// auth: pass+sasl+apop
	QString qurl;

	if (service == SERVICE_POP3) {
		qurl = QString("pop3://");
		session->getProperties()["store.pop3.auth.username"] = login.toStdString();
		session->getProperties()["store.pop3.auth.password"] = password.toStdString();
		session->getProperties()["store.pop3.server.port"] = port;
		if (crypto >= CRYPTO_TLS_TRY) {
			session->getProperties()["store.pop3.connection.tls"] = true;
			if (crypto == CRYPTO_TLS_REQUIRED)
				session->getProperties()["store.pop3.connection.tls.required"] = true;
		}
		if (sasl >= AUTH_TRY) {
			session->getProperties()["store.pop3.options.sasl"] = true;
			if (sasl == AUTH_REQUIRED)
				session->getProperties()["store.pop3.options.sasl.fallback"] = true;
		}
		if (apop >= AUTH_TRY) {
			session->getProperties()["store.pop3.options.apop"] = true;
			if (apop == AUTH_REQUIRED)
				session->getProperties()["store.pop3.options.apop.fallback"] = true;
		}
	}
	qurl += host;
	store = session->getStore(vmime::utility::url(qurl.toStdString()));
	try {
		store->connect();
	} catch (vmime::exception &e) {
	//} catch (vmime::exception::socket_exception &e) {
		qDebug() << QString("Exception:");
		qDebug() << e.name();
		qDebug() << e.what();
	}
	vmime::ref <vmime::net::folder> rootFolder = store->getRootFolder();
	rootFolder->open(vmime::net::folder::MODE_READ_WRITE);
	std::vector <vmime::ref <vmime::net::folder> > folders = rootFolder->getFolders(false);
	for (unsigned int i = 0; i < folders.size(); i++) {
		qDebug() << QString(folders[i]->getName());
	}
	//qDebug() << url;
	return true;
}

bool	MailBox::setTransport(QString &host, int port, QString login, QString password, CRYPTO crypto, AUTH pass, AUTH sasl) {
	return true;
}

QStringList	getFolders(void);
