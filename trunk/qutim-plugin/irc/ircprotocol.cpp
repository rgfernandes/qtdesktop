/*
    ircProtocol

		Copyright (c) 2008-2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include <qutim/plugininterface.h>
#include "ircprotocol.h"

ircProtocol::ircProtocol(const QString &account_name, const QString &profile_name, QObject *parent)
	: QObject(parent)

{
	m_account_name = account_name;
	m_profile_name = profile_name;

	socket = new QTcpSocket();
	connect(socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
	connect(socket, SIGNAL(connected()), this, SLOT(authorize()));

	cstatus = "offline";
	confCodepage = "UTF-8";

	quint8 ver_major; quint8 ver_minor;	quint8 ver_secminor; quint16 ver_rev;
	qutim_sdk_0_2::SystemsCity::PluginSystem()->getQutimVersion(ver_major, ver_minor, ver_secminor, ver_rev);

	QString ver_timezone; int ver_timezone_offset;
	qutim_sdk_0_2::SystemsCity::PluginSystem()->getSystemInfo(version_system, ver_timezone, ver_timezone_offset);

	version_core = QString::number(ver_major)+"."+QString::number(ver_minor)+"."+QString::number(ver_secminor)+" r"+QString::number(ver_rev);
}

ircProtocol::~ircProtocol() {
	onDisconnect();
}

void ircProtocol::autologin() 
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+m_account_name, "accountsettings");
	if (settings.value("main/autologin").toBool()) {
		doConnect();
	}
}


void ircProtocol::loadSettings()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+m_account_name, "accountsettings");
	confServer = settings.value("main/server").toString();
	confPort = settings.value("main/port").toString();
	confNick = settings.value("main/nick").toString();
	confNick2 = settings.value("main/nick2").toString();
	confPassword = settings.value("main/password").toString();
	confRealName = settings.value("main/realname").toString();
	if (settings.value("main/codepage").isValid()) confCodepage = settings.value("main/codepage").toString();

	confAutoCmds = settings.value("advanced/autocmds").toString().split("\n");
	confMsgPart = settings.value("advanced/msgpart").toString();
	confMsgQuit = settings.value("advanced/msgquit").toString();

	debug = settings.value("main/debug", 0).toInt();

	cnickname = confNick;

	QStringList idnts;
	idnts<<"age"<<"gender"<<"location"<<"languages"<<"avatar"<<"other";
	foreach (QString idnt, idnts) {
		confIdnt[idnt] = settings.value("identify/"+idnt).toString().trimmed();
	}
}

void ircProtocol::doConnect()
{
	if (cstatus=="offline") {
		emit statusChanged("connecting");
		loadSettings();
		emit serverMsg(tr("Connecting to %1").arg(confServer + ":" + confPort));
		socket->connectToHost(confServer, confPort.toInt());
		if (!socket->waitForConnected()) {
			emit statusChanged("offline");
		}
	}
	else if (cstatus=="away") {
		socketWrite("AWAY\n");
	}
}

void ircProtocol::authorize() {
	socketWrite("NICK "+cnickname+"\nUSER "+cnickname+" 8 * : "+confRealName+"\n");
}

void ircProtocol::doAutoCmds() {
	foreach (QString cmd, confAutoCmds) {
		sendCmd(cmd);
	}
}

void ircProtocol::onDisconnect()
{
	if (cstatus!="offline") {
		socketWrite("QUIT :"+confMsgQuit+"\n");
		socket->disconnectFromHost();
	}
	cstatus="offline";
	emit statusChanged(cstatus);
	emit serverMsg("Disconnected");  
}

void ircProtocol::socketRead()
{
	while (socket->canReadLine()) {

		QTextCodec *codec = QTextCodec::codecForName(confCodepage.toAscii());
		
		QByteArray socketline = socket->readLine();
		QString msg = codec->toUnicode(socketline);

		if (debug>0) qDebug()<<"IRC"<<"<"<<msg.trimmed();

		QByteArray outmsg;
		QRegExp rx;

		rx.setPattern("^PING\\s(:\\S+)");
		if (rx.indexIn(msg)==0) {
			outmsg.append("PONG " + rx.cap(1).trimmed() + "\n");
		}

		rx.setPattern("NOTICE\\sAUTH[^:]+:(.+)");
		if (rx.indexIn(msg)>=0) {
			emit serverMsg(rx.cap(1).trimmed(), "Notice");
		}
	
		rx.setPattern("^:\\S+\\s(\\d\\d\\d)\\s(\\S+)\\s(.+)\n");
		if (rx.indexIn(msg)==0) {
			QString type;
			int rc=rx.cap(1).toInt();
			if (cstatus=="offline" && rc>199 && rc<300) {
				setStatus("online");
				doAutoCmds();
			}

			QString smsg = rx.cap(3).trimmed();
			QRegExp rx1;

			if (rc>=372 && rc<=376) {
				if (rc==375) type="MOTDSTART";
				else if (rc==372) type="MOTD";
				else if (rc==376) type="ENDOFMOTD";
				emit serverMsg(smsg, type);
			}
			else if (rc==305) {
				setStatus("online");
				emit serverMsg(smsg, "Reply");
			}
			else if (rc==306) {
				setStatus("away");
				emit serverMsg(smsg, "Reply");
			}
			else if (rc==321) { emit listChannelRow("%START%", "", ""); }
			else if (rc==323) { emit listChannelRow("%END%", "", ""); }
			else if (rc==322) {
				rx1.setPattern("^(\\S+)\\s(\\d+)\\s:(.*)");
				if (rx1.indexIn(smsg)==0) {
					emit listChannelRow(rx1.cap(1), rx1.cap(2), rpl(rx1.cap(3)));
				}
			}
			else if (rc==353 || rc==366) {
				rx1.setPattern("^(.)\\s(\\S+)\\s:(.+)");
				if (rx1.indexIn(smsg)==0) {
					emit channelNickNames(rx1.cap(2), rx1.cap(3).split(QRegExp("\\s+")));
				}
			}
			else if (rc==332) {
				rx1.setPattern("^(\\S+)\\s:(.+)");
				if (rx1.indexIn(smsg)==0) {
					emit channelTopic(rx1.cap(1), "",rpl(rx1.cap(2)));
				}
			}
			else if (rc>430 && rc<437) {
				emit serverMsg(smsg, "error");
				if (cnickname != confNick2) {
					cnickname = confNick2;
					emit serverMsg(tr("Trying alternate nick: %1").arg(cnickname));
					authorize();
				}
				else onDisconnect();
			}
			else if (rc>400 && rc<503) {
				emit serverMsg(smsg, "error");
			}
			else if (rc>199 && rc<400) {
				emit serverMsg(smsg, "Reply");
			}
			else {
				rx1.setPattern("^[:\\d]");
				if (rx1.indexIn(smsg)>=0) emit serverMsg(smsg.replace(":", ""), type);
			}
		}

		rx.setPattern("^:([^\\!]+)\\!(\\S+)\\s([A-Z]+)\\s([^:\\s$]*):?(.*)");
		if (rx.indexIn(msg)==0) {
			QString innick = rx.cap(1);
			QString inreal = rx.cap(2);
			QString incmd = rx.cap(3);
			QString inarg1 = rx.cap(4).trimmed();
			QString inarg2 = rx.cap(5).trimmed();
			nicklongnames[innick] = inreal;
			QRegExp rxinarg2("^:(.+)");
			if (rxinarg2.indexIn(inarg2)>=0) inarg2=rxinarg2.cap(1);

//			qDebug()<<"IRC"<<"srx"<<innick<<inreal<<incmd<<inarg1<<inarg2;

			if (incmd=="JOIN") {
				QString channel;
				channel = inarg1.trimmed()+inarg2.replace(":", "").trimmed();
				if (channel.length()>0) emit joinedChannel(channel, innick);
			}
			else if (incmd=="PART") {
				emit leavedChannel(inarg1, innick, inarg2);
			}
			else if (incmd=="QUIT") {
				emit nickQuit(innick, inarg2);
			}
			else if (incmd=="PRIVMSG") {
					QString privmsg = inarg2;
					QRegExp rx2("^\\001([A-Z]+)(.*)\\001");
					if (rx2.indexIn(privmsg)==0) {
						QString ctcpcmd=rx2.cap(1);
						QString ctcpargs=rx2.cap(2);
						if (ctcpcmd=="ACTION") privmsg = "/me "+ctcpargs;
						else if (ctcpcmd=="CLIENTINFO") {
							emit serverMsg(innick+" requests "+ctcpcmd, "Notice");
							privmsg.clear();
							sendNotice(innick, "\001CLIENTINFO qutIM 0.2 IRC plugin - http://qutim.org - Supported tags: ACTION,AVATAR,CLIENTINFO,PING,USERINFO,VERSION\001");
						}
						else if (ctcpcmd=="PING") {
							emit serverMsg(tr("%1 requests %2").arg(innick).arg(ctcpcmd), "Notice");
							privmsg.clear();
							sendNotice(innick, "\001PING"+ctcpargs+"\001");
						}
						else if (ctcpcmd=="VERSION") {
							emit serverMsg(tr("%1 requests %2").arg(innick).arg(ctcpcmd), "Notice");
							privmsg.clear();
							sendNotice(innick, "\001VERSION qutIM "+version_core+", IRC plugin alpha / "+version_system+"\001");
						}
						else if (ctcpcmd=="AVATAR") {
							emit serverMsg(tr("%1 requests %2").arg(innick).arg(ctcpcmd), "Notice");
							privmsg.clear();
							if (!confIdnt["avatar"].isEmpty()) {
								sendNotice(innick, "\001AVATAR "+confIdnt["avatar"]+" ?\001");
							}
						}
						else if (ctcpcmd=="USERINFO") {
							emit serverMsg(tr("%1 requests %2").arg(innick).arg(ctcpcmd), "Notice");
							privmsg.clear();
							QStringList idnts; QStringList infos;
							idnts<<"Age"<<"Gender"<<"Location"<<"Languages";
							foreach (QString idnt, idnts) {
								QString idntl = idnt.toLower();
								if (!confIdnt[idntl].isEmpty()) infos.append(idnt+"="+confIdnt[idntl]);
							}
							if (!confIdnt["other"].isEmpty()) infos.append(confIdnt["other"]);
							sendNotice(innick, "\001USERINFO "+infos.join("; ")+"\001");
						}
						else {
							emit serverMsg(tr("%1 requests unknown command %2").arg(innick).arg(ctcpcmd));
							privmsg.clear();
						}
					}
					if (!privmsg.isEmpty()) {
						if (privmsg.length()>0 && inarg1==cnickname) {
							emit privateMsg(innick, rpl(privmsg));
						}
						else emit channelMsg(inarg1, innick, rpl(privmsg));
					}
			}
			else if (incmd=="TOPIC") {
				emit channelTopic(inarg1, innick, rpl(inarg2));
			}
			else if (incmd=="NOTICE") {
					QString privmsg = inarg2;
					QRegExp rxctcp("\\001([A-Z]+)\\s(.+)\\001");
					if (rxctcp.indexIn(privmsg)>=0) {
						QString descr = rxctcp.cap(2);
						QString cmd = rxctcp.cap(1);
						if (cmd=="PING") {
							descr = QString::number(pingtime->elapsed()) + " msec";
							delete pingtime;
						}
						privmsg = tr("%1 reply from %2: %3").arg("CTCP "+cmd).arg(innick).arg(descr);
						if (cmd=="AVATAR") {
							rxctcp.setPattern("(\\S+)");
							if (rxctcp.indexIn(descr)==0) {
								emit nickAvatar(innick, rxctcp.cap(1));
							}
						}
					}
					reqReply(innick, privmsg);
			}
			else if (incmd=="MODE") {
				QRegExp rx1("^([\\+\\-][A-Za-z\\+\\-]+)\\s(.*)");
				if (rx1.indexIn(inarg2)==0) {
					emit channelSystemMsg(inarg1, tr("%1 has set mode %2").arg(innick).arg(inarg2));
					QString mq;
					QString modes = rx1.cap(1);
					QStringList nicknames = rx1.cap(2).trimmed().split(QRegExp("\\s+"));
					int ni=0;
					for (int i=0; i<modes.length(); i++) {
						if (modes[i]=='+' || modes[i]=='-') {
							mq=modes[i]; ni=0;
						}
						else {
							emit channelNickModeChanged(inarg1, nicknames[ni], mq + modes[i]);
							if ((ni+1)<nicknames.count()) ni++;
						}
					}
				}
			}
			else if (incmd=="NICK") {
				nicklongnames[inarg2] = nicklongnames[innick];
				nicklongnames[innick].clear();
				emit nickRename(innick, inarg2);
			}
			else if (incmd=="KICK") {
				QRegExp rx1("^(\\S+)\\s:(.*)");
				if (rx1.indexIn(inarg2)==0) {
					emit kickedFromChannel(inarg1, rx1.cap(1), innick, rx1.cap(2));
				}
			}
		}
		
		if (outmsg.length()>0) {
			socketWrite(outmsg);
		}
	
	}
}

void ircProtocol::socketWrite(QString line) {
	QTextCodec *codec = QTextCodec::codecForName(confCodepage.toLatin1());
	QByteArray socketline = codec->fromUnicode(line);
	if (debug>0) qDebug()<<"IRC"<<">"<<socketline.trimmed();
	socket->write(socketline);
}

QString ircProtocol::rpl(QString str) {
/// "normal \002bold \037underlined \026italic \017normal";
	QRegExp rx("(\003\\d\\d?,?\\d?\\d?)[^\003$]+");
	int pos=-1;
	while ((pos=rx.indexIn(str))!=-1) {
		str = str.replace(rx.cap(1), "");
	}
	rx.setPattern("([\002\037\026\017])");
	while ((pos=rx.indexIn(str))!=-1) {
		str = str.replace(rx.cap(1), "");
	}
	return str;
	
	
}

void ircProtocol::setStatus(QString st) {
	cstatus = st;
	emit statusChanged(cstatus);	
}

void ircProtocol::joinChannel(QString channel) {
	QRegExp rx("^\\w");
	if (rx.indexIn(channel)==0) channel = "#"+channel;
	if (cstatus != "offline") {
		socketWrite("JOIN "+channel+"\n");
	}
}

void ircProtocol::leaveChannel(QString channel) {
	if (cstatus != "offline") {
		socketWrite("PART "+channel+" :"+confMsgPart+"\n");
	}
}

void ircProtocol::sendCmd(QString line) {
	QRegExp rx("^/([a-zA-Z]+)(\\s*.*)");
	if (rx.indexIn(line)==0) {
		QString cmd = rx.cap(1);
		QString args = rx.cap(2);
		if (cmd=="msg" && !args.isEmpty()) {
			QRegExp rxmsg("(\\S+)\\s+(.+)");
			if (rxmsg.indexIn(args)>-1) {
				QString to = rxmsg.cap(1);
				QString msg = rxmsg.cap(2);
				QRegExp rxto("^\\#");
				if (rxto.indexIn(to)==0) emit channelMsg(to, cnickname, msg);
				socketWrite("PRIVMSG "+rxmsg.cap(1)+" :"+rxmsg.cap(2)+"\n");
			}
		}
		else if (cmd=="join" && !args.isEmpty()) {
			QRegExp rxjoin("(\\S+)");
			if (rxjoin.indexIn(args)>-1) {
				joinChannel(rxjoin.cap(1));
			}
		}
		else if (cmd=="ctcp" && !args.isEmpty()) {
			QRegExp rxctcp("(\\S+)\\s+(\\w+)");
			if (rxctcp.indexIn(args)>-1) {
				socketWrite("PRIVMSG "+rxctcp.cap(1)+" :\001"+rxctcp.cap(2)+"\001\n");
			}
		}
		else {
			socketWrite(rx.cap(1)+rx.cap(2)+"\n");
		}
	}
	else {
		socketWrite(line+"\n");
	}
}

void ircProtocol::sendMsg(QString to, QString msg) {
	foreach (QString line, msg.split("\n")) {
		QRegExp rxmsg("^/(.+)");
		if (rxmsg.indexIn(line)==0) {
			QRegExp rxcmd("^/me\\s+(.+)");
			if (rxcmd.indexIn(line)==0) {
				socketWrite("PRIVMSG "+to+" :\001ACTION "+rxcmd.cap(1)+"\001\n");
				emit channelMsg(to, cnickname, "/me "+rxcmd.cap(1));
			}
			else {
				sendCmd(line);
			}
		}
		else {
			socketWrite("PRIVMSG "+to+" :"+line+"\n");
			if (to.startsWith("#")) {
				emit channelMsg(to, cnickname, line);
			}
		}
	}
}

void ircProtocol::sendNotice(QString to, QString msg) {
	foreach (QString line, msg.split("\n")) {
		socketWrite("NOTICE "+to+" :"+line+"\n");
	}
}

void ircProtocol::reqReply(QString innick, QString msg) {
	if (innick==req_nickname) {
		channelSystemMsg(req_channel, msg);
		req_nickname.clear();
		req_channel.clear();
	}
	emit serverMsg(msg, "Notice");
}

void ircProtocol::actionTriggered(QVariant action_data) {
	QString send_to = req_nickname;
	QRegExp rx("channel_(.+)");
	if (rx.indexIn(action_data.toString())==0) {
		action_data = rx.cap(1);
		send_to = req_channel;
	}

	if (action_data=="notifyavatar") {
		sendNotice(send_to, "\001AVATAR "+confIdnt["avatar"]+" ?\001");
	}
	else if (action_data=="info_whois") {
		socketWrite("WHOIS "+send_to+"\n");
	}
	else if (action_data=="info_whowas") {
		socketWrite("WHOWAS "+send_to+"\n");
	}
	else if (action_data=="info_who") {
		socketWrite("WHO "+send_to+"\n");
	}
	else if (action_data=="ctcp_avatar") {
		socketWrite("PRIVMSG "+send_to+" :\001AVATAR\001\n");
	}
	else if (action_data=="ctcp_ping") {
		QDateTime dt(QDateTime::currentDateTime());
		socketWrite("PRIVMSG "+send_to+" :\001PING "+dt.toString("yyyyMMddhhmmsszzz")+"\001\n");
		pingtime = new QTime();
		pingtime->start();
	}
	else if (action_data=="ctcp_version") {
		socketWrite("PRIVMSG "+send_to+" :\001VERSION\001\n");
	}
	else if (action_data=="ctcp_clientinfo") {
		socketWrite("PRIVMSG "+send_to+" :\001CLIENTINFO\001\n");
	}
	else if (action_data=="ctcp_userinfo") {
		socketWrite("PRIVMSG "+send_to+" :\001USERINFO\001\n");
	}
	else if (action_data=="modes_giveop") {
		socketWrite("MODE "+req_channel+" +o "+req_nickname+"\n");
	}
	else if (action_data=="modes_takeop") {
		socketWrite("MODE "+req_channel+" -o "+req_nickname+"\n");
	}
	else if (action_data=="modes_givehalfop") {
		socketWrite("MODE "+req_channel+" +h "+req_nickname+"\n");
	}
	else if (action_data=="modes_takehalfop") {
		socketWrite("MODE "+req_channel+" -h "+req_nickname+"\n");
	}
	else if (action_data=="modes_givevoice") {
		socketWrite("MODE "+req_channel+" +v "+req_nickname+"\n");
	}
	else if (action_data=="modes_takevoice") {
		socketWrite("MODE "+req_channel+" -v "+req_nickname+"\n");
	}
	else if (action_data=="kb_kick") {
		if (req_text.length()==0) req_text = req_nickname;
		socketWrite("KICK "+req_channel+" "+req_nickname+" :"+req_text+"\n");
	}
	else if (action_data=="kb_ban1") {
		socketWrite("MODE "+req_channel+" +b *!"+nicklongnames[req_nickname]+"\n");
	}
	else if (action_data=="kb_unban1") {
		socketWrite("MODE "+req_channel+" -b *!"+nicklongnames[req_nickname]+"\n");
	}
}


void ircProtocol::requestChannelsList() {
	socketWrite("LIST\n");
}
