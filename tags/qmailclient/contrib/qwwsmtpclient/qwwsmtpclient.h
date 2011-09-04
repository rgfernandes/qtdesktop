//
// C++ Interface: qwwsmtpclient
//
// Description:
//
//
// Author: Witold Wysota <wysota@wysota.eu.org>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QWWSMTPCLIENT_H
#define QWWSMTPCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QString>

#if QT_VERSION >= 0x040300 && !defined(QT_NO_OPENSSL)
#include <QSslError>
#endif

class QwwSmtpClientPrivate;

/*!
        \class  QwwSmtpClient
        \author Witold Wysota <wysota@wysota.eu.org>
        \brief  Cross-platform asynchronous handling of client side SMTP connections

                Features:

                Connection mode - open, TLS, SSL
                Authentication  - PLAIN, LOGIN
                Handshake       - HELO, EHLO

                - low-level mail sending (everything you pass, goes through to the server)
                - raw command sending
                - multiple rcpt
                - option reporting

       \todo    CRAM-MD5 Authentication
                VRFY
                abort()
                SSL errors handling
                network errors
                error handling (status codes, etc.)

*/
class QwwSmtpClient : public QObject {
    Q_OBJECT
    Q_ENUMS(State);
    Q_FLAGS(Options);
    Q_ENUMS(AuthMode);
    Q_FLAGS(AuthModes);

public:
    QwwSmtpClient(QObject *parent = 0);
    ~QwwSmtpClient();
    enum State { Disconnected, Connecting, Connected, TLSRequested, Authenticating, Sending, Disconnecting };
    enum Option { NoOptions = 0, StartTlsOption, SizeOption, PipeliningOption, EightBitMimeOption, AuthOption };
    Q_DECLARE_FLAGS ( Options, Option );
    enum AuthMode { AuthNone, AuthAny, AuthPlain, AuthLogin };
    Q_DECLARE_FLAGS ( AuthModes, AuthMode );
    void setLocalName(const QString &ln);
#if QT_VERSION >= 0x040300 && !defined(QT_NO_OPENSSL)
    void setLocalNameEncrypted(const QString &ln);
#endif

    int connectToHost ( const QString & hostName, quint16 port = 25);

#if QT_VERSION >= 0x040300 && !defined(QT_NO_OPENSSL)
    int connectToHostEncrypted(const QString &hostName, quint16 port = 465);
#endif
//     int connectToHost ( const QHostAddress & address, quint16 port = 25);
    int authenticate(const QString &user, const QString &password, AuthMode mode = AuthAny);
    int sendMail(const QString &from, const QString &to, const QString &content);
    int sendMail(const QString &from, const QStringList &to, const QString &content);
    int rawCommand(const QString &cmd);
    AuthModes supportedAuthModes() const;
    Options options() const;
    QString errorString() const;
public slots:
    int disconnectFromHost();
#if QT_VERSION >= 0x040300 && !defined(QT_NO_OPENSSL)
    int startTls();
    void ignoreSslErrors();
#endif

signals:
    void done(bool);
    void connected();
    void disconnected();
    void stateChanged(State);
    void commandFinished(int, bool error);
    void commandStarted(int);
#if QT_VERSION >= 0x040300 && !defined(QT_NO_OPENSSL)
    void tlsStarted();
    void sslErrors(const QList<QSslError> &);
#endif
    void authenticated();
    void rawCommandReply(int code, const QString &details);


private:
    QwwSmtpClientPrivate *d;
    Q_PRIVATE_SLOT(d, void onConnected());
    Q_PRIVATE_SLOT(d, void onDisconnected());
    Q_PRIVATE_SLOT(d, void _q_readFromSocket());
#if QT_VERSION >= 0x040300 && !defined(QT_NO_OPENSSL)
    Q_PRIVATE_SLOT(d, void _q_encrypted());
#endif
    friend class QwwSmtpClientPrivate;
};

#endif
