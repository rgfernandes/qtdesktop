/*
    ircAccount

    Copyright (c) 2008 by Alexander Kazarin <boiler@co.ru>s

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef IRCACCOUNT_H
#define IRCACCOUNT_H

//#include <QtGui/QWidget>
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QHBoxLayout>
#include <QSettings>
#include <QVector>
#include <QToolButton>
#include <QHash>
#include <QWidgetAction>

#include <QDebug>

#include "ircpluginsystem.h"
#include "accountbutton.h"
//#include "soundevents.h"

#include "ircprotocol.h"
#include "ircconsole.h"
#include "ircavatar.h"

// Qt standard classes
class QSystemTrayIcon;
class QGridLayout;
class QMenu;
class QAction;
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
class QSystemTrayIcon;
class QActionGroup;

// qutIM own classes
class accountButton;
class ircSettings;
class ircPluginSystem;
class ircConference;
//class networkSettings;
//class statusSettings;

//class statusDialog;
//class customStatusDialog;
//class SoundEvents;

class ircAccount : public QObject
{
	Q_OBJECT
public:
	ircAccount(const QString &account_name, const QString &profile_name, QObject *parent = 0);
	~ircAccount();
	void createAccountButton(QHBoxLayout *account_button_layout);
	QMenu *getAccountMenu();
	void removeMenuAccount(QMenu *account_menu);

	inline ircProtocol *getProtocol() const { return ircpr; }
	inline ircPluginSystem &getPluginSystem() const { return m_irc_plugin_system; }
//	inline ircConference *getConferenceManagementObject() const { return m_conference_management_object; }
	inline QString getAccountName() const {return m_account_name;}
	QStringList getAdditionalInfoAboutContact(const QString &item_name);
	void clearChannelLists(QString channel) { m_channel_list.removeAll(channel); m_channel_nicknames[channel].clear(); }
	void conferenceItemContextMenu(const QList<QAction*> &, const QString &, const QString &,const QPoint &);
	void showTopicConfig(QString);
	void showChannelConfigMenu(QString, QPoint);
	QString channelNickToolTip(QString, QString);


public slots:

signals:
	void serverMsg(QString, QString);

private slots:
	void onlineActionTriggered();
	void awayActionTriggered();
	void offlineActionTriggered();
	void consoleActionTriggered();
	void channelslistActionTriggered();
	void joinconfActionTriggered();
	void statusChanged(QString);
	void setStatusIcon(QString);
	void joinedChannel(QString, QString);
	void leavedChannel(QString, QString, QString);
	void kickedFromChannel(QString, QString, QString, QString);
	void nickAvatar(QString, QString);
	void nickAvatarArrived(const QString&, const QByteArray&);
	void nickRename(QString, QString);
	void nickQuit(QString, QString);
	void channelAddNickNames(QString, QStringList);
	void channelMsg(QString, QString, QString);
	void channelTopic(QString, QString, QString);
	void channelNickRoleChange(QString, QString, int);
	void channelNickModeChange(QString, QString, QString);
	void channelSystemMsg(QString, QString);
	void privateMsg(QString, QString);
	void serviceMsg(QString, QString);
	void createChat(QString);
	void actionItemContextTriggered();

private:
	ircPluginSystem &m_irc_plugin_system;
	void createStatusMenu();
	QToolButton *m_account_button;
	QAction *onlineAction;
	QAction *awayAction;
	QAction *offlineAction;
	QAction *consoleAction;
	QAction *joinconfAction;
	QAction *channelslistAction;
//	QHash<QString, QAction *> statusMenuActions;
	QMenu *m_status_menu;
	QString m_account_name;
	QString m_profile_name;
	QIcon m_status_icon;
	ircConsole *consoleWidget;
	ircProtocol *ircpr;
	ircAvatar *ircavatar;
//	ircConference *ircconf;
	QIcon getIcon(QString);
	QStringList m_channel_list;
	QHash<QString, QStringList> m_channel_nicknames;
	QHash<QString, QString> m_nickavatars;
	QHash<QString, QHash<QString, QString> > m_channel_nickmodes;
	QHash<QChar, QString> modenames;
	QHash<QString, int> my_channel_role;
	QHash<QString, QString> m_channel_topic;
	TreeModelItem createTreeModelItem(QString);
	QString ci_menu_current_channel;
	QString ci_menu_current_nickname;
	void channelNickRoleSet(QString, QString);
};

#endif
