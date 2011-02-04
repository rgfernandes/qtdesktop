#include <QDebug>
#include <QTime>

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "mailbox.h"
#include <vmime/platforms/posix/posixHandler.hpp>

static bool handlerOK = false;

QStringList	cmpQSL(QStringList &sl1, QStringList &sl2) {
	// compare 2 stringlists
	// return - sl1-sl2 - difference - items, that are in sl1 and not in sl2
	QStrinList retvalue;
	for (int i = 0; i < sl1.size(); i++)
		if (!sl2->contains(sl1[i]))
			retvalue.append(sl1[i])
	return retvalue; 
}

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

void	listFolder(vmime::ref <vmime::net::folder> folder) {
	QTime ct = QTime::currentTime();
	std::cerr << "Start mail" << std::endl;
	std::vector <vmime::ref <vmime::net::message> > allMessages = folder->getMessages();	// just ask
	int foptions = folder->getFetchCapabilities();
	// Mail.ru: ENVELOPE, CONTENT_INFO, SIZE, FULL_HEADER, UID, IMPORTANCE
	std::cerr << "Messages:" << folder->getMessageCount() << std::endl;
	//folder->fetchMessages(allMessages, vmime::net::folder::FETCH_FLAGS|vmime::net::folder::FETCH_ENVELOPE);
	std::cerr << "fetch all start: " << ct.msecsTo(QTime::currentTime()) << std::endl;
	//folder->fetchMessages(allMessages, foptions);
	folder->fetchMessages(allMessages, vmime::net::folder::FETCH_UID);
	std::cerr << "fetch all ok: " << ct.msecsTo(QTime::currentTime()) << std::endl;
	for (unsigned int i = 0; i < allMessages.size(); ++i) {
		vmime::ref <vmime::net::message> msg = allMessages[i];
		const int flags = msg->getFlags();
		std::cerr << "Message " << i << ": ";
		if (flags & vmime::net::message::FLAG_SEEN)
			std::cerr << "\t - read, ";
		if (flags & vmime::net::message::FLAG_DELETED)
			std::cerr << "\t - deleted, ";
		//folder->fetchMessage(msg, foptions);
		std::cerr << "\tUID: " << msg->getUniqueId();
	//	vmime::ref <const vmime::header> hdr = msg->getHeader();
	//	std::cerr << "\tSent on: " << hdr->Date()->generate() << std::endl;
		//std::cerr << "\tSent by: " << hdr->From()->generate() << std::endl;
		//vmime::text txt = vmime::text::decodeAndUnfold(hdr->From()->getValue());
		//std::cerr << "\tSent by: " << txt << std::endl;
		//std::cerr << hdr->From()->getValue().getConvertedText(vmime::charset("utf-8"));
	}
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
	// list all folders
	vmime::ref <vmime::net::folder> rootFolder = store->getRootFolder();
	rootFolder->open(vmime::net::folder::MODE_READ_WRITE);
	std::vector <vmime::ref <vmime::net::folder> > folders = rootFolder->getFolders(false);
	for (unsigned int i = 0; i < folders.size(); i++) {
		std::cerr << folders[i]->getName().getBuffer() << std::endl;
	}
	// list msgs in INBOX
	vmime::net::folder::path path;
	path /= vmime::net::folder::path::component("INBOX");
	vmime::ref <vmime::net::folder> inbox = store->getFolder(path);
	inbox->open(vmime::net::folder::MODE_READ_WRITE);
	listFolder(inbox);
	//qDebug() << url;
	return true;
}

//bool	MailBox::setTransport(QString &host, int port, QString login, QString password, CRYPTO crypto, AUTH pass, AUTH sasl) {
//	return true;
//}

QStringList	getFolders(void);
