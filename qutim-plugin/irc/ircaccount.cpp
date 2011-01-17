/*
    ircAccount

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

#include "ircaccount.h"

#include "joinchannel.h"
#include "listchannel.h"
#include "textdialog.h"

ircAccount::ircAccount(const QString &account_name, const QString &profile_name, QObject *parent) : QObject(parent), m_irc_plugin_system(ircPluginSystem::instance())
{
	m_account_name = account_name;
	m_profile_name = profile_name;
	m_status_icon = getIcon("offline");

	modenames['q'] = 	tr("Channel owner");
	modenames['a'] = 	tr("Channel administrator");
	modenames['o'] = 	tr("Channel operator");
	modenames['h'] = 	tr("Channel half-operator");
	modenames['v'] = 	tr("Voice");
	modenames['b'] = 	tr("Banned");

	createStatusMenu();

	consoleWidget = new ircConsole();
	connect(this, SIGNAL(serverMsg(QString, QString)), consoleWidget, SLOT(addMessage(QString, QString)));

	ircpr = new ircProtocol(m_account_name, m_profile_name, this);
	connect(ircpr, SIGNAL(serverMsg(QString, QString)), consoleWidget, SLOT(addMessage(QString, QString)));
	connect(ircpr, SIGNAL(statusChanged(QString)), this, SLOT(statusChanged(QString)));
	connect(ircpr, SIGNAL(joinedChannel(QString, QString)), this, SLOT(joinedChannel(QString, QString)));
	connect(ircpr, SIGNAL(leavedChannel(QString, QString, QString)), this, SLOT(leavedChannel(QString, QString, QString)));
	connect(ircpr, SIGNAL(kickedFromChannel(QString, QString, QString, QString)), this, SLOT(kickedFromChannel(QString, QString, QString, QString)));
	connect(ircpr, SIGNAL(nickAvatar(QString, QString)), this, SLOT(nickAvatar(QString, QString)));
	connect(ircpr, SIGNAL(nickRename(QString, QString)), this, SLOT(nickRename(QString, QString)));
	connect(ircpr, SIGNAL(nickQuit(QString, QString)), this, SLOT(nickQuit(QString, QString)));
	connect(ircpr, SIGNAL(channelNickNames(QString, QStringList)), this, SLOT(channelAddNickNames(QString, QStringList)));
	connect(ircpr, SIGNAL(channelMsg(QString, QString, QString)), this, SLOT(channelMsg(QString, QString, QString)));
	connect(ircpr, SIGNAL(channelSystemMsg(QString, QString)), this, SLOT(channelSystemMsg(QString, QString)));
	connect(ircpr, SIGNAL(channelTopic(QString, QString, QString)), this, SLOT(channelTopic(QString, QString, QString)));
	connect(ircpr, SIGNAL(channelNickModeChanged(QString, QString, QString)), this, SLOT(channelNickModeChange(QString, QString, QString)));
	connect(ircpr, SIGNAL(privateMsg(QString, QString)), this, SLOT(privateMsg(QString, QString)));
	connect(ircpr, SIGNAL(serviceMsg(QString, QString)), this, SLOT(serviceMsg(QString, QString)));
	connect(ircpr, SIGNAL(createChat(QString)), this, SLOT(createChat(QString)));
	connect(consoleWidget, SIGNAL(cmdEntered(QString)), ircpr, SLOT(sendCmd(QString)));
//	ircpr->autologin();

	ircavatar = new ircAvatar(m_account_name, m_profile_name, this);
	connect(ircavatar, SIGNAL(avatarDownloaded(const QString &, const QByteArray &)), this , SLOT(nickAvatarArrived(const QString&,const QByteArray &)));
}

ircAccount::~ircAccount()
{
	delete m_account_button;
	delete m_status_menu;
}

void ircAccount::createStatusMenu()
{
	onlineAction = new QAction(getIcon("online"), tr("Online"), this);
	connect(onlineAction, SIGNAL(triggered()), this, SLOT(onlineActionTriggered()));

	offlineAction = new QAction(getIcon("offline"), tr("Offline"), this);
	connect(offlineAction, SIGNAL(triggered()), this, SLOT(offlineActionTriggered()));

	awayAction = new QAction(getIcon("away"), tr("Away"), this);
	connect(awayAction, SIGNAL(triggered()), this, SLOT(awayActionTriggered()));

	consoleAction = new QAction(tr("Console"), this);
	connect(consoleAction, SIGNAL(triggered()), this, SLOT(consoleActionTriggered()));

	channelslistAction = new QAction(tr("Channels List"), this);
	connect(channelslistAction, SIGNAL(triggered()), this, SLOT(channelslistActionTriggered()));
	channelslistAction->setEnabled(false);

	joinconfAction = new QAction(getIcon("chat"), tr("Join Channel"), this);
	connect(joinconfAction, SIGNAL(triggered()), this, SLOT(joinconfActionTriggered()));
	joinconfAction->setEnabled(false);

	m_status_menu = new QMenu();
	m_status_menu->setTitle(m_account_name);
	m_status_menu->setIcon(m_status_icon);

	m_status_menu->addAction(onlineAction);
	m_status_menu->addAction(awayAction);
	m_status_menu->addSeparator();
	m_status_menu->addAction(consoleAction);
	m_status_menu->addAction(channelslistAction);
	m_status_menu->addAction(joinconfAction);
	m_status_menu->addSeparator();
	m_status_menu->addAction(offlineAction);

}


void ircAccount::createAccountButton(QHBoxLayout *account_button_layout)
{
	m_account_button = new QToolButton();
	m_account_button->setMinimumSize(QSize(22, 22));
	m_account_button->setMaximumSize(QSize(22, 22));
	m_account_button->setAutoRaise(true);
#if defined(Q_OS_MAC)
	account_button_layout->addWidget(m_account_button, 0, Qt::AlignLeft);
#else
	account_button_layout->addWidget(m_account_button, 0, Qt::AlignRight);
#endif
	m_account_button->setToolTip(m_account_name);
	m_account_button->setIcon(m_status_icon);
	m_account_button->setPopupMode(QToolButton::InstantPopup);
	m_account_button->setMenu(m_status_menu);

}

QMenu *ircAccount::getAccountMenu()
{
	return m_status_menu;
}

QStringList ircAccount::getAdditionalInfoAboutContact(const QString &item_name)
{
	QStringList adinfo;
	if (item_name==m_account_name) {
		adinfo.append(ircpr->getNick());
	}
	else {
		adinfo.append(item_name);
	}
	return adinfo;
}

void ircAccount::removeMenuAccount(QMenu *account_menu)
{
	delete m_account_button;
}

void ircAccount::onlineActionTriggered()
{
	ircpr->doConnect();
}

void ircAccount::awayActionTriggered()
{
	QString sName = "away-";
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+m_account_name, "accountsettings");
	QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+m_account_name, "accountsettings");

	QString status_message;
	if (!settings.value("autoreply/" + sName + "dshow", false).toBool()) {
		status_message = account_settings.value("autoreply/" + sName + "msg", "").toString();

		ircPluginSystem &ips = ircPluginSystem::instance();
		bool dialog_not_show = false;

		if (ips.setStatusMessage(status_message, dialog_not_show)) {
			settings.setValue("autoreply/" + sName + "dshow", dialog_not_show);
			if (dialog_not_show) {
				settings.setValue("autoreply/" + sName + "msg", status_message.left(1000));
			}
			account_settings.setValue("autoreply/" + sName + "msg", status_message.left(1000));
		}
		else settings.setValue("autoreply/" + sName + "msg", status_message.left(1000));
	}

	status_message = settings.value("autoreply/" + sName + "msg").toString();

	if (status_message.length()>0) {
		ircpr->doConnect();
		ircpr->sendMsg("", "/AWAY :"+status_message+"\n");
	}
}

void ircAccount::offlineActionTriggered() {
	ircpr->onDisconnect();
	foreach (QString channel, m_channel_list) {
		channelSystemMsg(channel, "Disconnected");
		foreach (QString nickname, m_channel_nicknames[channel]) {
			m_irc_plugin_system.removeConferenceItem("IRC", channel, m_account_name, nickname);
		}
	}
	m_channel_nicknames.clear();
	m_channel_list.clear();

}

void ircAccount::consoleActionTriggered()
{
	consoleWidget->show();
}

void ircAccount::channelslistActionTriggered()
{
	listChannel *channelslist;
	channelslist = new listChannel();
	connect(channelslist, SIGNAL(requestList()), ircpr, SLOT(requestChannelsList()));
	connect(channelslist, SIGNAL(joinChannel(QString)), ircpr, SLOT(joinChannel(QString)));
	connect(ircpr, SIGNAL(listChannelRow(QString, QString, QString)), channelslist, SLOT(addRow(QString, QString, QString)));
	channelslist->show();
}

void ircAccount::joinconfActionTriggered()
{
	joinChannelDialog joinChannelDialog;
	if (joinChannelDialog.exec()) ircpr->joinChannel(joinChannelDialog.getChannel());
}

void ircAccount::showTopicConfig(QString channel)
{
	textDialog textDialog;
	textDialog.setTitle(tr("Change topic"));
	textDialog.setText(m_channel_topic[channel]);
	if (textDialog.exec()) ircpr->sendMsg("", "/TOPIC "+channel+" :"+textDialog.getText().replace("\n", " "));
}

void ircAccount::statusChanged(QString st)
{
	setStatusIcon(st);
	if (st=="online") {
		joinconfAction->setEnabled(true);
		channelslistAction->setEnabled(true);
	}
	else if (st=="offline") {
		joinconfAction->setEnabled(false);
		channelslistAction->setEnabled(false);
	}
}

void ircAccount::setStatusIcon(QString st)
{
	m_status_icon = getIcon(st);
	m_account_button->setIcon(m_status_icon);
	m_status_menu->setIcon(m_status_icon);
}

QIcon ircAccount::getIcon(QString it)
{
	QIcon ri = m_irc_plugin_system.getStatusIcon(it, "irc");
	if (ri.pixmap(16,16).isNull()) return QIcon(":/icons/irc-"+it+".png");
	else return ri;
}

void ircAccount::joinedChannel(QString channel, QString nickname)
{
	if (nickname == ircpr->getNick()) {
		if (m_channel_list.indexOf(channel)) m_channel_list.removeAll(channel);
		if (m_channel_nicknames.contains(channel)) {
			foreach (QString cnick, m_channel_nicknames[channel]) {
				m_irc_plugin_system.removeConferenceItem("IRC", channel, m_account_name, nickname);
			}
		}
		m_channel_list.append(channel);
		m_irc_plugin_system.createConference("IRC", channel, m_account_name);
		m_irc_plugin_system.changeOwnConferenceNickName("IRC", channel, m_account_name, nickname);
		channelSystemMsg(channel, "You have joined the channel "+channel);
		serverMsg("You have joined the channel "+channel, "info");
	}
	else {
		channelSystemMsg(channel, nickname+" has joined this channel");
		channelNickRoleSet(channel, nickname);
	}
}

void ircAccount::leavedChannel(QString channel, QString nickname, QString msg)
{
	if (nickname == ircpr->getNick()) {
		serverMsg("You have left the channel "+channel+" : "+msg, "info");
	}
	else {
		m_channel_nicknames[channel].removeAll(nickname);
		m_irc_plugin_system.removeConferenceItem("IRC", channel, m_account_name, nickname);
		channelSystemMsg(channel, nickname+" has left this channel : "+msg);
	}
}

void ircAccount::kickedFromChannel(QString channel, QString nickname, QString bynick, QString msg)
{
	if (nickname == ircpr->getNick()) {
		if (m_channel_list.indexOf(channel)) m_channel_list.removeAll(channel);
		if (m_channel_nicknames.contains(channel)) {
			foreach (QString cnick, m_channel_nicknames[channel]) {
				m_irc_plugin_system.removeConferenceItem("IRC", channel, m_account_name, nickname);
			}
		}
		QString outmsg = "You have been kicked from channel "+channel+" by "+bynick+" : "+msg;
		channelSystemMsg(channel, outmsg);
		serverMsg(outmsg, "info");
	}
	else {
		m_channel_nicknames[channel].removeAll(nickname);
		m_irc_plugin_system.removeConferenceItem("IRC", channel, m_account_name, nickname);
		channelSystemMsg(channel, nickname+" has left this channel : "+msg);
	}
}

void ircAccount::nickAvatar(QString nickname, QString avatar) {
	ircavatar->requestForAvatar(nickname, avatar);
}

void ircAccount::nickAvatarArrived(const QString &nickname, const QByteArray &avatar_hash)
{
	m_nickavatars[nickname] = avatar_hash.toHex();
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+m_account_name, "contactlist");
	QString avatar_path = settings.fileName().section('/', 0, -3) + "/ircicons/" + m_nickavatars[nickname];
	foreach (QString channel, m_channel_list) {
		if (m_channel_nicknames[channel].contains(nickname)) {
			m_irc_plugin_system.setConferenceItemIcon("IRC", channel, m_account_name, nickname, QIcon(avatar_path), 1);
		}
	}
}

void ircAccount::nickRename(QString nickname, QString new_nickname)
{
	if (nickname==ircpr->getNick()) {
		ircpr->setNick(new_nickname);
	}
	foreach (QString channel, m_channel_list) {
		if (m_channel_nicknames[channel].contains(nickname)) {
			m_channel_nicknames[channel].removeAll(nickname);
			m_channel_nicknames[channel].append(new_nickname);

			QHash<QString, QString> nickmodes = m_channel_nickmodes[channel];
			nickmodes[new_nickname] = nickmodes[nickname];
			nickmodes.remove(nickname);
			m_channel_nickmodes[channel] = nickmodes;

			m_irc_plugin_system.renameConferenceItem("IRC", channel, m_account_name, nickname, new_nickname);
			m_irc_plugin_system.changeOwnConferenceNickName("IRC", channel, m_account_name, new_nickname);
			channelSystemMsg(channel, nickname+" is now known as "+new_nickname);

			if (m_nickavatars.contains(nickname)) {
				m_nickavatars[new_nickname] = m_nickavatars[nickname];
				m_nickavatars.remove(nickname);
			}

		}
	}
}

void ircAccount::nickQuit(QString nickname, QString msg)
{
	foreach (QString channel, m_channel_list) {
		if (m_channel_nicknames[channel].contains(nickname)) {
			m_channel_nicknames[channel].removeAll(nickname);
			m_irc_plugin_system.removeConferenceItem("IRC", channel, m_account_name, nickname);
			channelSystemMsg(channel, nickname+" has left this server : "+msg);
		}
	}
}

void ircAccount::channelAddNickNames(QString channel, QStringList nicknames)
{
	foreach (QString nickname, nicknames) {
		channelNickRoleSet(channel, nickname);
	}
}

void ircAccount::channelMsg(QString channel, QString nickname, QString msg)
{
	m_irc_plugin_system.addMessageToConference("IRC", channel, m_account_name, nickname, msg, QDateTime::currentDateTime(), false);
}

void ircAccount::channelTopic(QString channel, QString nickname, QString msg) {
	m_channel_topic[channel] = msg;
	m_irc_plugin_system.setConferenceTopic("IRC", channel, m_account_name, msg);
	channelSystemMsg(channel, "Channel topic is: "+msg);
}


void ircAccount::channelNickRoleSet(QString channel, QString nickname) {
	int role=9; // normal
	QRegExp rx;
	rx.setPattern("^\\*(.+)");
	if (rx.indexIn(nickname)==0) {
		role = 0; // ircop
		nickname = rx.cap(1);
	}
	rx.setPattern("^\\~(.+)");
	if (rx.indexIn(nickname)==0) {
		role = 1; // chowner
		nickname = rx.cap(1);
	}
	rx.setPattern("^\\&(.+)");
	if (rx.indexIn(nickname)==0) {
		role = 2; // chadmin
		nickname = rx.cap(1);
	}
	rx.setPattern("^\\@(.+)");
	if (rx.indexIn(nickname)==0) {
		role = 3; // chop
		nickname = rx.cap(1);
	}
	rx.setPattern("^\\%(.+)");
	if (rx.indexIn(nickname)==0) {
		role = 4; // choplim
		nickname = rx.cap(1);
	}
	rx.setPattern("^\\+(.+)");
	if (rx.indexIn(nickname)==0) {
		role = 5; //chvoice
		nickname = rx.cap(1);
	}

	QHash<int, QString> rolesmode;
	rolesmode[1] = "q";
	rolesmode[2] = "a";
	rolesmode[3] = "o";
	rolesmode[4] = "h";
	rolesmode[5] = "v";

	if (rolesmode.contains(role)) {
		QHash<QString, QString> nicksmodes = m_channel_nickmodes[channel];
		QString nickmodes = nicksmodes[nickname];
		nickmodes.replace(rolesmode[role], "");
		nickmodes.append(rolesmode[role]);
		nicksmodes[nickname] = nickmodes;
		m_channel_nickmodes[channel] = nicksmodes;
	}

	if (!m_channel_nicknames[channel].contains(nickname)) {
		m_irc_plugin_system.addConferenceItem("IRC", channel, m_account_name, nickname);
		m_channel_nicknames[channel].append(nickname);
	}
	else qDebug()<<"IRC"<<channel<<"Duplicate nickname"<<nickname;

	channelNickRoleChange(channel, nickname, role);
}

void ircAccount::channelNickRoleChange(QString channel, QString nickname, int role) {
	QString rolename = "normal";
	QString roleicon = "white";
	if (role==0) { rolename = tr("IRC operator"); roleicon="black"; }
	else if (role==1) { rolename = modenames['q']; roleicon="pink"; }
	else if (role==2) { rolename = modenames['a']; roleicon="red"; }
	else if (role==3) { rolename = modenames['o']; roleicon="orange"; }
	else if (role==4) { rolename = modenames['h']; roleicon="green"; }
	else if (role==5) { rolename = modenames['v']; roleicon="blue"; }

	if (nickname == ircpr->getNick()) my_channel_role[channel] = role;

	m_irc_plugin_system.setConferenceItemIcon("IRC", channel, m_account_name, nickname, getIcon("normal"), 2);
	m_irc_plugin_system.setConferenceItemRole("IRC", channel, m_account_name, nickname, getIcon(roleicon), rolename, role);
}


void ircAccount::channelNickModeChange(QString channel, QString nickname, QString mode) {
	QRegExp rx("^([\\+\\-])([A-Za-z])$");
	if (rx.indexIn(mode)==0) {
		QHash<QString, QString> nicksmodes = m_channel_nickmodes[channel];
		QString nickmodes = nicksmodes[nickname];
		nickmodes.replace(rx.cap(2), "");
		if (rx.cap(1)=="+") nickmodes.append(rx.cap(2));

		QHash<QChar, int> modesrole;
		modesrole['q'] = 1;
		modesrole['a'] = 2;
		modesrole['o'] = 3;
		modesrole['h'] = 4;
		modesrole['v'] = 5;

		int mrole = 9;
		for (int i=0; i<nickmodes.length(); i++) {
			QChar cmode = nickmodes[i];
			if (modesrole.contains(cmode)) {
				if (modesrole[cmode]<mrole) mrole = modesrole[cmode];
			}
		}

		nicksmodes[nickname] = nickmodes;
		m_channel_nickmodes[channel] = nicksmodes;

		channelNickRoleChange(channel, nickname, mrole);
	}
}

void ircAccount::channelSystemMsg(QString channel, QString msg) {
	m_irc_plugin_system.addSystemMessageToConference("IRC", channel, m_account_name, msg, QDateTime::currentDateTime(), false);
}

void ircAccount::privateMsg(QString nickname, QString msg) {
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "IRC";
	contact_item.m_account_name = m_account_name;
	contact_item.m_item_name = nickname;
	contact_item.m_item_type = 0;
	m_irc_plugin_system.addMessageFromContact(contact_item, msg, QDateTime::currentDateTime());
}

void ircAccount::serviceMsg(QString nickname, QString msg) {
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "IRC";
	contact_item.m_account_name = m_account_name;
	contact_item.m_item_name = nickname;
	contact_item.m_item_type = 0;
	m_irc_plugin_system.addMessageFromContact(contact_item, msg, QDateTime::currentDateTime());
}

void ircAccount::createChat(QString nickname) {
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "IRC";
	contact_item.m_account_name = m_account_name;
	contact_item.m_item_name = nickname;
	contact_item.m_item_type = 0;
	m_irc_plugin_system.createChat(contact_item);
}

QString ircAccount::channelNickToolTip(QString channel, QString nickname) {
	QStringList nickmodenames;
	QHash<QString, QString> nicksmodes = m_channel_nickmodes[channel];
	QString nickmodes = nicksmodes[nickname];
	for (int i=0; i<nickmodes.length(); i++) {
		QChar cmode = nickmodes[i];
		if (modenames.contains(cmode)) {
			nickmodenames.append(modenames[cmode]);
		}
	}

	QString html="<table><tr><td>";
//	html.append("<img src='qrc:/icons/irc-normal.png'> ");
	html.append("<b>"+nickname+"</b><br>");

	if (nickmodenames.count()>0) {
		html.append("<font size='2'><b>"+tr("Mode")+":</b> "+nickmodenames.join(", ")+"</font><br>");
	}

	if (!m_nickavatars[nickname].isEmpty()) {
		QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+m_account_name, "contactlist");
		QString avatar_path = settings.fileName().section('/', 0, -3) + "/ircicons/" + m_nickavatars[nickname];
		html.append("<div><img src='"+avatar_path+"'></div>");
	}

	html.append("</td></tr></table>");
	return html;
}

void ircAccount::conferenceItemContextMenu(const QList<QAction*> &action_list,const QString &channel,
		const QString &nickname,const QPoint &menu_point)
{
	if(!m_channel_list.contains(channel) || !m_channel_nicknames[channel].contains(nickname)) return;
	ci_menu_current_channel = channel;
	ci_menu_current_nickname = nickname;
	ircpr->setReqChannel(channel);
	ircpr->setReqNickname(nickname);
	ircpr->setReqText("");

	QLabel *ci_menu_label;
	ci_menu_label = new QLabel;
	ci_menu_label->setAlignment(Qt::AlignCenter);
	ci_menu_label->setText("<b>"+ nickname +"</b>");
	
	QWidgetAction *ci_menu_title;
	ci_menu_title = new QWidgetAction(this);
	ci_menu_title->setDefaultWidget(ci_menu_label);

	QAction *ci_action_privatechat = new QAction(getIcon("message"), tr("Private chat"), this);
	ci_action_privatechat->setData("privatechat");
	connect(ci_action_privatechat, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_notifyavatar = new QAction(tr("Notify avatar"), this);
	ci_action_notifyavatar->setData("notifyavatar");
	connect(ci_action_notifyavatar, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_info_whois = new QAction("WHOIS", this);
	ci_action_info_whois->setData("info_whois");
	connect(ci_action_info_whois, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));
	
	QAction *ci_action_info_whowas = new QAction("WHOWAS", this);
	ci_action_info_whowas->setData("info_whowas");
	connect(ci_action_info_whowas, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));
	
	QAction *ci_action_info_who = new QAction("WHO", this);
	ci_action_info_who->setData("info_who");
	connect(ci_action_info_who, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_ctcp_avatar = new QAction("AVATAR", this);
	ci_action_ctcp_avatar->setData("ctcp_avatar");
	connect(ci_action_ctcp_avatar, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));


	QAction *ci_action_ctcp_ping = new QAction("PING", this);
	ci_action_ctcp_ping->setData("ctcp_ping");
	connect(ci_action_ctcp_ping, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_ctcp_version = new QAction("VERSION", this);
	ci_action_ctcp_version->setData("ctcp_version");
	connect(ci_action_ctcp_version, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));
	
	QAction *ci_action_ctcp_clientinfo = new QAction("CLIENTINFO", this);
	ci_action_ctcp_clientinfo->setData("ctcp_clientinfo");
	connect(ci_action_ctcp_clientinfo, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_ctcp_userinfo = new QAction("USERINFO", this);
	ci_action_ctcp_userinfo->setData("ctcp_userinfo");
	connect(ci_action_ctcp_userinfo, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_modes_giveop = new QAction(tr("Give Op"), this);
	ci_action_modes_giveop->setData("modes_giveop");
	connect(ci_action_modes_giveop, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_modes_takeop = new QAction(tr("Take Op"), this);
	ci_action_modes_takeop->setData("modes_takeop");
	connect(ci_action_modes_takeop, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_modes_givehalfop = new QAction(tr("Give HalfOp"), this);
	ci_action_modes_givehalfop->setData("modes_givehalfop");
	connect(ci_action_modes_givehalfop, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_modes_takehalfop = new QAction(tr("Take HalfOp"), this);
	ci_action_modes_takehalfop->setData("modes_takehalfop");
	connect(ci_action_modes_takehalfop, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_modes_givevoice = new QAction(tr("Give Voice"), this);
	ci_action_modes_givevoice->setData("modes_givevoice");
	connect(ci_action_modes_givevoice, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_modes_takevoice = new QAction(tr("Take Voice"), this);
	ci_action_modes_takevoice->setData("modes_takevoice");
	connect(ci_action_modes_takevoice, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_kb_kick = new QAction(tr("Kick"), this);
	ci_action_kb_kick->setData("kb_kick");
	connect(ci_action_kb_kick, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_kb_kickwith = new QAction(tr("Kick with..."), this);
	ci_action_kb_kickwith->setData("kb_kickwith");
	connect(ci_action_kb_kickwith, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_kb_ban1 = new QAction(tr("Ban"), this);
	ci_action_kb_ban1->setData("kb_ban1");
	connect(ci_action_kb_ban1, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *ci_action_kb_unban1 = new QAction(tr("UnBan"), this);
	ci_action_kb_unban1->setData("kb_unban1");
	connect(ci_action_kb_unban1, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QMenu *ci_menu_info;
	ci_menu_info = new QMenu();
	ci_menu_info->setIcon(getIcon("info"));
	ci_menu_info->setTitle(tr("Information"));
	ci_menu_info->addAction(ci_action_info_whois);
	ci_menu_info->addAction(ci_action_info_whowas);
	ci_menu_info->addAction(ci_action_info_who);

	QMenu *ci_menu_ctcp;
	ci_menu_ctcp = new QMenu();
	ci_menu_ctcp->setIcon(getIcon("ques"));
	ci_menu_ctcp->setTitle(tr("CTCP"));
	ci_menu_ctcp->addAction(ci_action_ctcp_avatar);
	ci_menu_ctcp->addAction(ci_action_ctcp_ping);
	ci_menu_ctcp->addAction(ci_action_ctcp_version);
	ci_menu_ctcp->addAction(ci_action_ctcp_clientinfo);
	ci_menu_ctcp->addAction(ci_action_ctcp_userinfo);

	QMenu *ci_menu_modes;
	ci_menu_modes = new QMenu();
	ci_menu_modes->setTitle(tr("Modes"));
	if (my_channel_role[channel]<4) {
		ci_menu_modes->addAction(ci_action_modes_giveop);
		ci_menu_modes->addAction(ci_action_modes_takeop);
		ci_menu_modes->addAction(ci_action_modes_givehalfop);
		ci_menu_modes->addAction(ci_action_modes_takehalfop);
	}
	if (my_channel_role[channel]<5) {
		ci_menu_modes->addAction(ci_action_modes_givevoice);
		ci_menu_modes->addAction(ci_action_modes_takevoice);
	}

	QMenu *ci_menu_kb;
	ci_menu_kb = new QMenu();
	ci_menu_kb->setTitle(tr("Kick / Ban"));
	if (my_channel_role[channel]<5) {
		ci_menu_kb->addAction(ci_action_kb_kick);
		ci_menu_kb->addAction(ci_action_kb_kickwith);
		ci_menu_kb->addAction(ci_action_kb_ban1);
		ci_menu_kb->addAction(ci_action_kb_unban1);
	}

	QMenu *ci_menu;
	ci_menu = new QMenu();
	ci_menu->addAction(ci_menu_title);
	ci_menu->addAction(ci_action_privatechat);
	ci_menu->addAction(ci_action_notifyavatar);
	ci_menu->addMenu(ci_menu_info);
	ci_menu->addMenu(ci_menu_ctcp);
	if (my_channel_role[channel]<5) {
		ci_menu->addMenu(ci_menu_modes);
		ci_menu->addMenu(ci_menu_kb);
	}

	// Default actions at menu
	//	for(int i=0;i<2 && i<action_list.size();i++) m_conference_context_menu->addAction(action_list[i]);
	//
	// I don't wanna use this shit


	// Actions at menu from plugins
	for(int i=3;i<action_list.size();i++) ci_menu->addAction(action_list[i]);

	ci_menu->exec(menu_point);
	delete ci_menu;
}

void ircAccount::showChannelConfigMenu(QString channel, QPoint menu_point)
{
	ircpr->setReqChannel(channel);

	QLabel *cc_menu_label;
	cc_menu_label = new QLabel;
	cc_menu_label->setAlignment(Qt::AlignCenter);
	cc_menu_label->setText("<b>"+ channel +"</b>");

	QWidgetAction *cc_menu_title;
	cc_menu_title = new QWidgetAction(this);
	cc_menu_title->setDefaultWidget(cc_menu_label);

	QAction *cc_action_notifyavatar = new QAction(tr("Notify avatar"), this);
	cc_action_notifyavatar->setData("channel_notifyavatar");
	connect(cc_action_notifyavatar, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *cc_action_ctcp_avatar = new QAction("AVATAR", this);
	cc_action_ctcp_avatar->setData("channel_ctcp_avatar");
	connect(cc_action_ctcp_avatar, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *cc_action_ctcp_ping = new QAction("PING", this);
	cc_action_ctcp_ping->setData("channel_ctcp_ping");
	connect(cc_action_ctcp_ping, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *cc_action_ctcp_version = new QAction("VERSION", this);
	cc_action_ctcp_version->setData("channel_ctcp_version");
	connect(cc_action_ctcp_version, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *cc_action_ctcp_clientinfo = new QAction("CLIENTINFO", this);
	cc_action_ctcp_clientinfo->setData("channel_ctcp_clientinfo");
	connect(cc_action_ctcp_clientinfo, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QAction *cc_action_ctcp_userinfo = new QAction("USERINFO", this);
	cc_action_ctcp_userinfo->setData("channel_ctcp_userinfo");
	connect(cc_action_ctcp_userinfo, SIGNAL(triggered()), this, SLOT(actionItemContextTriggered()));

	QMenu *cc_menu_ctcp;
	cc_menu_ctcp = new QMenu();
	cc_menu_ctcp->setIcon(getIcon("ques"));
	cc_menu_ctcp->setTitle(tr("CTCP"));
	cc_menu_ctcp->addAction(cc_action_ctcp_avatar);
	cc_menu_ctcp->addAction(cc_action_ctcp_ping);
	cc_menu_ctcp->addAction(cc_action_ctcp_version);
	cc_menu_ctcp->addAction(cc_action_ctcp_clientinfo);
	cc_menu_ctcp->addAction(cc_action_ctcp_userinfo);

	QMenu *cc_menu;
	cc_menu = new QMenu();
	cc_menu->addAction(cc_menu_title);
	cc_menu->addAction(cc_action_notifyavatar);
	cc_menu->addMenu(cc_menu_ctcp);

	cc_menu->exec(menu_point);
}

void ircAccount::actionItemContextTriggered() {

	QAction *action = qobject_cast<QAction *>(sender());
	if (action->data()=="privatechat") {
		createChat(ci_menu_current_nickname);
	}
	else if (action->data()=="kb_kickwith") {
		textDialog textDialog;
		textDialog.setTitle(tr("Kick reason"));
		if (textDialog.exec()) {
			ircpr->setReqText(textDialog.getText().replace("\n", " "));
			ircpr->actionTriggered("kb_kick");
		}
	}
	else {
		ircpr->actionTriggered(action->data());
	}
}

