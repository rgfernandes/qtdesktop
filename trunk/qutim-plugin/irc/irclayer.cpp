/*
	ircLayer

		Copyright (c) 2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "irclayer.h"
#include "settings/accountsettings.h"

bool ircLayer::init(PluginSystemInterface *plugin_system)
{
	ProtocolInterface::init(plugin_system);
	m_login_widget = 0;
	m_protocol_icon = new QIcon(":/icons/irc_protocol.png");
	m_plugin_system = plugin_system;

	ircPluginSystem::instance().setIrcLayer(this);

	return true;
}

void ircLayer::setProfileName(const QString &profile_name)
{
	m_profile_name = profile_name;
	SystemsCity::PluginSystem()->registerEventHandler("Core/ChatWindow/CreateChat", this, SLOT(eventCreateChat(TreeModelItem,bool)));
}

void ircLayer::eventCreateChat(const TreeModelItem &item, const bool &create)
{
	if(item.m_protocol_name == "IRC" && item.m_item_type == 33)
		(*(QString *)&item.m_item_name).remove(0, item.m_parent_name.size() + 1);
}

void ircLayer::editAccount(const QString &account_name)
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "ircsettings");
	QStringList accounts = settings.value("accounts/list").toStringList();
	if ( accounts.contains(account_name) ){
		QSettings *account_settings = new QSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+account_name, "accountsettings");
		ircAccountSettings *acc_setings = new ircAccountSettings(account_settings);
		acc_setings->show();
	}
}

void ircLayer::removeAccount(const QString &account_name)
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "ircsettings");
	QStringList accounts = settings.value("accounts/list").toStringList();
	accounts.removeAll(account_name);
	accounts.sort();
	settings.setValue("accounts/list", accounts);
	killAccount(account_name);
        QSettings dir_settings_path(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+account_name, "accountsettings");
	QDir account_dir(dir_settings_path.fileName());
	account_dir.cdUp();
	if(account_dir.exists()) removeProfileDir(account_dir.path());
	
}

QWidget *ircLayer::loginWidget()
{
	if (!m_login_widget)
		m_login_widget = new AddAccountForm;
	return m_login_widget;
}

void ircLayer::removeLoginWidget()
{
	delete m_login_widget;
	m_login_widget = 0;
}

void ircLayer::applySettingsPressed()
{

}

QList<QMenu*> ircLayer::getAccountStatusMenu()
{
	QList<QMenu*> accounts_menu_list;
	foreach(ircAccount *account, m_irc_list) {
		accounts_menu_list.append(account->getAccountMenu());
	}
	return accounts_menu_list;
}

void ircLayer::addAccountButtonsToLayout(QHBoxLayout *account_button_layout)
{
	m_account_buttons_layout = account_button_layout;
        QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "ircsettings");
	QStringList accountList = settings.value("accounts/list").toStringList();
	foreach(QString account_from_list, accountList)
	{
		addAccount(account_from_list);
	}
}

void ircLayer::saveLoginDataFromLoginWidget()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "ircsettings");
	QStringList accounts = settings.value("accounts/list").toStringList();
	QString account_nick = m_login_widget->getNick();
	QString account_passwd = m_login_widget->getPasswd();
	QString account_server = m_login_widget->getServer();
	QString account_port = m_login_widget->getPort();
	QString account_realname = m_login_widget->getRealName();

	QString account_name = account_server;
	if (accounts.contains(account_name)) account_name = account_server+"_"+account_port;
	if (accounts.contains(account_name)) account_name = account_nick+"@"+account_server;
	if (accounts.contains(account_name)) account_name = account_nick+"@"+account_server+"_"+account_port;

	if (!accounts.contains(account_name)) {
		accounts.append(account_name);
		accounts.sort();
		settings.setValue("accounts/list", accounts);
		QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+account_name, "accountsettings");
		account_settings.setValue("main/name",account_name);
		account_settings.setValue("main/server",account_server);
		account_settings.setValue("main/port",account_port);
		account_settings.setValue("main/nick",account_nick);
		account_settings.setValue("main/password",account_passwd);
		account_settings.setValue("main/realname",account_realname);
		addAccount(account_name);
	}
}

QList<SettingsStructure> ircLayer::getSettingsList()
{
	QList<SettingsStructure> settings_list;



	return settings_list;
}

void ircLayer::removeProtocolSettings()
{
	delete m_login_widget;
}

QList<AccountStructure> ircLayer::getAccountList()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "ircsettings");
	QList<AccountStructure> accounts_list;
	QStringList accounts = settings.value("accounts/list").toStringList();
	foreach(QString account_name, accounts)
	{
		AccountStructure info_account;
		info_account.protocol_icon = *m_protocol_icon;
		info_account.protocol_name = "IRC";
		info_account.account_name = account_name;
		accounts_list.append(info_account);
	}
	return accounts_list;
}

QList<AccountStructure> ircLayer::getAccountStatuses()
{
	QList<AccountStructure> accounts_list;
	foreach(QString account_name, m_irc_list.keys())
	{
		ircAccount *irc_account = m_irc_list.value(account_name);
		if (irc_account)
		{
			AccountStructure info_account;
//			info_account.protocol_icon = irc_account->getStatusIcon();
			info_account.protocol_name = "IRC";
			info_account.account_name = account_name;
			accounts_list.append(info_account);
		}
	}
	return accounts_list;
}

void ircLayer::setAutoAway()
{

}

void ircLayer::setStatusAfterAutoAway()
{

}

void ircLayer::itemActivated(const QString &account_name, const QString &contact_name)
{
//	qDebug()<<"IRC"<<"itemActivated";
}

void ircLayer::itemContextMenu(const QList<QAction*> &action_list, const QString &account_name, const QString &contact_name, int item_type, const QPoint &menu_point)
{

}

QString ircLayer::getConferenceItemToolTip(const QString &conference_name, const QString &account_name, const QString &nickname)
{
	if (m_irc_list.contains(account_name))	{
		return m_irc_list.value(account_name)->channelNickToolTip(conference_name, nickname);
	}
	return "";
}

void ircLayer::sendMessageTo(const QString &account_name, const QString &contact_name, int item_type, const QString& message, int message_icon_position)
{
	m_irc_list.value(account_name)->getProtocol()->sendMsg(contact_name, message);
}

QStringList ircLayer::getAdditionalInfoAboutContact(const QString &account_name, const QString &item_name, int item_type ) const
{
	if (m_irc_list.contains(account_name))	{
		return m_irc_list.value(account_name)->getAdditionalInfoAboutContact(item_name);
	}
	return QStringList();
}

void ircLayer::showContactInformation(const QString &account_name, const QString &item_name, int item_type )
{

}

void ircLayer::sendImageTo(const QString &account_name, const QString &item_name, int item_type, const QByteArray &image_raw )
{

}

void ircLayer::sendFileTo(const QString &account_name, const QString &item_name, int item_type, const QStringList &files)
{

}

void ircLayer::sendTypingNotification(const QString &account_name, const QString &item_name, int item_type, int notification_type)
{

}

void ircLayer::moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item)
{

}

QString ircLayer::getItemToolTip(const QString &account_name, const QString &contact_name)
{

	return "";
}

void ircLayer::deleteItemSignalFromCL(const QString &account_name, const QString &item_name, int type)
{

}

void ircLayer::chatWindowOpened(const QString &account_name, const QString &item_name)
{

}

void ircLayer::sendMessageToConference(const QString &conference_name, const QString &account_name, const QString &message)
{
	m_irc_list.value(account_name)->getProtocol()->sendMsg(conference_name, message);
}

void ircLayer::leaveConference(const QString &conference_name, const QString &account_name)
{
	if (m_irc_list.contains(account_name))	{
		m_irc_list.value(account_name)->getProtocol()->leaveChannel(conference_name);
		m_irc_list.value(account_name)->clearChannelLists(conference_name);
		
	}
}

void ircLayer::addAccount(const QString &account_name)
{
	ircAccount *account = new ircAccount(account_name, m_profile_name);
	account->createAccountButton(m_account_buttons_layout);
	m_irc_list.insert(account_name, account);
}

void ircLayer::killAccount(const QString &account_name)
{
	ircAccount *delete_account = m_irc_list.value(account_name);
	m_irc_list.remove(account_name);
	delete delete_account;
}

void ircLayer::removeProfileDir(const QString &path)
{
	QFileInfo fileInfo(path);
	if(fileInfo.isDir())
	{
		QDir dir(path);
		QFileInfoList fileList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
		for (int i = 0; i < fileList.count(); i++)
			removeProfileDir(fileList.at(i).absoluteFilePath());
		dir.rmdir(path);
	}
	else
		QFile::remove(path);
}

void ircLayer::conferenceItemActivated(const QString &conference_name, const QString &account_name, const QString &nickname)
{

}

void ircLayer::conferenceItemContextMenu(const QList<QAction*> &action_list, const QString &conference_name,
			const QString &account_name, const QString &nickname, const QPoint &menu_point)
{
	if (m_irc_list.contains(account_name)) {
		m_irc_list.value(account_name)->conferenceItemContextMenu(action_list,conference_name,nickname,menu_point);
	}
}

void ircLayer::showConferenceContactInformation(const QString &conference_name, const QString &account_name, const QString &nickname)
{

}

void ircLayer::showConferenceTopicConfig(const QString &conference_name, const QString &account_name)
{
	if (m_irc_list.contains(account_name))	{
		m_irc_list.value(account_name)->showTopicConfig(conference_name);
	}
}

void ircLayer::showConferenceMenu(const QString &conference_name, const QString &account_name, const QPoint &menu_point)
{
	if (m_irc_list.contains(account_name))	{
		m_irc_list.value(account_name)->showChannelConfigMenu(conference_name, menu_point);
	}
}

Q_EXPORT_PLUGIN2(ircprotocol, ircLayer);
