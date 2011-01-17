/*
    jConference

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>
	     2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
				 2008-2009 by Denis Daschenko <daschenko@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JCONFERENCE_H_
#define JCONFERENCE_H_

#include <QString>
//#include <QObject>
#include <gloox/client.h>
#include <gloox/mucroom.h>
#include <gloox/mucroomhandler.h>
#include <gloox/message.h>
#include <gloox/discohandler.h>
#include <gloox/mucroomconfighandler.h>
#include <gloox/dataform.h>
#include "src/jBuddy.h"
#include "src/jJoinChat.h"
#include "src/conference/topicConfigDialog.h"
#include "src/jSaveBookmark.h"

using namespace gloox;

class jProtocol;
class jAccount;
class jPluginSystem;
class jJoinChat;
class jConferenceConfig;
class jConferenceParticipant;
class VersionExtension;

class jConference : public QObject, MUCRoomHandler, DiscoHandler, PresenceHandler, MUCRoomConfigHandler
{
	Q_OBJECT

public:
	struct MucContact
	{
		jBuddy::ResourceInfo m_info;
		QString m_avatar_hash;
		QString m_role_string;
		QString m_affiliation_string;
		QString m_real_jid;
		MUCRoomRole m_role;
		MUCRoomAffiliation m_affiliation;
	};
	struct Room
	{
		inline Room() : entity(NULL) {}
		~Room();
		MUCRoom *entity;
		QString topic;
		QHash<QString, MucContact> contacts_list;
		QPointer<jConferenceConfig> config;
		QPointer<jConferenceParticipant> participant;
		QDateTime last_message;
		QString password;
	};
	jConference(jProtocol *real_parent, Client *client, jAccount *plugin_system, QObject *parent=0);
	virtual ~jConference();
	void loadSettings();
	void handleMUCParticipantPresence (MUCRoom *room, const MUCRoomParticipant participant, const Presence &presence);
	void handleMUCMessage (MUCRoom *room, const Message &message, bool privateMessage);
	bool handleMUCRoomCreation (MUCRoom *room);
	void handleMUCSubject (MUCRoom *, const std::string &, const std::string &);
	void handleMUCInviteDecline (MUCRoom *, const JID &, const std::string &);
	void handleMUCError (MUCRoom *room, StanzaError error);
	void handleMUCInfo (MUCRoom *, int , const std::string &, const DataForm *);
	void handleMUCItems (gloox::MUCRoom*, const std::list<gloox::Disco::Item*, std::allocator<gloox::Disco::Item*> >&);
	void handleMUCConfigList(MUCRoom *room, const MUCListItemList &items, MUCOperation operation);
	void handleMUCConfigForm(MUCRoom *room, const DataForm &form);
	void handleMUCConfigResult(MUCRoom *room, bool success, MUCOperation operation);
	void handleMUCRequest(MUCRoom *room, const DataForm &form);
	void sendMessageToConference(const QString &conference_name, const QString &message);
	void leaveConference(const QString& conference_name);
	bool JIDIsRoom(const QString &jid){ return m_rooms.contains(jid); }
	void handleDiscoInfo(const JID &from, const Disco::Info &info, int context);
	void handleDiscoItems(const JID &from, const Disco::Items &items, int context);
	void handleDiscoError(const JID &from, const Error *error, int context);
	bool handleDiscoSet(const IQ &iq);
	void handlePresence (const Presence &presence);
	void setClient(const QString &conference, const QString &nick, const VersionExtension *version);
	void sendPresence(const QString &conference);
	void sendPresenceToAll();
	QString getToolTip(const QString &conference, const QString &nickname);
	QStringList getAdditionalInfoAboutContact(const QString &conference, const QString &nickname);
	void setAvatar(const QString &conference, const QString &nickname, const QString &path);
	void connectAll();
	void disconnectAll();
	void setConferences();
	void showTopicConfig(const QString &conference);
	void itemContextMenu(const QList<QAction*> &action_list,const QString &conference_name,
		const QString &nickname,const QPoint &menu_point);
	void chatWindowAboutToBeOpened(const QString &conference, const QString &nick);
	void chatWindowClosed(const QString &conference, const QString &nick);
	void joinGroupchat(const QString &room, const QString &nick, const QString &password, bool showBookmarks);
	QStringList getConferenceList() {return m_rooms.keys();}
	void showConferenceMenu(const QString &room, const QPoint &menu_point);

public slots:
	//void joinGroupChat();
	void joinGroupchat();
	void destroyJoinGroupChat();
//	void createConferenceRoom(const QString &host,
//			const QString &room,
//			const QString &nick,
//			const QString &password);
	void s_createConferenceRoom(const QString &conference, const QString &nickname, const QString &password, MUCRoom::HistoryRequestType request_type = MUCRoom::HistoryUnknown, const QString &value = "");
	void kickUser();
	void banUser();
	void setVisitorUser();
	void setParticipantUser();
	void setModeratorUser();
	void s_conferenceInvite(const JID &room, const JID &from, const QString &reason, const QString &password);
	void inviteUser();
	void createConfigDialog(const QString &room);
	void createConfigDialog();
	void createParticipantDialog();
	void createParticipantDialog(const QString &room);

signals:
	void addConferenceItem(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname);
	void removeConferenceItem(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname);
	void renameConferenceItem(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname,const QString &new_nickname);
	void setConferenceItemStatus(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname,const QString &status, int mass);
	void setConferenceItemRole(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname,const QString &role, int mass);
	void addMessageToConference(const QString &conference_name, const QString &account_name,
			const QString &from, const QString &message, const QDateTime &date, bool history);
	void conferenceClientVersion(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname, const QString &name);
	void changeOwnConferenceNickName(const QString &protocol_name,
			const QString &conference_name, const QString &account_name, const QString &nickname);
	void createConference(const QString &protocol_name, const QString &conference_name, const QString &account_name);
	void setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name,
			const QString &nickname, const QString &icon_name, int position);
	void setConferenceTopic(const QString &protocol_name, const QString &conference_name,
			const QString &account_name, const QString &topic);
	void addSystemMessageToConference(const QString &protocol_name, const QString &conference_name,
			const QString &account_name, const QString &message, const QDateTime &date, bool history);
	void addToRoster(QString, QString);

private slots:
	void storeRoomConfig(const DataForm &form);
	void storeRoomParticipant(const QString &room, const MUCListItemList &items, MUCOperation operation);
	void copyJID();
	void addToRoster();
	void createConferenceRoom();
	void saveToBookmark();

private:
	jAccount *m_jabber_account;
	Client *m_client_base;
	QHash<QString, Room *> m_rooms;
	QStringList m_conference_windows;
	QString m_account_name;
	jProtocol *m_real_parent;
	jPluginSystem &m_plugin_system;
	Presence *m_presence;
	jJoinChat *m_join_form;
	QMenu *m_context_menu;
	QLabel *m_menu_label;
	QWidgetAction *m_menu_title;
	QAction *m_kick_user;
	QAction *m_ban_user;
	QActionGroup *m_role_user;
	QAction *m_user_visitor;
	QAction *m_user_participant;
	QAction *m_user_moderator;
	QString m_current_context_nick;
	QString m_current_context_conference_name;
	bool m_auto_retrieve_vcards;
};

#endif /*JCONFERENCE_H_*/
