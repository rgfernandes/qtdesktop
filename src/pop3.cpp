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

#include "pop3.h"
#include "mail.h"

#include <qtextstream.h>
#include <qtcpsocket.h>
#include <qhostinfo.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qregexp.h>

enum State {
	Wait,
	Disconnected,
	Connected,
	Init,
	Pass,
	Stat,
	List,
	Retr,
	Dele,
	Quit,
	Close,
	None
    };

class Pop3Private
{
public:

	Pop3Private(Pop3* _parent, const QString &pop3, const QString & _user, const QString & _passwd):
	parent(_parent),
	pop3_server(pop3),
	socket(0),
	t(0),
	user(_user),
	passwd(_passwd),
    state(Disconnected),
	read_state(Disconnected),
    mxLookup(0),
	bytes_received(0),
	//response(),
	//responseLine(),
	mails_to_read(0),
	bytes_to_read(0),
	firstLine(1),
	readData(0),
	curr_mail(0),
	read_data(),
	wait_for_line(TRUE),
	wait_for_data(FALSE),
	delete_mails(TRUE)
	{
		// socket = new QSocket(parent);
		socket = new QTcpSocket (parent);
	};
	
	~Pop3Private()
	{
//qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "~LHPop3Private() start");
		if (socket)
		{
			delete socket;
		}
		if (t)
		{
			delete t;
		}
		if (mxLookup)
		{
			delete mxLookup;
		}
//qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "~LHPop3Private() STOP");
	};

	Pop3* parent;
	QString pop3_server;
    // QSocket *socket;
    QTcpSocket *socket;
    QTextStream *t;
	QString user;
	QString passwd;
    State state;
	State read_state;
    QHostInfo * mxLookup;
	uint bytes_received;
	//QString response;
	//QString responseLine;
	uint mails_to_read;
	uint bytes_to_read;
	bool firstLine;
	bool readData;
	uint curr_mail;
	QString read_data;
	bool wait_for_line;
	bool wait_for_data;
	bool delete_mails;
};

Pop3::Pop3(const QString &user, const QString &passwd, const QString &smtp_server, QObject *parent)
 : QObject(parent)
{
	d = new Pop3Private(this, smtp_server, user, passwd);
	
	//connect ( d->socket, SIGNAL( readyReadMs() ),
	//		  this, SLOT( readyRead() ) );
	
	connect ( d->socket, SIGNAL( readyRead()),this, SLOT( readyRead()));
	
	/*
	connect ( d->socket, SIGNAL( bytesWritten(int) ),
			  this, SLOT( emitBytesWritten(int) ) );
	*/
	
	connect ( d->socket, SIGNAL( connected() ),
			  this, SLOT( connected() ) );

	connect ( d->socket, SIGNAL( connectionClosed() ),
			  this, SLOT( connectionClosed() ) );

	d->state = Disconnected;
}


Pop3::~Pop3()
{
//qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "Destructor LHPOP3");
	delete d;
}


void Pop3::getAllMails(bool del)
{
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "Pop3::getAllMails");
	d->delete_mails = del;
	openConnection();
}

void Pop3::openConnection()
{
	//from here we go to dnsLookupHelper, when connect is done
	if (d->state != Disconnected)
	{
		qDebug("Wrong state in connect");
		emit status( tr( "Wrong state in connect") );
                emit (error (tr( "Wrong state in connect")));
		return;
	}
	//QHostInfo::lookupHost(d->pop3_server,this,SLOT(dnsLookupHelper(QHostInfo)));
	emit status( tr( "Connecting to %1" ).arg( d->pop3_server ) );

	d->state = Init;
	d->read_state = None;
	d->socket->connectToHost( d->pop3_server , 110 );
	d->t = new QTextStream( d->socket );
}

void Pop3::dnsLookupHelper(const QHostInfo &hostInfo)
{
	QString host = "";
	*d->mxLookup = hostInfo;
	QList<QHostAddress> s = d->mxLookup->addresses();
	if (hostInfo.error() != QHostInfo::NoError) {
		qDebug() << "Lookup failed:" << hostInfo.errorString();
		return;
	}
	if (s.isEmpty())
	{
		host = d->pop3_server;
	}
	else
	{
		host = s.first().toString();
	}

	emit status( tr( "Connecting to %1" ).arg( host ) );

	d->state = Init;
	d->read_state = None;
	d->socket->connectToHost( host , 110 );
	d->t = new QTextStream( d->socket );
}

void Pop3::connected()
{
	emit status( tr( "Connected to %1" ).arg( d->socket->peerName() ) );
}

void Pop3::connectionClosed()
{
	emit status( tr( "Connection to %1 closed" ).arg( d->socket->peerName() ) );
	emit disconnected(this);
	d->state = Disconnected;
	//deleteLater();
}

/*
void Pop3::emitBytesWritten(int bw)
{
	d->bytes_written += bw;
	emit bytesWritten(d->bytes_written);
	emit status( tr("Bytes written: %1").arg(d->bytes_written));
}
*/

void Pop3::parseMail(const QString& mail_data)
{
	Mail* mail = new Mail();
	//qDebug()<<"parse Mail";
	mail->setData(mail_data);
	emit newMail(mail);
}

void Pop3::parseStatLine(const QString& line)
{
	QStringList sl = line.split(" "); 
	
	
	d->mails_to_read = sl[1].toUInt();
	d->bytes_to_read = sl[2].toUInt();
	
	//qDebug("Stat line: %s, mails: %d, bytes: %d", line.toAscii(), d->mails_to_read, d->bytes_to_read);
}

void Pop3::readyRead()
{
	uint bytes = d->socket->bytesAvailable();
	//char* buf = (char*) malloc(bytes+1);
	//memset(buf, 0, bytes+1);
	//uint r = d->socket->read(buf, bytes);
	//buf[r+1] = 0;
	d->read_data += d->socket->readAll();

        // qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "d->read_data");
        // qDebug ("*** %s,%d : %s", __FILE__, __LINE__, d->read_data);
        
	if (d->wait_for_line)
	{
		int nl = d->read_data.indexOf("\n");
		qDebug("ready Read, bytes: %d, nl: %d", bytes, nl);
		if (nl>0)
		{
			QString new_line = d->read_data.left(nl);
			nextLine(new_line);
			d->read_data = "";
			//qDebug("ready Read, after nextLine, read_data: '%s'", d->read_data.toAscii());
		}
	}
	else if (d->wait_for_data)
	{
		int nl = d->read_data.indexOf("\n.\r\n");
		qDebug("ready Read, bytes: %d, nl: %d", bytes, nl);
		if (nl>0)
		{
			QString new_line = d->read_data.left(nl);
			nextLine(new_line);
			d->read_data = "";
			//qDebug("ready Read, after nextLine, read_data: '%s'", d->read_data.toAscii());
		}
	}
}
	
void Pop3::nextLine(const QString& line)
{
	//qDebug()<<"nextLine: "<<line.toAscii();
	
	qDebug()<<d->read_state;
	
	QCoreApplication::processEvents();
	
	//d->responseLine = line;
	
	if (line.left(3) != "+OK")
	{
		qDebug("Pop3 Error");
		//some kind of error
		*(d->t) << "QUIT\r\n";
		emit status( tr( "Unexpected reply from POP3 server:\n\n%1").arg(line) );
		emit (error (tr( "Unexpected reply from POP3 server:\n\n%1").arg(line) ));
		d->state = Close;
	}
	
	if (d->read_state == Stat)
	{
		qDebug()<<"Stat";
		parseStatLine(line);
		if (d->mails_to_read > 0)
		{
			d->state = Retr;
			d->curr_mail = 1;
		}
		else
		{
			d->state = Quit;
		}
	}
    else if (d->read_state == Retr)
	{
		qDebug()<<"Retr";
		QString m=line.mid(line.indexOf("\n")+1);
//qDebug()<<line.indexOf("\n")<<"\n\n"<<m;
		parseMail(m);
		d->curr_mail++;
		if (d->curr_mail > d->mails_to_read)
		{
			if (d->delete_mails)
			{
				d->curr_mail = 1;
				d->state = Dele;
			}
			else
			{
				d->state = Quit;
			}
		}
		else
		{
			d->state = Retr;
		}
			
	}
	else if (d->read_state == Dele)
	{
		qDebug()<<"Dele";
		d->curr_mail++;
		if (d->curr_mail > d->mails_to_read)
		{
			d->state = Quit;
		}
		else
		{
			d->state = Dele;
		}
	}
	
	if (d->state == Init)
	{
		qDebug()<<"user";
		*(d->t) << "USER ";
		*(d->t) << d->user;
		*(d->t) << "\r\n";
		d->state = Pass;
		d->wait_for_data = FALSE;
		d->wait_for_line = TRUE;
		d->t->flush();
	}
	else if (d->state == Pass)
	{
		*(d->t) << "PASS ";
		*(d->t) << d->passwd;
		*(d->t) << "\r\n";
		d->state = Stat;
		d->wait_for_data = FALSE;
		d->wait_for_line = TRUE;
		d->t->flush();
	}
	else if (d->state == Stat)
	{
		*(d->t) << "STAT \r\n";
		
		d->read_state = Stat;
		d->wait_for_data = FALSE;
		d->wait_for_line = TRUE;
		d->state = Wait; //until we know how many mails do we have
		d->t->flush();
	}
	else if (d->state == Retr)
	{
		*(d->t) << "RETR " << d->curr_mail << "\r\n";
		d->read_state = Retr;
		d->wait_for_data = TRUE;
		d->wait_for_line = FALSE;
		emit status( tr( "Receiving mail %1/%2" ).arg(d->curr_mail).arg(d->mails_to_read) );
		d->t->flush();
	}
	else if (d->state == Dele)
	{
		*(d->t) << "DELE " << d->curr_mail << "\r\n";
		// here, we just close.
		d->wait_for_data = FALSE;
		d->wait_for_line = TRUE;
		d->read_state = Dele;
		emit status( tr( "Deleting mail %1/%2" ).arg(d->curr_mail).arg(d->mails_to_read) );
		d->t->flush();
	}
	else if (d->state == Quit)
	{
		*(d->t) << "QUIT\r\n";
		// here, we just close.
		d->state = Close;
		d->wait_for_data = FALSE;
		d->wait_for_line = TRUE;
		d->read_state = None;
		emit status( tr( "All Messages received" ));
		d->t->flush();
	}
	else if (d->state == Close)
	{
		emit disconnected(this);
		//deleteLater();
		return;
	}
	else
	{
		// something broke.
		emit status( tr( "Unexpected reply from POP3 server:\n\n%1").arg(line) );
		emit (error (tr( "Unexpected reply from POP3 server:\n\n%1").arg(line) ));
		d->state = Close;
	}
}

/****************************************************************************/

QString Pop3::getPop3Server () const
{
	return d->pop3_server;
}

/****************************************************************************/

QString Pop3::getUser () const
{
        return d->user;
}

/****************************************************************************/

QString Pop3::getPassword () const
{
        return d->passwd;
}

