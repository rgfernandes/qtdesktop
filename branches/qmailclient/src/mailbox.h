#ifndef __MAILBOX_H__
#define __MAILBOX_H__

#include <QStringList>

#include <vmime/vmime.hpp>
#include <vmime/net/session.hpp>

enum	SERVICE {
	SERVICE_NONE,
	SERVICE_POP3,
	SERVICE_IMAP,
	SERVICE_MAILDIR
};

enum	CRYPTO {
	CRYPTO_NONE,
	CRYPTO_SSL,
	CRYPTO_TLS_TRY,
	CRYPTO_TLS_REQUIRED
};

enum	AUTH {
	AUTH_NONE,
	AUTH_TRY,
	AUTH_REQUIRED
};

class MailBox
{
public:
	MailBox(void);
	bool		setSession();
	bool		setService(SERVICE service, QString &host, int port, QString &login, QString &password, CRYPTO crypto=CRYPTO_NONE, AUTH sasl=AUTH_NONE, AUTH apop=AUTH_NONE);
	bool		setTransport(QString &host, int port, QString login, QString password, CRYPTO crypto=CRYPTO_NONE, AUTH pass=AUTH_NONE, AUTH sasl=AUTH_NONE);
	QStringList	getFolders(void);
private:
	bool		setHandler();
	vmime::ref <vmime::net::session>	session;
	vmime::ref <vmime::net::store>		store;
	vmime::ref <vmime::net::transport>	transport;
};

#endif // __MAILBOX_H__
