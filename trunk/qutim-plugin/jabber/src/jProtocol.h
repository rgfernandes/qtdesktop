/*
jProtocol.cpp

Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>
						   Nigmatullin Ruslan <euroelessar@gmail.com>
						   Denis Daschenko <daschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JPROTOCOL_H
#define JPROTOCOL_H

#include <QHash>
//#include <QHashIterator>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QNetworkProxy>
#include <QDebug>

#include "jInvitationHandler.h"

#include <gloox/pubsubitem.h>
#include <gloox/client.h>
#include <gloox/capabilities.h>
#include <gloox/connectionlistener.h>
#include <gloox/discohandler.h>
#include <gloox/disco.h>
#include <gloox/presencehandler.h>
#include <gloox/rostermanager.h>
#include <gloox/messagehandler.h>
#include <gloox/message.h>
#include <gloox/chatstatehandler.h>
#include <gloox/chatstate.h>
#include <gloox/iqhandler.h>
#include <gloox/lastactivity.h>
#include <gloox/pubsubmanager.h>
#include <gloox/vcard.h>
#include <gloox/vcardmanager.h>
#include <gloox/vcardhandler.h>
#include <gloox/bookmarkstorage.h>
#include <gloox/pubsubevent.h>
#include <gloox/lastactivityhandler.h>
#include <gloox/pubsubresulthandler.h>
#include <gloox/error.h>
#include <gloox/eventhandler.h>
#include <gloox/privacymanager.h>
#include <gloox/privacylisthandler.h>
#include <gloox/privacyitem.h>
#include <gloox/capabilities.h>
#include <gloox/receipt.h>
#include <gloox/vcardupdate.h>
#include <gloox/sha.h>
#include <gloox/connectionsocks5proxy.h>
#include <gloox/socks5bytestreamserver.h>
#include <gloox/connectionbosh.h>
#include <gloox/connectionhttpproxy.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/pubsubitem.h>
#include <gloox/nickname.h>
#include <gloox/pubsubevent.h>

#include <qutim/plugininterface.h>
#include "../include/qutim/jabber.h"

#include <string>

using namespace gloox;
using namespace qutim_sdk_0_2;
using namespace qutim_sdk_jabber;

#include "jTransport.h"
#include "jConnection.h"
#include "jAccount.h"
#include "jBuddy.h"
#include "jFileTransfer.h"
#include "jServiceDiscovery.h"
#include "jDiscoItem.h"
#include "xeps/versionextension.h"
#include "xeps/moodsextenstion.h"
#include "xeps/xpresenceextension.h"
#include "xeps/xstatusextension.h"
#include "xeps/gmailextension.h"
#include "xeps/activityextension.h"
#include "xeps/tuneextension.h"
#include "conference/jConference.h"
#include "jClientIdentification.h"
#include "jConnectionServer.h"
#include "customstatusdialog.h"
#include "activitydialog.h"
#include "utils.h"
#include <QDateTime>

class jAccount;
class jRoster;
class jBuddy;
class jConference;
class jServiceDiscovery;
class jDiscoItem;
class jFileTransfer;

class jProtocol : public QThread, RosterListener, ConnectionListener, MessageHandler,
		PresenceHandler, IqHandler, DiscoHandler, VCardHandler, SubscriptionHandler,
		BookmarkHandler, LogHandler, gloox::EventHandler, PrivacyListHandler, LastActivityHandler,
		PubSub::ResultHandler
{
	Q_OBJECT

public:
	jProtocol(const QString &account_name, const QString &profile_name, jRoster *jabber_roster, jAccount *parent);

	void loadSettings();

	~jProtocol();
	static QString getBare(const QString &full);
	static QString getResource(const QString &full);
	jConference *getConferenceManagmentObject(){return m_conference_management_object;}
	void setStatus(const Presence::PresenceType &presence, QString message = "");
	BookmarkStorage *getBookmarkStorage() { return m_bookmark_storage; }
	Client *getClient() { return jClient; }
	LastActivity *getLastActivityObj() { return m_last_activity; }

	void run();

	void onConnect();
	void onDisconnect(ConnectionError e);
	bool onTLSConnect(const CertInfo& info);
	void onResourceBindError(ResourceBindError error);
	void onSessionCreateError(SessionCreateError error);
	void handleItemSubscribed(const JID& jid);
	void handleItemAdded(const JID& jid);
	void handleItemUnsubscribed(const JID& jid);
	void handleItemRemoved(const JID& jid);
	void handleItemUpdated(const JID& jid);
	void handleRoster(const Roster& roster);
	void handleRosterError(Stanza *stanza);
	void handlePresence (const Presence &presence);
	void handleSubscription (const Subscription &s10n);
	void handleRosterPresence(const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string &msg);
	void handleSelfPresence(const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string &msg);
	void handleNonrosterPresence(const gloox::Presence&);
	void handleRosterError(const gloox::IQ&);
	bool handleSubscriptionRequest(const JID& jid, const std::string &msg);
	bool handleUnsubscriptionRequest(const JID& jid, const std::string &msg);
	void handleNonrosterPresence(Stanza* stanza);
	void handleMessage(const Message &stanza, MessageSession *session);
	void sendMessageTo(const QString &contact_name, const QString& message, int message_icon_position);
	void sendTypingNotification(const QString &item_name, int notification_type);
	bool handleIq(const IQ &iq);
	void handleIqID(const IQ &iq, int context);
	void handleDiscoInfo(const JID &from, const Disco::Info &info, int context);
	void handleDiscoItems(const JID &from, const Disco::Items &items, int context);
	void handleDiscoError(const JID &from, const Error *error, int context);
	bool handleDiscoSet(const IQ &iq);
	QStringList getAdditionalInfoAboutContact(const QString &item_name);
	void fetchVCard(const QString &jid, bool is_auto=false);
	void storeVCard(VCard *vcard);
	void setAvatar(const JID &jid, const QString &hash);
	inline QString getResource() { return m_resource; }
	inline int getPriority() { return m_priority.value(m_presence); }
	void updateAvatarPresence(const QString &hash);
	QString getAccountName() { return m_my_nick; }
	void moveContact(const QString &item_name, const QString &parent_name);
	void addContact(const QString &jid, QString name, const QString &group, bool authorize);
	void setActions(/*QAction *bookmark, */QAction *mood, QAction *activity);
	//QAction *getBookmarkAction() { return m_bookmark_action; }
	//QMenu *getBookmarkMenu() { return m_bookmark_menu; }
	//void setBookmarkMenu(QMenu *menu) { m_bookmark_menu = menu; }
	void handleLog (LogLevel level, LogArea area, const std::string &message);
	bool hasFeature(const QString &feature) { return qBinaryFind(m_server_features,feature)!=m_server_features.end(); }
	virtual void handleItem( const JID& service, const std::string& node, const Tag* entry ) {}
	virtual void handleItems( const std::string& id, const JID& service, const std::string& node, const PubSub::ItemList& itemList, const Error* error = 0 ) {}
	virtual void handleItemPublication( const std::string& id, const JID& service, const std::string& node, const PubSub::ItemList& item, const Error* error = 0 ) {}
	virtual void handleItemDeletion( const std::string& id, const JID& service, const std::string& node, const PubSub::ItemList& itemList, const Error* error = 0 ) {}
	virtual void handleSubscriptionResult( const std::string& id, const JID& service, const std::string& node, const std::string& sid, const JID& jid, const PubSub::SubscriptionType subType, const Error* error = 0 ) {}
  virtual void handleUnsubscriptionResult( const std::string& id, const JID& jid, const Error* error = 0 ) {}
	virtual void handleSubscriptionOptions( const std::string& id, const JID& service, const JID& jid, const std::string& node, const DataForm* options, const Error* error = 0 ) {}
	virtual void handleSubscriptionOptionsResult( const std::string& id, const JID& service, const JID& jid, const std::string& node, const Error* error = 0 ) {}
	virtual void handleSubscribers( const std::string& id, const JID& service, const std::string& node, const PubSub::SubscriberList* list, const Error* error = 0 ) {}
	virtual void handleSubscribersResult( const std::string& id, const JID& service, const std::string& node, const PubSub::SubscriberList* list, const Error* error = 0 ) {}
	virtual void handleAffiliates( const std::string& id, const JID& service, const std::string& node, const PubSub::AffiliateList* list, const Error* error = 0 ) {}
	virtual void handleAffiliatesResult( const std::string& id, const JID& service, const std::string& node, const PubSub::AffiliateList* list, const Error* error = 0 ) {}
	virtual void handleNodeConfig( const std::string& id, const JID& service, const std::string& node, const DataForm* config, const Error* error = 0 ) {}
	virtual void handleNodeConfigResult( const std::string& id, const JID& service, const std::string& node, const Error* error = 0 ) {}
	virtual void handleNodeCreation( const std::string& id, const JID& service, const std::string& node, const Error* error = 0 ) {}
	virtual void handleNodeDeletion( const std::string& id, const JID& service, const std::string& node, const Error* error = 0 ) {}
	virtual void handleNodePurge( const std::string& id, const JID& service, const std::string& node, const Error* error = 0 ) {}
	virtual void handleSubscriptions( const std::string& id, const JID& service, const PubSub::SubscriptionMap& subMap, const Error* error = 0 ) {}
	virtual void handleAffiliations( const std::string& id, const JID& service, const PubSub::AffiliationMap& affMap, const Error* error = 0 ) {}
	virtual void handleDefaultNodeConfig( const std::string& id, const JID& service, const DataForm* config, const Error* error = 0 ) {}
	void handleEvent(const gloox::Event &event);
	void showTransportReg(const QString &jid);
	void removeTransportReg(const QString &jid);
	void LogOutTransport(const QString &jid);
	void handleBookmarks(const BookmarkList &b_list, const ConferenceList &c_list);
	void LogInTransport(const QString &jid);
	void handlePrivacyListNames(const std::string &active, const std::string &def, const StringList &lists);
	void handlePrivacyList (const std::string &name, const PrivacyList &items);
	void handlePrivacyListChanged (const std::string &name);
	void handlePrivacyListResult (const std::string &id, PrivacyListResult plResult);
	void handleLastActivityResult (const JID &jid, long seconds, const std::string &message);
	void handleLastActivityError (const JID &jid, StanzaError error);
	jFileTransfer *getFileTransfer() { return m_file_transfer; }
	void setPrivacyStatus(QString &list);
	void setMood(const QStringList &list);
	void setActivity(const QStringList &list);
	void setTune(const QStringList &list);
	QString getActivePrivacyList() const {return m_privacy_active;}
	QList<PrivacyItem> getPrivacyList(const QString &list) {return m_privacy_items.value(list);}
	void storePrivacyList(const QString &list, PrivacyList items);

signals:
	void setRealStatus(int presence);
	void setClientCustomIcon(const TreeModelItem &, const QString &, int);
	void setClientCustomText(const TreeModelItem &, const QString &, int);
	void addMessageFrom(const TreeModelItem &item, const QDateTime &, const QString &);
	void messageDelievered(const TreeModelItem &item, int position);
	void sendTypingNotification(const TreeModelItem &item, bool composing);
	void conferenceClientVersion(const QString &protocol_name,const QString &conference_name,const QString &account_name,const QString &nickname, const QString &name);
	void systemNotification(const QString &account_name, const QString &message);
	void onFetchVCard(const QString &m_account_name, const VCard *vcard, const QString &avatarUrl);
	void createAcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client);
	void bookmarksHandled();
	void tagHandled(const QString &xml, bool in);
	void receiveSDisco(const JID &from, const Disco::Items &items, const QString &type);
	void receiveSDiscoInfo(const JID &from, const Disco::Info &info, const QString &type);
	void conferenceInvite(const JID &room, const JID &from, const QString &reason, const QString &password);
	void setPrivacyLists(const QStringList &list, const QString &active);

public slots:
	void addMessageFrom(const JID &from, const Message &stanza, bool conference = false);
	void onSetMood();
	void onSetActivity();
	void onTimer();
	void getDiscoItems(const QString &jid, const QString &node, DiscoHandler *handler = 0);
	void getDiscoInfo(const QString &jid, const QString &node, DiscoHandler *handler = 0);

private slots:
	void receiveInvite(const JID &room, const JID &from, const QString &reason, const QString &password);
	void reconnecting();

private:
	void handleVCard(const JID &jid, const VCard *vcard);
	void handleVCardResult(VCardContext context, const JID &jid, StanzaError se);
	void getListItem(const QString &p_list = "");
	void requestBookmarks();
	jRoster *m_jabber_roster;
	jAccount *m_jabber_account;
	jConference *m_conference_management_object;
	jFileTransfer *m_file_transfer;
	jServiceDiscovery *m_proxy_discovery;
	jDiscoItem *m_server_disco_item;
	Client *jClient;
	VCardManager *m_vcardManager;
	jInvitationHandler *m_invitationHandler;
	LastActivity *m_last_activity;
	BookmarkStorage *m_bookmark_storage;
	PrivacyManager *m_privacy_manager;
	//QMenu *m_bookmark_menu;
	//QAction *m_bookmark_action;
	QAction *m_mood_action;
	QAction *m_activity_action;
	QTimer m_ping_timer;
	int m_server_ping;
//	QTimer *m_gloox_timer;
//	bool m_do_recv;
//	QMutex m_timer_mutex;
	jConnection *m_connection;

	QString m_account_name;
	QString m_profile_name;
	QString m_resource;
	QString m_avatar_hash;
	QString m_my_nick;
	QHash <QString, bool> m_composing_hash;
	QHash <QString,QPair<QString,int> > m_receipts_hash;
	QList<QString> m_jids_auto_vcard;
	QStringList m_server_features;

	int m_xmpp_port;
	QString m_xmpp_server;
	int m_proxy_type;
	int m_proxy_port;
	QString m_proxy_host;
	quint64 m_last_gmail_date;
	PubSub::Manager *m_pubsub_manager;
	QString m_current_mood;
	QString m_current_mood_text;
	QPair<QString,QString> m_current_activity;

	bool m_uExit;
	bool m_pep_support;
	bool m_auto_retrieve_vcards;
	int m_gmail_notify_type;

	QStringList m_privacy_list;
	QHash<QString, QList<PrivacyItem> > m_privacy_items;
	int m_store_count, m_list_count;
	QString m_privacy_active;
	bool m_privacy_default_created;
	bool m_autopriority;
	QHash<Presence::PresenceType, int> m_priority;
	Presence::PresenceType m_presence;

	bool m_show_mainres_notify;

	bool m_local_bookmark;
	bool m_state_online;
};

#endif
