/****************************************************************************
* http://sourceforge.net/projects/nlcreator/
*
* QNewsletterCreator - Business Email Client for Mass Mails
* Nuntius Leo - Personal Qt Email Client
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License Version 2 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
* USA. http://www.gnu.org/copyleft/library.html
*
* Please report all bugs and problems to the project admins:
* http://sourceforge.net/projects/nlcreator/
* 
*
* Copyright by dontinelli@users.sourceforge.net if no additional copyright information is given
* otherwise copyright by given authors/organizations 
* and modfified, e.g. ported, by dontinelli@users.sourceforge.net.
* Some code has been taken from 
* http://sourceforge.net/projects/lhmail - License : GNU Library General Public License    
* Authors: lukasz.iwaszkiewicz@gmail.com
* lukasz.iwaszkiewicz@lefthand.com.pl, lukasz.iwaszkiewicz@software.com.pl 
* Copyright (C) 2004/2005 LeftHand sp.z.o.o. info@lefthand.com.pl 
*
*****************************************************************************/

#ifndef SMTP_H
#define SMTP_H

#include <QtNetwork>
#include <QApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QDate>
#include <QThread>
#include "mail.h"

enum State {
	Wait,
	Disconnected,
	Connect,
	Connected,
	Auth,
	Pass,
	AuthOk,
	Rcpt,
	Data,
	Send,
	Sent,
	Noop,
	Close,
	Quit,
	None
};

/**
* Usage: Build a Mail, build a smtp-object, start thread with start(), call sendMail(Mail *m) when
* ReadyToSend() is emitted. For disconnecting, call disconnectSmtp();
* If smtp should be destroyed after the sending, connect the terminate()-signal with the
* distroying routine. It is, however, also possible to send another mail after having called run().
*
*/

#ifdef Q_OS_WIN32
	__declspec(dllexport)
#endif

class Smtp : public QThread
{
    Q_OBJECT
   
public:
	Smtp(const QString &lsmtpusername, const QString &lsmtppass, const QString &lsmtphost, QObject *parent) 
	: QThread(parent),error(false),newMail(false),readyToSend(false),preserveMails(false), state(Disconnected),read_state(Disconnected),sentMail(false),
	quitAfterSending(false),mailsSent(0){
		smtpusername=lsmtpusername;
		smtppass=lsmtppass;
		smtphost=lsmtphost;
		t=0;
		smtpsocket=0;
		return;
	};
	~Smtp();
	
	QStringList ErrorMSG;
	
	void setPreserveMails(bool p){preserveMails=p;};
	
	// Send the mail
	void queueMail(Mail *m, bool qas=false);
	void sendMail(Mail *m, bool qas=false);
	
	// Disconnect the socket
	void disconnectSmtp();
	bool error;
	bool isRunning(){return running;};
	int sentMails(){return mailsSent;};
	int mailsToSend(){return queuedMails.size();};

protected:
	// start the thread and connect to the server
	void run();
 
signals:
	void status( const QString &);
	void ConnectorSuccess();
	void connected();
	void sendNextLine();
	void ErrorCloseAll();
	void SuccessQuit();
	void ReadyToSend();
	void mailSent(Mail *m);

private slots:
	void grabLine();
	void exitLoop();
	void ReadLiner();
	void nextLine();
	void sendLine(QString senddata);

private:
	bool newMail;
	bool readyToSend;
	bool preserveMails;
	Mail *currentMail;
	QList<Mail *> queuedMails;	
	QString smtpusername;
	QString smtppass;
	QString smtphost;
	QString message;
	QString output;
	QString RemoteServerName;
	QString mailstatus;
	State state;
	State read_state;
	bool sentMail;
	bool quitAfterSending;
	int mailsSent;
	QTextStream *t;
	QTcpSocket *smtpsocket;
	QString from;
	QStringList bcc;
	QString rcpt;
	QString response;
	int Timeout;
	QString encodeBase64( QString xml );
	QString decodeBase64( QString xml );
	QString TimeStampMail();
	bool running;
	QMutex mutex;
};

#endif
