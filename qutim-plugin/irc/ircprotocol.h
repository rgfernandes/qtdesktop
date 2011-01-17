/*
    ircProtocol

		Copyright (c) 2008-2009 by Alexander Kazarin <boiler@co.ru>s

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef IRCPROTOCOL_H
#define IRCPROTOCOL_H
//
#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QSettings>
#include <QRegExp>
#include <QStringList>
#include <QTextCodec>
#include <QDateTime>
#include <QTime>

//
//#include "ircaccount.h"


class ircProtocol : public QObject
{
	Q_OBJECT
public:

	ircProtocol(const QString &, const QString &profile_name, QObject *parent = 0);
	~ircProtocol();
	
	QString getNick() { return cnickname; }
  void setNick(QString new_nickname) { cnickname = new_nickname; }
	void setReqNickname(QString nickname) { req_nickname = nickname; }
	void setReqChannel(QString channel) { req_channel = channel; }
	void setReqText(QString text) { req_text = text; }

	void loadSettings();
	void doConnect();

	void leaveChannel(QString);
	void sendMsg(QString, QString);
	void sendNotice(QString, QString);
	void onDisconnect();
	void autologin();

private:
	QString m_account_name;
	QString m_profile_name;
	QString version_core;
	QString version_system;

	QTcpSocket *socket;
	int debug;

	QString confServer;
	QString confPort;
	QString confNick;
	QString confNick2;
	QString confPassword;
	QString confRealName;
	QString confCodepage;
	QStringList confAutoCmds;
	QString confMsgPart;
	QString confMsgQuit;
	QHash<QString, QString> confIdnt;

	QString lastmsg;
	QString cstatus;
	QString cnickname;
	void setStatus(QString);
  QString rpl(QString);
  QString req_channel;
	QString req_nickname;
	QString req_text;
	QHash<QString, QString> nicklongnames;
	void reqReply(QString, QString);
	void doAutoCmds();
	QTime *pingtime;
	
//	QTextCodec *codec;

signals:
	void serverMsg(QString, QString=QString());
	void statusChanged(QString);
	void joinedChannel(QString, QString);
	void leavedChannel(QString, QString, QString);
	void kickedFromChannel(QString, QString, QString, QString);
	void nickAvatar(QString, QString);
	void nickRename(QString, QString);
	void nickQuit(QString, QString);
	void channelNickModeChanged(QString, QString, QString);
	void channelNickNames(QString, QStringList);
	void channelMsg(QString, QString, QString);
	void channelTopic(QString, QString, QString);
	void channelNickRoleChanged(QString, QString, QString);
	void channelSystemMsg(QString, QString);
	void privateMsg(QString, QString);
	void serviceMsg(QString, QString);
	void createChat(QString);
	void listChannelRow(QString, QString, QString);

public slots:
	void joinChannel(QString);
	void sendCmd(QString);
	void socketWrite(QString);
	void actionTriggered(QVariant);

private slots:
	void authorize();
	void socketRead();

	void requestChannelsList();

};
#endif
