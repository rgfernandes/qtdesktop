/*
	loginform.h

	Copyright (c) 2008 by zodiac <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QtGui/QWidget>
#include <QThread>
#include <QMovie>
#include <QMessageBox>
#include "ui_loginform.h"
#include "utils.h"

#include <gloox/client.h>
#include <gloox/jid.h>
#include <gloox/connectionlistener.h>
#include <gloox/registrationhandler.h>

#include "jConnection.h"
#include "jTransport.h"
#include "jPluginSystem.h"

using namespace gloox;

class LoginForm;

class ClientThread : public QThread, ConnectionListener, LogHandler
{
public:
	ClientThread(LoginForm *login, const QString &server, const QString &username, const QString &password);
	~ClientThread();

	void onConnect();
	void disconnectFromServer() { r_connection->cleanup(); r_client->disconnect(); }

	inline void onDisconnect(ConnectionError e){}
	inline void onResourceBind(const std::string &resource){qDebug()<< "onResourceBind";}
	inline void onResourceBindError(const Error *error){qDebug() <<"onResourceBindError";}
	inline void onSessionCreateError(const Error *error){qDebug() <<"onSessionCreateError";}
	inline bool onTLSConnect(const CertInfo &info){return true;}
	inline void onStreamEvent(StreamEvent event){}
	void handleLog (LogLevel, LogArea area, const std::string&message);

private:
	LoginForm *m_login_form;
	Client *r_client;
	jConnection *r_connection;
	Registration *r_manager;
	QString m_username, m_password;

};

class LoginForm : public QWidget, public RegistrationHandler
{
Q_OBJECT

public:
	LoginForm(QWidget *parent = 0);
	~LoginForm();
	QString getJid() const {return utils::fromStd(JID(utils::toStd(ui.jidEdit->text())).bare());}
	QString getPass() const {return ui.passowrdEdit->text();}
	void handleRegistrationFields(const JID &from, int fields,std::string instructions){}
	void handleAlreadyRegistered(const JID &from){}
	void handleRegistrationResult(const JID &from,RegistrationResult regResult);
	void handleDataForm(const JID &from, const DataForm &form){}
	void handleOOB(const JID &from, const OOB &oob){}

private slots:
	void btnRegisterClick();
	void doCleanup();

private:
	Ui::LoginFormClass ui;
	ClientThread *cltrd;
	QMovie *movie;

};

#endif
