/*
    jAccount.h

    Copyright (c) 2008 by Nikita Belov <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JACCOUNT_H
#define JACCOUNT_H

//#include <QtGui/QWidget>
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QHBoxLayout>
#include <QSettings>
#include <QVector>
#include <QToolButton>

#include <gloox/client.h>
#include <gloox/bookmarkhandler.h>
#include <gloox/privacyitem.h>

#include "jServiceBrowser.h"
#include "jSearch.h"

#include "jVCard.h"
#include "jAddContact.h"
#include "jAdhoc.h"

using namespace gloox;

class jPluginSystem;
class jProtocol;
class jSlotSignal;
class jRoster;
class jConference;
class jVCard;
class VCardRole;
class jJoinChat;
class jSearchConference;
class jSearch;
class jEventHandler;

class jAccount : public QObject
{
	Q_OBJECT
public:
	jAccount(const QString &account_name, const QString &profile_name, jEventHandler *event_handler, QObject *parent = 0);
	~jAccount();
	void createAccountButton(QHBoxLayout *account_button_layout);
	QMenu *getAccountMenu();
	void removeMenuAccount(QMenu *account_menu);
	void removeAccountButton();

	inline jProtocol *getProtocol() const { return m_jabber_protocol; }
	inline jRoster *getJabberRoster() const { return m_jabber_roster; }
	inline jPluginSystem &getPluginSystem() const { return m_jabber_plugin_system; }
	inline jConference *getConferenceManagementObject() const { return m_conference_management_object; }
	inline jSlotSignal *getSlotSignal() const {return m_jabber_slotsignal;}
	inline QString getAccountName() const {return m_account_name;}
	inline QString getProfileName() const {return m_profile_name;}
	inline jEventHandler *getEventHandler() const { return m_jabber_event_handler; }
	Presence::PresenceType getPresence() { return m_status; }
	Presence::PresenceType getGmailHackPresence() { return m_gmail_hack_status; }

	QString getStatusIconName(Presence::PresenceType presence);
	static QString getStatusName(Presence::PresenceType presence);
	static Presence::PresenceType getPresence(const QString &status);
	static int getStatusMass(Presence::PresenceType presence);
	void showVCardWidget(const QString &jid);
	void setVCardInfo(const VCard *vcard, const QString &jid, const QString &avatarUrl);
	void setDiscoItems(const JID &from, const Disco::Items &items);
	const QString &getPathToAvatars() { return m_path_to_avatars; }
	const QIcon &getStatusIcon() { return m_status_icon; }
	const QString &getAccountName() {return m_account_name;}
	const QString &getProfileName() {return m_profile_name;}
	QMenu *getPrivacyMenu() {return m_privacy_list;}
	void setStatusP(const Presence::PresenceType &presence, bool hide=false);
	void setPrivacyExist(bool privacy) {m_privacy_exist = privacy;}
	bool isPrivacyExist() {return m_privacy_exist;}
	void setAutoAway();
	void setStatusAfterAutoAway();
	void setRecentBookmarks(const BookmarkList &b_list, const ConferenceList &c_list, bool isLocal = false);
	void clearRecentBookmarks();
	QList<ConferenceListItem> getRecentBookmarks(bool isLocal = false);
	QList<BookmarkListItem> getRecentUrlmarks();
	void storeBookmarks(const QList<ConferenceListItem> &c_list);

public slots:
	void setRealStatus(int status);
	void getDiscoItem(const QString &server, const QString &node, DiscoHandler *handler);
	void getDiscoInfo(const QString &server, const QString &node, DiscoHandler *handler);
	void showAddDialog(const QString &jid="", const QString &nick="");
	void setPrivacyStatus(bool checked);
	void executeCommand(const QString &command, const QString &node);
	void adhoc(const QString &jid);
	void generalSettingsChanged();

private slots:
	//void showTransportsBrowser();
	void showServicesBrowser();
	void setStatus();
	void s_requestVCard(const QString &jid);
	void s_saveVCard(VCard *vcard);
	void s_closeVCard(const QString &jid);
	void s_requestDiscoItems(const QString &server, const QString &node);
	void addContact(const QString &jid, const QString &name, const QString &group, bool authorize);
	void showInformation(const QString &jid) {showVCardWidget(jid);}
	void showInformation() {showVCardWidget(m_account_name);}
	void joinConference(const QString &conference);
	void registerTransport(const QString &transport);
	void showSearch(const QString &type = "", const QString &jid = "");
	void modifyPrivacyList(const QString &list, const PrivacyItem &jid, bool add);

private:
	void createStatusMenu();
	void setStatusDepends(bool state);

	QToolButton *m_account_button;

	jSlotSignal *m_jabber_slotsignal;
	jProtocol *m_jabber_protocol;
	jRoster *m_jabber_roster;
	jConference *m_conference_management_object;
	jEventHandler *m_jabber_event_handler;

	QAction *onlineAction;
	QAction *offlineAction;
	QAction *ffcAction;
	QAction *awayAction;
	QAction *xaAction;
	QAction *dndAction;
	QHash<QString, QAction *> statusMenuActions;

	QAction *m_join_groupchat;

	QMenu *m_additional_menu;
	QAction *m_open_xml_console;
	QAction *m_add_contact;
	QAction *m_find_contact;
	QAction *m_change_vcard;
	QAction *m_service_browser;

	QMenu *m_privacy_list;

	QMenu *m_status_menu;
	QString m_account_name;
	QString m_profile_name;
	QString m_path_to_avatars;

	jPluginSystem &m_jabber_plugin_system;

	Presence::PresenceType m_status;
	Presence::PresenceType m_autoaway_status;
	Presence::PresenceType m_gmail_hack_status;
	QIcon m_status_icon;

	QHash<QString, jVCard*> m_vcard_list;
	Adhoc *m_adhoc;

	bool m_privacy_exist;
};

#endif
