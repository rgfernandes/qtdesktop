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

#include "smtp.h"

void Smtp::exitLoop()
{
	//qDebug()<<"exit loop";
	mutex.lock();
	running=false;
	mutex.unlock();
}

void Smtp::run()
{
	running=true;
	qDebug()<<"run";
	
	smtpsocket = new QTcpSocket();
	t = new QTextStream( smtpsocket );
	connect(smtpsocket,SIGNAL(disconnected()),this,SLOT(exitLoop()));

	int waittime = 50 * 1000;
	readyToSend=false;
	
	ErrorMSG.clear();
	
	Timeout = waittime;
	
	//qDebug() << "Config server smtp connect to......  "  << smtphost;
	
//qDebug()<<smtphost<<smtpusername<<smtppass;
	
	connect( this, SIGNAL(ConnectorSuccess()), this ,SLOT(ReadLiner()));
	connect( this, SIGNAL(sendNextLine()), this ,SLOT(nextLine()));
	
	smtpsocket->connectToHost(smtphost,25);
	if (smtpsocket->waitForConnected(Timeout)) {
		qDebug() <<"connected on  " << smtphost;
		QCoreApplication::processEvents();
		if (smtpsocket->waitForReadyRead(Timeout)) {
			qDebug() <<"emit from waitForReadyRead connect go can read";
			emit ConnectorSuccess();
			emit status(tr("Connected to host"));
		}
	} else {
		qDebug()<<"error"<<smtpsocket->errorString()<<smtphost;
		emit ErrorCloseAll();
	}
	connect( smtpsocket,SIGNAL(readyRead()),this,SLOT(grabLine()));
	
	while(running){
		QCoreApplication::processEvents();
		if(readyToSend && queuedMails.size()>0){
			mutex.lock();
			Mail *m=queuedMails.takeFirst();
			mutex.unlock();
			sendMail(m,quitAfterSending);
		}
//qDebug()<<quitAfterSending << sentMail << queuedMails.size();
		if(quitAfterSending && sentMail && queuedMails.size()<=0) disconnectSmtp();
	}
	qDebug()<<"end";
	quit();
	delete t;
	t=0;
	delete smtpsocket;
	smtpsocket=0;
}

void Smtp::queueMail(Mail *m, bool qas)
{
	qDebug()<<"qas"<<qas<<running;
	if(readyToSend && queuedMails.size()==0){
		mutex.lock();
		quitAfterSending=qas;
		mutex.unlock();
		sendMail(m,qas);
		return;
	}
	
	mutex.lock();
	quitAfterSending=qas;
	queuedMails.append(m);
//qDebug()<<"appended queued mail";
	mutex.unlock();
	//if(!running) start();
	//mutex.unlock();
}

void Smtp::sendMail(Mail *m, bool qas)
{
	//qDebug()<<readyToSend;//"qas"<<qas;
	if(!readyToSend){
		qDebug()<<"not ready: queue mail";
		queueMail(m,qas);
		return;
	}
	mutex.lock();
	quitAfterSending=qas;
	qDebug()<<qas;
	readyToSend=false;
	newMail=true;
	
	emit status(tr("Send mail"));
	
	from = m->from();
	if(from.contains("<")){
		from=from.mid(from.indexOf("<")+1);
		from=from.left(from.indexOf(">"));
	}

	if(m->bcc()!="") bcc=m->bcc().split(QRegExp(",|;"));
	if(m->cc()!="") bcc<<m->cc().split(QRegExp(",|;"));

	//m->emptyBcc();
	rcpt=m->to();
	message=m->getSendData();
	
	read_state=Rcpt;
	mutex.unlock();
	
	sendLine(QString("MAIL FROM: %1\r\n").arg(from));
	if(preserveMails) currentMail=m;
	else delete m;
}

void Smtp::disconnectSmtp()
{
	//disconnect(this, SIGNAL(ConnectorSuccess()), this ,SLOT(ReadLiner()));
	//disconnect(this, SIGNAL(sendNextLine()), this ,SLOT(PutsendNextLine()));  
	read_state=Close;
	nextLine();
}

void Smtp::ReadLiner()
{
        //qDebug() << "### socketType = " << smtpsocket->socketType();
        //qDebug() << "### ReadLiner is start by textstream ";
        
        int loops = 0;
        while (!t->atEnd()){
        loops++;
        response = t->readLine();
        //qDebug() << loops << " in line  " << response;
        }
        if (response.size() > 0) {
        RemoteServerName = response;
        mailstatus = response.left(3);
        if (mailstatus == "220") {
        	qDebug()<<"connected with status 220";
                response="";
                read_state = Connect;
                emit sendNextLine();
                }
        } else {
           emit ErrorCloseAll();
        }
}

Smtp::~Smtp()

{
	delete smtpsocket;
	delete t;
}

/* LINE SENDER  */
void Smtp::nextLine()
{
	
	State current = read_state;
	//qDebug() <<"### Go and Send line " << read_state;
	switch(current) {
	case Connect:
		qDebug()<<"send HELO";
		read_state=Connected;
		response="";
		sendLine("HELO there\r\n");
	break;
	case Connected:
		if (response.size() > 0) {
		    ErrorMSG.append(response);
		    qDebug() << "1---- " << response;
		     read_state=Auth;
		     response ="";
		     sendLine("AUTH LOGIN\r\n");
		} else {
		     qDebug() << "Connection lost1";
		     response ="";
		     emit ErrorCloseAll();
		}
	break;
	case Auth:
		if (response.size() > 0) {
			ErrorMSG.append(response);
			qDebug() << "2---- " << response;
			read_state=Pass;
			sendLine(encodeBase64(smtpusername)+"\r\n");   //send username
		} else {
			qDebug() << "Connection lost2";
			emit ErrorCloseAll();
		}
		response ="";
	break;
	case Pass:
		if (response.size() > 0) {
		ErrorMSG.append(response);
			qDebug() << "3---- " << response;
			read_state=AuthOk;
		 	sendLine(encodeBase64(smtppass)+"\r\n"); //send password
		} else {
		     qDebug() << "Connection lost3";
		     emit ErrorCloseAll();
		}
		response ="";
	break;
	case AuthOk:
		qDebug() << "4---- " << response;
		if (response.size() > 0) {
			ErrorMSG.append(response);
			if (response.contains("successful", Qt::CaseInsensitive)||response.contains("ok", Qt::CaseInsensitive)){
				response="";
				read_state=Noop;
				emit status(tr("Login successful"));
				emit sendNextLine();                   		
			}else{
				qDebug() << "Connection lost4.1";
				emit ErrorCloseAll();               			
			}
		} else {
			qDebug() << "Connection lost4.2";
			emit ErrorCloseAll();
		}
		response ="";
      break;
      case Rcpt:
          //if (!newMail) break;
           qDebug() << "5---- " << response;
           if (response.size() > 0) {
                read_state=Data;
                sendLine("RCPT TO: "+rcpt+"\r\n");
           } else {
                qDebug() << "Connection lost5";
                emit ErrorCloseAll();
           }
	break;
	case Data:
		qDebug() << "6---- " << response;
		if (response.size() > 0){
			if(bcc.size()>0){
				read_state=Data;
				sendLine("RCPT TO: "+bcc.takeFirst()+"\r\n");
			}else{
				ErrorMSG.append(response);
				read_state=Send;
				sendLine("DATA\r\n");
			}
		} else {
			qDebug() << "Connection lost6";
			emit ErrorCloseAll();
		}
		//response ="";
	break;
	case Send:
        qDebug() << "7---- " << response;
	       if (!response.contains("not", Qt::CaseInsensitive)) {
	        ErrorMSG.append(response);
	        response ="";             
	        read_state = Sent;
	       	sendLine(message+"\r\n.\r\n");
	       } else {
	           qDebug() << "Connection lost7";
	           emit ErrorCloseAll();
	       }
	break;
	case Sent:
		qDebug() << "8---- " << response;
		if(response.left(3)=="354"){
			
		}else{
			if (response.size() && (response.contains("ok", Qt::CaseInsensitive)||response.contains("accepted", Qt::CaseInsensitive))) {
				ErrorMSG.append(response);
				read_state = Noop;
				qDebug() << "Mail to " << rcpt << " successfully sent";
				response="";
				if(preserveMails) emit mailSent(currentMail);
				emit status(tr("Mail successfully sent"));
				mutex.lock();
				mailsSent++;
				sentMail=true;
				mutex.unlock();
				sendLine("NOOP\r\n");
			} else {
				qDebug() << "Connection lost8";
				emit ErrorCloseAll();
			}
			response ="";
		}
	break;
	case Noop:
		if(response=="") sendLine("NOOP\r\n");
		else{
			qDebug() << "9---- "<<response;
			//QCoreApplication::processEvents();
//qDebug()<<"queuedMails.size"<<queuedMails.size();
qDebug()<<mailsSent<<queuedMails.size()<<quitAfterSending;
			if(queuedMails.size()>0){
				mutex.lock();
				readyToSend=true;
				Mail *m=queuedMails.takeFirst();
				mutex.unlock();
				sendMail(m,quitAfterSending);
			} else {
				mutex.lock();
				readyToSend=true;
				mutex.unlock();
				emit ReadyToSend();
			}
			//qDebug() << "Signal ReadyToSend emitted";
			response ="";
		}
	break;
	case Close:
		read_state=Quit;
		emit status(tr("Disconnect Smtp"));
		sendLine("QUIT\r\n");
	break;
	case Quit:
		qDebug() << "10---- " << response;
		ErrorMSG.append(response);
		smtpsocket->disconnectFromHost();
		//qDebug()<<"disconnectFromHost";
		emit status(tr("Disconnected"));
		emit SuccessQuit();
		response ="";
		if(quitAfterSending) exitLoop();
	break;
	default:
		qDebug() << "ERROR!  last loop or false line emit...";
		emit ErrorCloseAll();
	break;
	}   
}

/* SENDER AND RECIVER  */
void Smtp::sendLine(QString senddata)
{
	*t << senddata.toAscii();
	t->flush();
//qDebug()<<"line sent";
	return;
}

void Smtp::grabLine()
{
//qDebug()<<"grabLine";
	int loops = 0;
	QString incommingData = "";
	while (!t->atEnd()) {
		QCoreApplication::processEvents();
		loops++;
		QString opera = t->readLine();
		if(loops==1)incommingData.append(opera);
		else incommingData.append("\n"+opera);
		//qDebug() << loops << "|#" << opera << "#|";
		response=incommingData;
	}
	emit sendNextLine();
}
 
/*
void Smtp::disconnected()
{
    qDebug() <<"disconneted";
    qDebug() << "error "  << smtpsocket->errorString();
}

void Smtp::connected()
{
    output.append("connected");
    qDebug() << "Connected ";
} */

QString Smtp::encodeBase64( QString xml )
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

QString Smtp::decodeBase64( QString xml )
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}

QString Smtp::TimeStampMail()
{
	/* Date: Mon, 08 May 2006 17:57:52 +0200 */
	/* Date: Sun, 28 May 2006 06:32:25 -0420 */
	QLocale english(QLocale::English,QLocale::UnitedKingdom);
	
	QDateTime dt = QDateTime::currentDateTime();
	QDate timecute = QDate::currentDate();
	QString day_en = english.dayName(timecute.dayOfWeek(),QLocale::ShortFormat);
	QString month_en = english.monthName(timecute.month(),QLocale::ShortFormat);
	QString last = dt.toString("yyyy hh:mm:ss");
	QString maildate = QString( "Date: %1, %2 %3 %4 +0200" ).arg( day_en , QString::number(timecute.day()), month_en ,   last );
	qDebug() << maildate;
	return maildate;
}
