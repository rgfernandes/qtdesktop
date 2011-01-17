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

#ifndef IRCLAYER_H
#define IRCLAYER_H

#include <qutim/protocolinterface.h>

using namespace qutim_sdk_0_2;

#include "ircaccount.h"
#include "addaccountform.h"

class ircAccount;
class QString;
class QHBoxLayout;

class ircLayer : public QObject, ProtocolInterface
{
	Q_OBJECT
	Q_INTERFACES(qutim_sdk_0_2::PluginInterface)
public:
	virtual bool init(PluginSystemInterface *plugin_system);
	PluginSystemInterface *getMainPluginSystemPointer() { return m_plugin_system; }
	virtual void release() {}
	virtual void processEvent(Event &event) {}
	virtual QWidget *settingsWidget() { return 0; }
	virtual QString name() { return "IRC"; }
	virtual QString description() { return "IRC protocol plugin for qutIM"; }
	virtual QIcon *icon() { return m_protocol_icon; }
	virtual void setProfileName(const QString &profile_name);
	virtual void editAccount(const QString &account_name);
	virtual void removeAccount(const QString &account_name);
	virtual QWidget *loginWidget();
	virtual void removeLoginWidget();
	virtual void applySettingsPressed();
	virtual QList<QMenu*> getAccountStatusMenu();
	virtual void addAccountButtonsToLayout(QHBoxLayout *account_button_layout);
	virtual void saveLoginDataFromLoginWidget();
	virtual QList<SettingsStructure> getSettingsList();
	virtual void removeProtocolSettings();
	virtual QList<AccountStructure> getAccountList();
	virtual QList<AccountStructure> getAccountStatuses();
	virtual void setAutoAway();
	virtual void setStatusAfterAutoAway();
	virtual void itemActivated(const QString &account_name, const QString &contact_name);
	virtual void itemContextMenu(const QList<QAction*> &action_list, const QString &account_name, const QString &contact_name, int item_type, const QPoint &menu_point);
	virtual void sendMessageTo(const QString &account_name, const QString &contact_name, int item_type, const QString& message, int message_icon_position);
	virtual QStringList getAdditionalInfoAboutContact(const QString &account_name, const QString &item_name, int item_type ) const;
	virtual void showContactInformation(const QString &account_name, const QString &item_name, int item_type );
	virtual void sendImageTo(const QString &account_name, const QString &item_name, int item_type, const QByteArray &image_raw );
        virtual void sendFileTo(const QString &account_name, const QString &item_name, int item_type, const QStringList &files);
	virtual void sendTypingNotification(const QString &account_name, const QString &item_name, int item_type, int notification_type);
	virtual void moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item);
	virtual QString getItemToolTip(const QString &account_name, const QString &contact_name);
	virtual void deleteItemSignalFromCL(const QString &account_name, const QString &item_name, int type);
	virtual void chatWindowOpened(const QString &account_name, const QString &item_name);
	virtual void sendMessageToConference(const QString &conference_name, const QString &account_name, const QString &message);
	virtual void leaveConference(const QString &conference_name, const QString &account_name);
	virtual void conferenceItemActivated(const QString &conference_name, const QString &account_name, const QString &nickname);
	virtual void conferenceItemContextMenu(const QList<QAction*> &action_list, const QString &conference_name,
				const QString &account_name, const QString &nickname, const QPoint &menu_point);
	virtual QString getConferenceItemToolTip(const QString &conference_name, const QString &account_name, const QString &nickname);
	virtual void showConferenceContactInformation(const QString &conference_name, const QString &account_name, const QString &nickname);
	virtual void showConferenceTopicConfig(const QString &conference_name, const QString &account_name);
	virtual void showConferenceMenu(const QString &conference_name, const QString &account_name,const QPoint &menu_point);
	virtual void getMessageFromPlugins(const QList<void *> &event) {}
	virtual void chatWindowAboutToBeOpened(const QString&, const QString&) {}
	virtual void chatWindowClosed(const QString&, const QString&) {}

private slots:
	void eventCreateChat(const TreeModelItem &item, const bool &create);
	
private:
	void addAccount(const QString &);
	void killAccount(const QString &);
	void removeProfileDir(const QString &);
	uint event_createchat_id;

	QIcon *m_protocol_icon;
	QHash<QString, ircAccount *> m_irc_list;
	QHBoxLayout *m_account_buttons_layout;
	QString m_profile_name;
	AddAccountForm *m_login_widget;
	PluginSystemInterface *m_plugin_system;
};

#endif
