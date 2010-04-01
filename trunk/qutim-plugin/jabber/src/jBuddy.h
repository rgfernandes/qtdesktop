/*
    jBuddy.h

    Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>
					Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JBUDDY_H
#define JBUDDY_H

#include <gloox/chatstate.h>
#include <gloox/presence.h>
#include "jAccount.h"
#include "jPluginSystem.h"

class jAccount;
class jPluginSystem;

using namespace gloox;

class jBuddy {
public:
	struct ResourceInfo
	{
		Presence::PresenceType m_presence;
		QString m_name;
		int m_priority;
		QString m_status_message_html;
		QString m_status_message;
		QString m_caps_node;		
		QString m_caps_ver;
		QString m_client_name;
		QString m_client_version;
		QString m_client_os;
		ChatStateType m_chat_state;
		QStringList m_features;
		QString m_x_presence;
		QString m_x_status;
		bool m_in_cl;
	};
	jBuddy(const QString &jid, const QString &group, const QString &path_to_avatars);
	virtual ~jBuddy();

	static QString getBaseToolTip(const ResourceInfo &info);
	
	void addResource(const QString & resource, int priority, Presence::PresenceType presence);
	void delResource(const QString & resource);
	void setName(const QString &name) { m_name = name; }
	void setResourceStatus(const QString &resource, Presence::PresenceType presence, int priority);
	QString getName() { return m_name; }
	bool resourceExist(const QString & resource, bool hard = false);
	void newMaxPriorityResource();
	QStringList getResources() { return m_resources_info.keys(); }
	QString getToolTip(const QString &resource="");
	ResourceInfo *getResourceInfo(const QString &name);
	//void setResourceInfo(const QString &name, const ResourceInfo &info);
	QString getMaxPriorityResource();
	int getMaxPriority() { return max_priority_resource_value; }
	QString getGroup();
	void setGroup(const QString &group) { m_group = group; }
	int getCountResources();
	QString getAvatarHash() { return m_avatar_hash; }
	void setAvatarHash(const QString &hash) { m_avatar_hash = hash; }
	Presence::PresenceType getMaxPriorityStatus();
	void setPubsubInfo(const QString &name, const QList<QVariant> &list);
	QList<QVariant> &getPubsubInfo(const QString &name) { return m_pubsub_info[name]; }
	bool hasPubsub(const QString &name);
	QStringList getPubsubKeys() { return m_pubsub_info.keys(); }
	void setSubscription(SubscriptionType type) { m_subscription_type = type; }
	SubscriptionType getSubscription() { return m_subscription_type; }
	ChatStateType m_chat_state;
	QString m_offline_message;
	QDateTime m_last_activity_time;
	QString m_last_activity_message;

private:
	jPluginSystem &m_plugin_system;
	QString m_jid;
	QString m_group;
	QString max_priority_resource;
	QString m_name;
	QString m_avatar_hash;
	QString m_path_to_avatars;
	QHash <QString, ResourceInfo> m_resources_info;
	/*
		m_pub_sub_info:
			0 -- (int) count of pubsub's items
			1-{0} -- items
			{0} +1 -- (int) position
				+2 -- (Qstring) icon path
				+3 -- (Qstring) html
				+4 -- (bool) have >0 element(s)
		mood:
			1 -- (QString) name
			2 -- (QString) text
		activity:
			1 -- (QString) general
			2 -- (QString) specific
			3 -- (QString) text
		tune:
			1 -- (QString) artist
			2 -- (QString) title
			3 -- (QString) source
			4 -- (QString) track
			5 -- (int) length (def -1)
			6 -- (int) rating (def -1)
			7 -- (QString) uri
	*/
	QHash <QString, QList<QVariant> > m_pubsub_info;
	int count_resources;
	int max_priority_resource_value;
	SubscriptionType m_subscription_type;
};

#endif
