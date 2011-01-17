/*
    jRoster.cpp

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

#include "jRoster.h"

jRoster::jRoster(const QString &account_name, const QString &profile_name, jAccount *jabber_account) :
	m_account_name(account_name),
	m_profile_name(profile_name),
	m_plugin_system(jPluginSystem::instance()),
	m_list_string(QChar(0)+QString("list")),
	m_jabber_account(jabber_account)
{
	QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name/*+"/jabber."+m_account_name*/, "jabbericons");
	m_path_to_avatars = account_settings.fileName();
	m_path_to_avatars.truncate(m_path_to_avatars.length()-4);
	m_add_contact_action = new QAction(m_plugin_system.getIcon("add_user"),tr("Add to contact list"),this);
	connect(m_add_contact_action,SIGNAL(triggered()),this,SLOT(onAddContact()));
	m_rename_action = new QAction(m_plugin_system.getIcon("edituser"),tr("Rename contact"),this);
	connect(m_rename_action,SIGNAL(triggered()),this,SLOT(onRenameAction()));
	m_delete_action = new QAction(m_plugin_system.getIcon("deleteuser"),tr("Delete contact"),this);
	connect(m_delete_action,SIGNAL(triggered()),this,SLOT(onDeleteAction()));
	m_move_action = new QAction(m_plugin_system.getIcon("moveuser"),tr("Move to group"),this);
	connect(m_move_action,SIGNAL(triggered()),this,SLOT(onMoveAction()));
	m_subscription_menu = new QMenu(tr("Authorization"));
	m_subscription_menu->menuAction()->setIcon(m_plugin_system.getIcon("auth"));
	m_send_subscription_action = new QAction(tr("Send authorization to"),this);
	m_subscription_menu->addAction(m_send_subscription_action);
	connect(m_send_subscription_action,SIGNAL(triggered()),this,SLOT(onSendSubscriptionAction()));
	m_ask_subscription_action = new QAction(tr("Ask authorization from"),this);
	m_subscription_menu->addAction(m_ask_subscription_action);
	connect(m_ask_subscription_action,SIGNAL(triggered()),this,SLOT(onAskSubscriptionAction()));
	m_remove_subscription_action = new QAction(tr("Remove authorization from"),this);
	m_subscription_menu->addAction(m_remove_subscription_action);
	connect(m_remove_subscription_action,SIGNAL(triggered()),this,SLOT(onRemoveSubscriptionAction()));
	m_transports_menu = new QMenu(tr("Transports"));
	m_register_transport = new QAction(tr("Register"),this);
	connect(m_register_transport,SIGNAL(triggered()),this,SLOT(onRegisterAction()));
	m_transports_menu->addAction(m_register_transport);
	m_unregister_transport = new QAction(tr("Unregister"),this);
	connect(m_unregister_transport,SIGNAL(triggered()),this,SLOT(onUnregisterAction()));
	m_transports_menu->addAction(m_unregister_transport);
	m_transports_menu->addSeparator();
	m_login_transport = new QAction(tr("Log In"),this);
	connect(m_login_transport,SIGNAL(triggered()),this,SLOT(onLogInTransportAction()));
	m_transports_menu->addAction(m_login_transport);
	m_logout_transport = new QAction(tr("Log Out"),this);
	connect(m_logout_transport,SIGNAL(triggered()),this,SLOT(onLogOutTransportAction()));
	m_transports_menu->addAction(m_logout_transport);
}


jRoster::~jRoster()
{
	this->setOffline();

	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = m_account_name;
	contact.m_item_type = 2;

	emit removeItemFromContactList(contact);

	delete m_my_connections;
	qDeleteAll(m_roster);
}

void jRoster::constr()
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_parent_name = m_account_name;
	contact.m_item_name = m_account_name;
	contact.m_item_type = 2;

	emit addItemToContactList(contact, m_account_name);
	m_my_connections = new jBuddy(m_account_name, "My connections", m_path_to_avatars);
	addGroup("My connections");
}

void jRoster::loadSettings()
{
	bool show_mood = m_show_mood;
	bool show_activity = m_show_activity;
	bool show_both_activity = m_show_both_activity;
	bool show_tune = m_show_tune;
	bool show_xpresence = m_show_xpresence;
	bool show_xstatus = m_show_xstatus;
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	settings.beginGroup("roster");
	if(!settings.value("myconnections", true).toBool())
		this->setInvisible("My connections", m_account_name, 1);
	m_show_mood = settings.value("showmood",false).toBool();
	m_show_activity = settings.value("showactivity",true).toBool();
	m_show_both_activity = settings.value("showbothactivity",false).toBool();
	m_show_tune = settings.value("showtune",false).toBool();
	m_show_xpresence = settings.value("showxpresence",false).toBool();
	m_show_xstatus = settings.value("showxstatus",true).toBool();
	m_show_message_status = settings.value("showmessagestatus",true).toBool();
	m_show_mainres_notify = settings.value("showmainresnotify", true).toBool();
	settings.endGroup();

	/*QStringList buddies = m_roster.keys();
	foreach(const QString &jid, buddies)
	{
		jBuddy *buddy = m_roster.value(jid);
		if(show_mood != m_show_mood && buddy->hasPubsub("mood"))
			updateIcon(jid, "mood");
	}

	QStringList myresources = m_my_connections->getResources();
	foreach(const QString &resource, myresources)
	{
		if(show_mood != m_show_mood && m_my_connections->hasPubsub("mood"))
			updateIcon(m_account_name+"/"+resource, "mood");
	}*/
}

void jRoster::loadRosterFromSettings()
{
	m_jabber_protocol = m_jabber_account->getProtocol();
	//m_conferences = m_jabber_protocol->getConferenceManagmentObject();
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "contactlist");
	QStringList contacts = settings.value(m_list_string+"/contacts").toStringList();
	/*QStringList groups = settings.value(m_list_string+"/groups").toStringList();
	foreach(QString group,groups)
		addGroup(group,false);
	m_groups= groups;*/
	m_groups << "";
	QSettings jabber_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	foreach(QString contact,contacts)
	{
		settings.beginGroup(contact);
		QString group = settings.value("group", "General").toString();
//		if(!m_groups.contains(group) && !m_groups.isEmpty())
//			group = "General";
		QString name = settings.value("nickname",contact).toString();
		addContact(contact,name,group,false);
		//changeItemStatus(contact,Presence::Unavailable);
		if(settings.contains("iconhash"))
		{
			QString hash = settings.value("iconhash").toString();
			setAvatar(contact,hash);
		}
		settings.endGroup();
	}
}

void jRoster::stopLoadRoster()
{
	qSort(m_buddies.begin(), m_buddies.end());
	QStringList roster_buddies = m_roster.keys();
	qSort(roster_buddies.begin(), roster_buddies.end());
	int num=0;
	for(int i=0;i<roster_buddies.size();i++)
	{
		if(num<m_buddies.size() && m_buddies[num]==roster_buddies[i])
			num++;
		else
		{
			moveContact(roster_buddies[i],"");
		}
	}
}

void jRoster::addGroup(const QString &group_name, bool /*use_qsettings*/)
{
	if(m_groups.contains(group_name))
		return;
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = group_name;
	contact.m_parent_name = m_account_name;
	contact.m_item_type = 1;
	if (contact.m_item_name == tr("Services"))
		m_plugin_system.setItemVisible(contact, true);
	emit addItemToContactList(contact, group_name);
	if (group_name != "My connections")
		m_groups << group_name;
	/*if(use_qsettings)
	{
		QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "contactlist");
		settings.setValue(m_list_string+"/groups",QVariant(m_groups));
	}*/
}

jBuddy *jRoster::addContact(const QString &item_name, const QString &contact_name, const QString &parent_name, bool use_qsettings)
{
	jBuddy *contact_buddy;
	if(!m_buddies.contains(item_name))
		m_buddies.append(item_name);
	addGroup(parent_name);
	if(!m_roster.contains(item_name))
	{
		contact_buddy = new jBuddy(item_name, parent_name, m_path_to_avatars);
		contact_buddy->setName(contact_name);
		m_roster.insert(item_name, contact_buddy);
		this->addItem(item_name, contact_name, parent_name, item_name);
		if(use_qsettings)
		{
			QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "contactlist");
			if(!contact_name.isEmpty())
				settings.setValue(item_name+"/nickname",contact_name);
			settings.setValue(item_name+"/group",parent_name);
			settings.setValue(m_list_string+"/contacts",QVariant(m_roster.keys()));;
		}
	}
	else
	{
		contact_buddy = m_roster.value(item_name);
		if(contact_buddy->getName()!=contact_name)
			renameContact(item_name,contact_name);
		if(contact_buddy->getGroup()!=parent_name)
			moveContact(item_name, parent_name);
	}
	return contact_buddy;
}
void jRoster::renameContact(const QString &item_name, const QString &contact_name)
{
	jBuddy *buddy = m_roster.value(item_name);
	if(!buddy)
		return;
	if(buddy->getName()!=contact_name)
	{
		QString group = buddy->getGroup();
		renameItem(item_name,contact_name,group);
		QStringList resources = buddy->getResources();
		foreach(QString resource, resources)
			renameItem(item_name+"/"+resource,contact_name,group);
		QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "contactlist");
		settings.setValue(item_name+"/nickname",contact_name);
		buddy->setName(contact_name);
	}
}

void jRoster::moveContact(const QString &item_name, const QString &parent_name)
{
	jBuddy *buddy = m_roster.value(item_name);
	if(!buddy || parent_name == "My connections")
		return;
	QString group = buddy->getGroup();
	if(group!=parent_name)
	{
		addGroup(parent_name);
		bool nil=parent_name.isEmpty();
		moveItem(item_name,group,parent_name);
		TreeModelItem contact;
		contact.m_protocol_name = "Jabber";
		contact.m_account_name = m_account_name;
		contact.m_item_name = item_name;
		contact.m_parent_name = parent_name;
		contact.m_item_type = 0;
		if(nil)
			emit setContactItemStatus(contact, jAccount::getStatusName(Presence::Unavailable), jAccount::getStatusMass(Presence::Unavailable));
		QStringList resources = buddy->getResources();
		foreach(QString resource, resources)
		{
			moveItem(item_name+"/"+resource,group,parent_name);
			contact.m_item_name = item_name+"/"+resource;
			if(nil)
				emit setContactItemStatus(contact, jAccount::getStatusName(Presence::Unavailable), jAccount::getStatusMass(Presence::Unavailable));
		}
		QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "contactlist");
		settings.setValue(item_name+"/group",parent_name);
		buddy->setGroup(parent_name);
	}
}

void jRoster::removeContact(const QString &item_name)
{
	jBuddy *buddy = m_roster.value(item_name);
	if(!buddy)
		return;
	QString group = buddy->getGroup();
	if(group.isEmpty())
	{
		delItem(item_name, group);
		QStringList resources = buddy->getResources();
		foreach(QString resource, resources)
			delItem(item_name+"/"+resource,group);

		delete m_roster[item_name];
		m_roster.remove(item_name);

		QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "contactlist");
		settings.setValue(m_list_string+"/contacts",QVariant(m_roster.keys()));;
	}
	else
		moveContact(item_name,"");
}


void jRoster::addItem(const QString &item_name, const QString &contact_name, const QString &parent_name, QString history_name, Presence::PresenceType presence, bool invisible)
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = item_name;
	contact.m_parent_name = parent_name;
	contact.m_item_history = history_name;
	contact.m_item_type = 0;

	emit addItemToContactList(contact, contact_name);
	if(invisible)
	{
		this->setItemVisibility(item_name, parent_name, 0, ShowAlwaysInvisible);
		jBuddy *contact_buddy = m_roster.value(jProtocol::getBare(item_name), 0);
		QString res = jProtocol::getResource(item_name);
		if ( contact_buddy && m_show_mainres_notify )
			m_plugin_system.setItemNotifications(contact, ( contact_buddy->getMaxPriorityResource() == res ) ? ShowInvNotifications : ShowAllNotifications);
		if ( !m_show_mainres_notify )
			m_plugin_system.setItemNotifications(contact, ShowAllNotifications);
	}
	else if ( !m_show_mainres_notify && contact.m_parent_name != tr("Services") && contact.m_parent_name != "My connections")
		m_plugin_system.setItemNotifications(contact, 0);
	
	//if (contact.m_parent_name == tr("Services"))
	//	m_plugin_system.setItemVisible(contact, true);
	int mass;
	if (contact.m_parent_name == tr("Services") && presence == Presence::Unavailable)
		mass = jAccount::getStatusMass(presence) - 1;
	else
		mass = jAccount::getStatusMass(presence);

	emit setContactItemStatus(contact, jAccount::getStatusName(presence), mass);
}

void jRoster::delItem(const QString &item_name, const QString &parent_name, bool dontshownot)
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = item_name;
	contact.m_parent_name = parent_name;
	contact.m_item_type = 0;

	if (dontshownot)
		m_plugin_system.setItemNotifications(contact, ShowInvNotifications);
	emit setContactItemStatus(contact, "offline", 1000);
	emit removeItemFromContactList(contact);
}

void jRoster::moveItem(const QString &item_name, const QString &parent_name, const QString &new_parent_name)
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_parent_name = parent_name;
	contact.m_item_name = item_name;
	contact.m_item_type = 0;
	TreeModelItem new_contact = contact;
	new_contact.m_parent_name = new_parent_name;
	emit moveItemInContactList(contact, new_contact);
}

void jRoster::renameItem(const QString &item_name, const QString &contact_name, const QString &parent_name)
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_parent_name = parent_name;
	contact.m_item_name = item_name;
	contact.m_item_type = 0;
	emit setContactItemName(contact,contact_name);
}

void jRoster::addMyConnect(const QString &resource, int priority, Presence::PresenceType presence)
{
	if (!this->myConnectExist(resource))
	{
		m_my_connections->addResource(resource, priority, presence);
		this->addItem(m_account_name+"/"+resource, m_account_name+"/"+resource, "My connections", m_account_name);
	}
}

void jRoster::addResource(const QString &jid, const QString &resource, int priority, Presence::PresenceType presence)
{
	jBuddy *contact_buddy = m_roster.value(jid, 0);
	if ( contact_buddy )
	{
		TreeModelItem contact;
		contact.m_protocol_name = "Jabber";
		contact.m_account_name = m_account_name;
		contact.m_parent_name = contact_buddy->getGroup();
		contact.m_item_name = jid+"/"+resource;
		contact.m_item_type = 0;

		if ( !contact_buddy->resourceExist(resource) )
		{
			contact_buddy->addResource(resource, priority, presence);
			addItem(jid+"/"+resource, (contact_buddy->getName().isEmpty())?jid+"/"+resource:contact_buddy->getName()+"/"+resource, contact_buddy->getGroup(), jid, Presence::Unavailable, true);
		}
		else 
		{
			contact_buddy->setResourceStatus(resource, presence, priority);
			if ( priority >= contact_buddy->getMaxPriority() && contact_buddy->getMaxPriorityResource() != resource )
			{
				if ( m_show_mainres_notify && contact_buddy->getCountResources() > 1 )
				{
					m_plugin_system.setItemNotifications(contact, ShowInvNotifications);
					contact.m_item_name = jid+"/"+contact_buddy->getMaxPriorityResource();
					m_plugin_system.setItemNotifications(contact, ShowAllNotifications);
				}
				contact_buddy->newMaxPriorityResource();
			}
			else if ( priority < contact_buddy->getMaxPriority() && contact_buddy->getMaxPriorityResource() == resource )
			{
				contact_buddy->newMaxPriorityResource();
				if ( m_show_mainres_notify )
				{
					m_plugin_system.setItemNotifications(contact, ShowAllNotifications);
					contact.m_item_name = jid+"/"+contact_buddy->getMaxPriorityResource();
					m_plugin_system.setItemNotifications(contact, ShowInvNotifications);
				}
			}
			if ( contact_buddy->getCountResources() > 1 )
			{
				contact.m_item_name = jid;
				Presence::PresenceType presence = contact_buddy->getMaxPriorityStatus();
				emit setContactItemStatus(contact, jAccount::getStatusName(presence), jAccount::getStatusMass(presence));
				jBuddy::ResourceInfo *info = contact_buddy->getResourceInfo(contact_buddy->getMaxPriorityResource());
				if(info)
					emit clientVersion(contact, info->m_client_name);
				updateIcon(jid, "xstatus");
			}
		}
	}
}

void jRoster::delMyConnect(const QString &resource)
{
	if (this->myConnectExist(resource))
	{
		m_my_connections->delResource(resource);
		this->delItem(m_account_name+"/"+resource, "My connections");
	}
}

void jRoster::delResource(const QString &jid, const QString &resource)
{
	jBuddy *contact_buddy = m_roster.value(jid);
	if(!contact_buddy || !contact_buddy->resourceExist(resource))
	    return;
	QString maxPriorityResource = contact_buddy->getMaxPriorityResource();

	contact_buddy->delResource(resource);

	if (maxPriorityResource == resource)
	{
	    TreeModelItem contact;
	    contact.m_protocol_name = "Jabber";
	    contact.m_account_name = m_account_name;
	    contact.m_item_name = jid;
	    contact.m_parent_name = contact_buddy->getGroup();
	    contact.m_item_type = 0;
	    Presence::PresenceType presence = contact_buddy->getMaxPriorityStatus();
	    emit setContactItemStatus(contact, jAccount::getStatusName(presence), jAccount::getStatusMass(presence));
	    jBuddy::ResourceInfo *info = contact_buddy->getResourceInfo(contact_buddy->getMaxPriorityResource());
	    if(info)
		emit clientVersion(contact, info->m_client_name);
	    updateIcon(jid, "xstatus");
		if ( m_show_mainres_notify )
		{
			contact.m_item_name = jid+"/"+contact_buddy->getMaxPriorityResource();
			m_plugin_system.setItemNotifications(contact, ShowInvNotifications);
		}
	}

	this->changeItemStatus(jid+"/"+resource, Presence::Unavailable);
	delItem(jid+"/"+resource, contact_buddy->getGroup(), true);
}

void jRoster::changeItemStatus(const QString &item_name, Presence::PresenceType presence)
{
	QString resource = jProtocol::getResource(item_name);
	QString jid = jProtocol::getBare(item_name);
	jBuddy *buddy = m_roster.value(jid);
	if(!buddy)
		buddy = m_my_connections;
	if(!buddy)
		return;
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = item_name;
	contact.m_parent_name = (jid != m_account_name) ? buddy->getGroup() : "My connections";
	contact.m_item_type = 0;
	int mass;
	if (contact.m_parent_name == tr("Services") && presence == Presence::Unavailable)
		mass = jAccount::getStatusMass(presence) - 1;
	else
		mass = jAccount::getStatusMass(presence);

	emit setContactItemStatus(contact, jAccount::getStatusName(presence), mass);
}

void jRoster::setInvisible(const QString &item_name, const QString &parent_name, quint8 item_type)
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = item_name;
	contact.m_parent_name = parent_name;
	contact.m_item_type = item_type;

	m_plugin_system.setItemInvisible(contact, true);
}

void jRoster::setItemVisibility(const QString &item_name, const QString &parent_name, quint8 item_type, int flags)
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = item_name;
	contact.m_parent_name = parent_name;
	contact.m_item_type = item_type;

	m_plugin_system.setItemVisibility(contact, flags);
}

void jRoster::setStatusMessage(const QString &jid, const QString &resource, const QString &message)
{
    jBuddy *contact_buddy = m_roster.value(jid);

    if (contact_buddy->resourceExist(resource))
	contact_buddy->getResourceInfo(resource)->m_status_message = message;
    else if (!contact_buddy->getCountResources())
	contact_buddy->m_offline_message = message;
    setStatusRow(jid);
}

bool jRoster::groupExist(const QString &group_name)
{
	return m_groups.contains(group_name);
}

bool jRoster::contactExist(const QString &jid)
{
	return m_roster.contains(jid);
}

bool jRoster::myConnectExist(const QString &resource)
{
	return m_my_connections->resourceExist(resource);
}

QString jRoster::getMaxPriorityResource(const QString &jid)
{
	return m_roster.value(jid)->getMaxPriorityResource();
}

void jRoster::setClient(const QString & jid, const QString & resource, const QString &name, bool my_connection)
{
	jBuddy *o_contact =  m_roster.value(jid);
	if(!o_contact)
	{
		if(jid==m_account_name)
			o_contact=m_my_connections;
		else
			return;
	}
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = (!resource.isEmpty()) ? jid+"/"+resource : jid;
	if(my_connection)
		contact.m_parent_name = "My connections";
	else
		contact.m_parent_name = o_contact->getGroup();
	contact.m_item_type = 0;


	emit clientVersion(contact, name);
	if(o_contact->getMaxPriorityResource()==resource)
	{
		contact.m_item_name = jid;
		emit clientVersion(contact, name);
	}
}

void jRoster::setOffline()
{
	QStringList resources = m_my_connections->getResources();
	foreach(QString resource, resources)
		delMyConnect(resource);
	QStringList contacts = m_roster.keys();
	foreach(QString jid, contacts)
	{
		jBuddy *contact = m_roster[jid];
		if(contact->getCountResources()>0)
		{
			changeItemStatus(jid, Presence::Unavailable);
			resources = contact->getResources();
			foreach(QString resource, resources)
				delResource(jid,resource);
		}
	}
}

QString jRoster::getToolTip(const QString &jid)
{
	QString toolTip=jid;
	jBuddy *o_contact =  m_roster.value(jid);
	if(!o_contact)
	{
		if(jid.startsWith(m_account_name+"/"))
		{
			QString resource = jid.mid(m_account_name.length()+1);
			if(m_my_connections->resourceExist(resource))
				toolTip=m_my_connections->getToolTip(resource);
		}
	}
	else
		toolTip=o_contact->getToolTip();
	return toolTip;
}

void jRoster::setAvatar(const QString &jid, const QString &hash)
{
	jBuddy *buddy = getBuddy(jid);
	if(buddy)
	{
		TreeModelItem item;
		item.m_protocol_name = "Jabber";
		item.m_account_name = m_account_name;
		item.m_parent_name = buddy->getGroup();
		item.m_item_name = jid;
		item.m_item_type = 0;
		buddy->setAvatarHash(hash);
		emit setItemIcon(item,m_jabber_account->getPathToAvatars()+"/"+hash,1);
		QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "contactlist");
		//if(settings.value(jid+"/iconhash","").toString()!=hash)
		settings.setValue(jid+"/iconhash",hash);
	}
}

void jRoster::setStatusRow(const QString &jid)
{
	jBuddy *buddy = getBuddy(jid);
	if(!buddy)
		return;

	if (!m_show_message_status)
		return;

	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = jid;
	contact.m_parent_name = buddy->getGroup();
	contact.m_item_type = 0;
	jBuddy::ResourceInfo *info = buddy->getResourceInfo(buddy->getMaxPriorityResource());

	QString status = (info) ? info->m_status_message : buddy->m_offline_message;
	if(status.isEmpty())
		status = "";
	else
		status = "     "+status;

	emit setItemCustomText(contact, status, 1);
}

void jRoster::customNotification(const QString &item_name, const QString &message)
{
	jBuddy *buddy = getBuddy(item_name);
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	if(buddy)
		contact.m_parent_name = buddy->getGroup();
	contact.m_item_name = item_name;
	contact.m_item_type = 0;
	emit s_customNotification(contact,message);
}

void jRoster::itemContextMenu(const QList<QAction*> &action_list, const QString &contact_name, int item_type, const QPoint &menu_point)
{
	if(item_type!=0)
		return;
	m_menu_type = item_type;
	m_menu_name = contact_name;
	QString name = contact_name;
	QMenu menu;
	QMenu *resources_menu = 0;
	QMenu *ft_menu = 0;
	QMenu *idles_menu = 0;
	QMenu *adhoc_menu = 0;
	QMenu *pubsub_info_menu = 0;
	menu.setTitle(name);
	menu.addAction(m_menu_title);
	if(m_menu_title.isNull())
		m_menu_title = new QWidgetAction(this);
	if(m_menu_label.isNull())
	{
		m_menu_label = new QLabel;
		m_menu_label->setAlignment(Qt::AlignCenter);
		m_menu_title->setDefaultWidget(m_menu_label);
	}
	m_menu_label->setText("<b>"+ name +"</b>");
	// Default actions at menu
	QString node = jProtocol::getBare(m_menu_name);
	QString resource = jProtocol::getResource(m_menu_name);
	jBuddy *buddy = (node == m_account_name) ? m_my_connections : m_roster.value(node);
	bool is_transport = !node.contains("@");
	bool is_self = (node == m_account_name) && m_menu_name.contains("/");
	bool connected = m_jabber_protocol->getClient()->state()==StateConnected;
	if(action_list.size()>0)
	{
		menu.addAction(action_list[0]);
		QAction *copy_jid = new QAction(m_plugin_system.getIcon("copy_uin"),tr("Copy JID to clipboard"),this);
		copy_jid->setData(node);
		connect(copy_jid, SIGNAL(triggered()), this, SLOT(copyJIDToClipboard()));
		menu.addAction(copy_jid);
		if(buddy && !is_self)
		{
			int c=0;

			if(!buddy->getName().isEmpty())
				m_menu_label->setText("<b>"+ buddy->getName() +"</b>");
			resources_menu = menu.addMenu(m_plugin_system.getIcon("message"),tr("Send message to:"));
			ft_menu = menu.addMenu(m_plugin_system.getIcon("save_all"),tr("Send file to:"));
			idles_menu = menu.addMenu(m_plugin_system.getIcon("idle"),tr("Get idle from:"));
			adhoc_menu = menu.addMenu(m_plugin_system.getIcon("command"),tr("Execute command:"));
			QStringList resources = buddy->getResources();
			foreach(QString resource, resources)
			{
				Presence::PresenceType presence = buddy->getResourceInfo(resource)->m_presence;
				if(presence!=Presence::Unavailable && !resource.isEmpty())
				{
					QAction *message_action = new QAction(jPluginSystem::instance().getStatusIcon(presence),resource,this);
					QAction *ft_action = new QAction(jPluginSystem::instance().getStatusIcon(presence),resource,this);
					QAction *idle_action = new QAction(jPluginSystem::instance().getStatusIcon(presence),resource,this);
					connect(message_action,SIGNAL(triggered()),this,SLOT(onSendMessage()));
					connect(ft_action,SIGNAL(triggered()),this,SLOT(onSendFile()));
					connect(idle_action,SIGNAL(triggered()),this,SLOT(onGetIdle()));
					message_action->setData(resource);
					ft_action->setData(resource);
					idle_action->setData(resource);
					resources_menu->addAction(message_action);
					ft_menu->addAction(ft_action);
					idles_menu->addAction(idle_action);

					jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
					if(info && info->m_features.contains("http://jabber.org/protocol/commands"))
					{
					    QAction *adhoc_action = new QAction(jPluginSystem::instance().getStatusIcon(presence),resource,this);
					    connect(adhoc_action,SIGNAL(triggered()),this,SLOT(onAdhocAction()));
					    adhoc_action->setData(resource);
					    adhoc_menu->addAction(adhoc_action);
					}

					c++;
				}
			}
			if(!c)
			{
				delete resources_menu;
				delete ft_menu;
				delete idles_menu;
				resources_menu = 0;
				ft_menu = 0;
				idles_menu = 0;
			}
			if(adhoc_menu->actions().size()==0)
			{
			    delete adhoc_menu;
			    adhoc_menu = 0;
			}

			jConference *conference_manager = m_jabber_account->getConferenceManagementObject();
			QStringList conferences = conference_manager->getConferenceList();
			if (!conferences.isEmpty())
			{
				QMenu *invite_menu = 0;
				invite_menu = menu.addMenu(m_plugin_system.getIcon("chat"), tr("Invite to conference:"));
				foreach(QString conference, conferences)
				{
					QAction *conference_action = new QAction(conference,this);
					connect(conference_action,SIGNAL(triggered()),conference_manager,SLOT(inviteUser()));
					conference_action->setData(node);
					invite_menu->addAction(conference_action);
				}
			}
		}
	}
	if (is_self)
	{
		QAction *ft_action = new QAction(m_plugin_system.getIcon("save_all"),tr("Send file"),this);
		QAction *idle_action = new QAction(m_plugin_system.getIcon("idle"),tr("Get idle"),this);
		connect(ft_action,SIGNAL(triggered()),this,SLOT(onSendFile()));
		connect(idle_action,SIGNAL(triggered()),this,SLOT(onGetIdle()));
		ft_action->setData(resource);
		idle_action->setData(resource);
		menu.addAction(ft_action);
		menu.addAction(idle_action);
		jBuddy::ResourceInfo *info = m_my_connections->getResourceInfo(resource);
		if(info && info->m_features.contains("http://jabber.org/protocol/commands"))
		{
		    QAction *adhoc_action = new QAction(m_plugin_system.getIcon("command"),tr("Execute command"),this);
		    connect(adhoc_action,SIGNAL(triggered()),this,SLOT(onAdhocAction()));
		    adhoc_action->setData(resource);
		    menu.addAction(adhoc_action);
		}
	}

	if(connected)
	{
		pubsub_info_menu = menu.addMenu(m_plugin_system.getIcon("pubsub"),tr("PubSub info:"));
		QStringList pubsub_keys = buddy->getPubsubKeys();
		foreach(QString pubsub_name, pubsub_keys)
		{
			QList<QVariant> pubsub_info = buddy->getPubsubInfo(pubsub_name);
			int pubsub_count = pubsub_info.value(0).toInt();
			QAction *pubsub_info_action = new QAction(jPluginSystem::instance().getIcon(pubsub_info.value(pubsub_count+2).toString()),utils::getPubsubName(pubsub_name),this);
			connect(pubsub_info_action,SIGNAL(triggered()),this,SLOT(onPubsubInfo()));
			pubsub_info_action->setData(pubsub_name);
			if (pubsub_info.value(pubsub_count+4).toBool())
				pubsub_info_menu->addAction(pubsub_info_action);
		}
		if(pubsub_info_menu->actions().size()==0)
		{
			delete pubsub_info_menu;
			pubsub_info_menu = 0;
		}
	}

	for(int i=1;i<3 && i<action_list.size();i++)
		menu.addAction(action_list[i]);
	if(item_type==0 && buddy && !is_self)
	{
		menu.addSeparator();
		bool nil_group = buddy->getGroup().isEmpty();
		if (connected)
		{
			m_add_contact_action->setEnabled(connected);
		m_rename_action->setEnabled(connected);
		m_delete_action->setEnabled(connected);
		m_move_action->setEnabled(connected);
		m_subscription_menu->menuAction()->setEnabled(connected);
		menu.addAction(m_rename_action);
		menu.addAction(m_delete_action);
		if(!nil_group&&!is_transport)
			menu.addAction(m_move_action);
		else
			menu.addAction(m_add_contact_action);
		menu.addMenu(m_subscription_menu);
		menu.addSeparator();
			if (m_jabber_account->isPrivacyExist())
			{
				QAction *action;
				if (m_roster_visible.contains(node))
				{
					action = new QAction(m_plugin_system.getIcon("visible"), tr("Delete from visible list"), this);
					connect(action, SIGNAL(triggered()), this, SLOT(deleteFromVisibleList()));
				}
				else
				{
					action = new QAction(m_plugin_system.getIcon("visible"), tr("Add to visible list"), this);
					connect(action, SIGNAL(triggered()), this, SLOT(addToVisibleList()));
				}
				action->setData(node);
				menu.addAction(action);
				if (m_roster_invisible.contains(node))
				{
					action = new QAction(m_plugin_system.getIcon("privacy"), tr("Delete from invisible list"), this);
					connect(action, SIGNAL(triggered()), this, SLOT(deleteFromInvisibleList()));
				}
				else
				{
					action = new QAction(m_plugin_system.getIcon("privacy"), tr("Add to invisible list"), this);
					connect(action, SIGNAL(triggered()), this, SLOT(addToInvisibleList()));
				}
				action->setData(node);
				menu.addAction(action);
				if (m_roster_ignore.contains(node))
				{
					action = new QAction(m_plugin_system.getIcon("ignorelist"), tr("Delete from ignore list"), this);
					connect(action, SIGNAL(triggered()), this, SLOT(deleteFromIgnoreList()));
				}
				else
				{
					action = new QAction(m_plugin_system.getIcon("ignorelist"), tr("Add to ignore list"), this);
					connect(action, SIGNAL(triggered()), this, SLOT(addToIgnoreList()));
				}
				action->setData(node);
				menu.addAction(action);
			}
		}
	}

	if(is_transport)
	{
	    m_transports_menu->setEnabled(connected);
	    menu.addMenu(m_transports_menu);
	}
	if(action_list.size()>3)
		menu.addSeparator();
	// Actions at menu from plugins
	for(int i=3;i<action_list.size();i++)
		menu.addAction(action_list[i]);

	menu.exec(menu_point);
	if(resources_menu)
		delete resources_menu;
	if(ft_menu)
		delete ft_menu;
	if(idles_menu)
		delete idles_menu;
	if(adhoc_menu)
	    delete adhoc_menu;
	if(pubsub_info_menu)
		delete pubsub_info_menu;
}

void jRoster::setBirthday(const JID &jid, QDate &birthday)
{
	qDebug() << "Yahoo! Account " << utils::fromStd(jid.bare()) << " birthday " << birthday.toString("d MMMM yyyy");
}

void jRoster::chatWindowOpened(const QString &item_name)
{
	jBuddy *buddy = (jProtocol::getBare(item_name) == m_account_name) ? m_my_connections : m_roster.value(item_name);
	m_chat_windows.append(item_name);

	if(!buddy)
		return;
	QString resource = jProtocol::getResource(item_name);
	if (!buddy->resourceExist(resource))
		resource = buddy->getMaxPriorityResource();
	if (!buddy->resourceExist(resource))
		return;
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_name = item_name;
	contact.m_parent_name = buddy->getGroup();
	contact.m_item_type = 0;
	jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
	QString status = info->m_status_message;
	status.replace("<br/>", " | ");
	if(!status.isEmpty())
		emit addServiceMessage(contact, status);
}

void jRoster::chatWindowAboutToBeOpened(const QString &item_name)
{
	QString resource = jProtocol::getResource(item_name);
	QString bare = jProtocol::getBare(item_name);
	jConference *conference_manager = m_jabber_account->getConferenceManagementObject();
	if(conference_manager->JIDIsRoom(bare))
	{
		conference_manager->chatWindowAboutToBeOpened(bare, resource);
	}
	else
	{
		if (!m_roster.contains(bare))
			addContact(bare, "", "");
		if(resource.isEmpty())
			return;

		jBuddy *buddy = (bare == m_account_name) ? m_my_connections : m_roster.value(bare);
		if(!buddy->resourceExist(resource, true))
			buddy->addResource(resource, -128, Presence::Unavailable);
		jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
		if(info->m_in_cl)
			return;
		info->m_in_cl = true;

		TreeModelItem contact;
		contact.m_protocol_name = "Jabber";
		contact.m_account_name = m_account_name;
		contact.m_item_name = item_name;
		contact.m_parent_name = buddy->getGroup();
		contact.m_item_history = bare;
		contact.m_item_type = 0;

		if (bare != m_account_name)
			addItem(item_name, (buddy->getName().isEmpty())?item_name+"/"+resource:buddy->getName()+"/"+resource, buddy->getGroup(), bare, info->m_presence, true);
		setClient(bare, resource, info->m_client_name, false);
	}
}

void jRoster::chatWindowClosed(const QString &item_name)
{
	QString resource = jProtocol::getResource(item_name);
	QString bare = jProtocol::getBare(item_name);

	jConference *conference_manager = m_jabber_account->getConferenceManagementObject();
	if(conference_manager->JIDIsRoom(bare))
	{
		conference_manager->chatWindowClosed(bare, resource);
	}
	else
	{
		m_chat_windows.removeAll(item_name);

		if(!m_roster.contains(bare))
			return;
		if(resource.isEmpty())
			return;
		jBuddy *buddy = (bare == m_account_name) ? m_my_connections : m_roster.value(bare);
		if(!buddy->resourceExist(resource, true))
			return;
		else
		{
			jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
			if(!info->m_in_cl)
				return;
			info->m_in_cl = false;
			if(info->m_presence == Presence::Unavailable)
				buddy->delResource(resource);
//			if (bare != m_account_name)
//				delItem(item_name, buddy->getGroup(), true);
		}
	}
}

QStringList jRoster::getGroups()
{
	QStringList groups = m_groups;
	groups.removeOne("My connections");
	groups.removeOne("");
	return groups;
}

void jRoster::onRenameAction()
{
	QString node = jProtocol::getBare(m_menu_name);
	jBuddy *buddy = m_roster.value(node, 0);
	JID jid(utils::toStd(m_menu_name));
	bool ok;
	QString name = QInputDialog::getText(0, tr("Rename contact"),tr("Name:"), QLineEdit::Normal, buddy?buddy->getName():m_menu_name , &ok);
	if(ok && buddy && buddy->getGroup().isEmpty())
	{
		renameContact(node, name);
		return;
	}
	if (ok )
	{
		RosterItem *item = m_roster_manager->getRosterItem(jid);
		if(item)
		{
			item->setName(utils::toStd(name));
			m_roster_manager->synchronize();
		}
	}
}

void jRoster::onDeleteAction()
{
	QString node = jProtocol::getBare(m_menu_name);
	jBuddy *buddy = m_roster.value(node,0);
	if(buddy && buddy->getGroup().isEmpty())
	{
		removeContact(node);
		return;
	}
	QMessageBox msg_box;
	msg_box.setWindowTitle(m_menu_name);
	if(buddy && buddy->getGroup() == tr("Services"))
	{
	    msg_box.setText(tr("Remove transport and his contacts?"));
	    QPushButton *withContacts = msg_box.addButton(tr("Delete with contacts"), QMessageBox::ActionRole);
	    QPushButton *withoutContacts = msg_box.addButton(tr("Delete without contacts") ,QMessageBox::ActionRole);
	    QPushButton *Cancel = msg_box.addButton(tr("Cancel") ,QMessageBox::RejectRole);
	    msg_box.exec();
	    if (msg_box.clickedButton() == Cancel) return;
	    if (msg_box.clickedButton() == withContacts) {
		for (int i = 0; i < m_buddies.size(); ++i){
		    if(m_buddies.at(i).endsWith(m_menu_name)){
			JID jid(utils::toStd(m_buddies.at(i)));
			m_roster_manager->remove(jid);
		    }
		}
	    } else if (msg_box.clickedButton() == withoutContacts) {
		JID jid(utils::toStd(m_menu_name));
		m_roster_manager->remove(jid);
	    }
	} else {
	    msg_box.setText(tr("Contact will be deleted. Are you sure?"));
	    msg_box.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
	    msg_box.setDefaultButton(QMessageBox::No);
	    if(msg_box.exec() == QMessageBox::Yes)
	    {
		    JID jid(utils::toStd(m_menu_name));
		    m_roster_manager->remove(jid);
	    }
	}
}

void jRoster::onMoveAction()
{
	JID jid(utils::toStd(m_menu_name));
	jBuddy *buddy = m_roster.value(utils::fromStd(jid.bare()),0);
	QString current = "General";
	if(buddy)
		current = buddy->getGroup();
	if (current==tr("Services")) return;
	bool ok;
	QStringList m_groups_without_services=m_groups;
	m_groups_without_services.removeAll(tr("Services"));
	QString group = QInputDialog::getItem(0, tr("Move %1").arg(m_menu_name), tr("Group:"), m_groups_without_services, m_groups_without_services.indexOf(current), true, &ok);
	if (group==tr("Services")) return;
	if( ok )
	{
		RosterItem *item = m_roster_manager->getRosterItem(jid);
		if(item)
		{
			StringList groups;
			groups.push_back(utils::toStd(group));
			item->setGroups(groups);
			m_roster_manager->synchronize();
		}
	}
}

void jRoster::onSendSubscriptionAction()
{
	QMessageBox msg_box;
	msg_box.setWindowTitle(m_menu_name);
	msg_box.setText(tr("Authorize contact?"));
	msg_box.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
	msg_box.setDefaultButton(QMessageBox::No);
	if(msg_box.exec() == QMessageBox::Yes)
	{
		JID jid(utils::toStd(m_menu_name));
		Subscription s( Subscription::Subscribed, jid.bareJID() );
		m_jabber_protocol->getClient()->send( s );
	}
}

void jRoster::onAskSubscriptionAction()
{
	JID jid(utils::toStd(m_menu_name));
	jBuddy *buddy = m_roster.value(utils::fromStd(jid.bare()),0);
	bool ok;
	QString reason = QInputDialog::getText(0, tr("Ask authorization from %1").arg(m_menu_name),tr("Reason:"), QLineEdit::Normal, buddy?buddy->getName():m_menu_name , &ok);
	if (ok )
	{
		Subscription s( Subscription::Subscribe, jid.bareJID(), utils::toStd(reason) );
		m_jabber_protocol->getClient()->send( s );
	}
}

void jRoster::onRemoveSubscriptionAction()
{
	JID jid(utils::toStd(m_menu_name));
	jBuddy *buddy = m_roster.value(utils::fromStd(jid.bare()),0);
	bool ok;
	QString reason = QInputDialog::getText(0, tr("Remove authorization from %1").arg(m_menu_name),tr("Reason:"), QLineEdit::Normal, buddy?buddy->getName():m_menu_name , &ok);
	if (ok)
	{
		m_roster_manager->cancel(jid, utils::toStd(reason));
	}
}

void jRoster::onSendMessage()
{
	QAction *action = qobject_cast<QAction *>(sender());
	TreeModelItem item;
	item.m_protocol_name = "Jabber";
	item.m_account_name = m_account_name;
	item.m_item_name = jProtocol::getBare(m_menu_name) + "/" + action->data().toString();
	if(m_menu_name != m_account_name)
	{
		jBuddy *buddy = m_roster.value(jProtocol::getBare(m_menu_name), 0);
		if(buddy)
			item.m_parent_name = buddy->getGroup();
	}
	item.m_item_type = 0;
	jPluginSystem::instance().createChat(item);
}

void jRoster::onSendFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QFileDialog dialog(0,QObject::tr("Open File"),"",QObject::tr("All files (*)"));
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setAttribute(Qt::WA_QuitOnClose, false);
    QStringList file_names;
    if ( !dialog.exec() )
	return;
    file_names = dialog.selectedFiles();
    m_jabber_protocol->getFileTransfer()->sendFileTo(jProtocol::getBare(m_menu_name)+"/"+action->data().toString(), file_names);
}

void jRoster::onAdhocAction()
{
    QAction *action = qobject_cast<QAction *>(sender());
    m_jabber_account->adhoc(jProtocol::getBare(m_menu_name) + "/" + action->data().toString());
}

void jRoster::onGetIdle()
{
	QAction *action = qobject_cast<QAction *>(sender());
	JID jid(utils::toStd(jProtocol::getBare(m_menu_name) + "/" + action->data().toString()));
	m_jabber_protocol->getLastActivityObj()->query(jid);
}

void jRoster::onPubsubInfo()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString jid = jProtocol::getBare(m_menu_name);
	jBuddy *buddy = (jid == m_account_name) ? m_my_connections : m_roster.value(jid,0);
	if (buddy)
	{
		jPubsubInfo *dialog = new jPubsubInfo(action->data().toString(), buddy->getPubsubInfo(action->data().toString()));
		dialog->show();
	}
}

void jRoster::onAddContact()
{
    QString jid = jProtocol::getBare(m_menu_name);
    jBuddy *buddy = m_roster.value(jid,0);
    if(buddy)
	m_jabber_account->showAddDialog(jid, buddy->getName());
    else
	m_jabber_account->showAddDialog();
}


void jRoster::onRegisterAction()
{
    QString jid = jProtocol::getBare(m_menu_name);
    m_jabber_protocol->showTransportReg(jid);
}

void jRoster::onUnregisterAction()
{
    QString jid = jProtocol::getBare(m_menu_name);
    m_jabber_protocol->removeTransportReg(jid);
}

void jRoster::onLogInTransportAction()
{
    QString jid = jProtocol::getBare(m_menu_name);
    m_jabber_protocol->LogInTransport(jid);
}

void jRoster::onLogOutTransportAction()
{
    QString jid = jProtocol::getBare(m_menu_name);
    m_jabber_protocol->LogOutTransport(jid);
}

void jRoster::updateIcon(const QString &jid, const QString &name)
{
	jBuddy *buddy = (jid == m_account_name) ? m_my_connections : m_roster.value(jid, 0);
	if(!buddy)
		return;

	TreeModelItem item;
	item.m_protocol_name = "Jabber";
	item.m_account_name = m_account_name;
	item.m_item_name = jid;
	item.m_parent_name = buddy->getGroup();
	item.m_item_type = 0;

	const QList<QVariant> &list = buddy->getPubsubInfo(name);
	int c = list.value(0).toInt();
	int pos = list.value(c+1).toInt();
	QIcon icon = Icon(list.value(c+2).toString());
	switch(pos)
	{
		case 3:
			if (!m_show_tune)
				icon = QIcon();
			break;
		case 4:
			if (!m_show_activity)
				icon = QIcon();
		case 5:
			if (!m_show_mood)
				icon = QIcon();
			break;
		default:
			return;
	}
	updateItemIcon(item, icon, pos);
}

void jRoster::updateXIcon(const QString &jid, const QString &name)
{
	jBuddy *buddy = (jid == m_account_name) ? m_my_connections : m_roster.value(jid, 0);
	if(!buddy)
		return;

	TreeModelItem item;
	item.m_protocol_name = "Jabber";
	item.m_account_name = m_account_name;
	item.m_item_name = jid;
	item.m_parent_name = buddy->getGroup();
	item.m_item_type = 0;

	int pos;
	QIcon icon;
	if (name == "presence" && m_show_xpresence)
	{
		pos = 6;
		if (buddy->getCountResources())
		{
			if(jBuddy::ResourceInfo *info = buddy->getResourceInfo(buddy->getMaxPriorityResource()))
				icon = m_plugin_system.getStatusIcon(info->m_x_presence, "icq");
			else
				icon = QIcon();
		}
	}
	else if (name == "status" && m_show_xstatus)
	{
		pos = 7;
		if (buddy->getCountResources())
		{
			if(jBuddy::ResourceInfo *info = buddy->getResourceInfo(buddy->getMaxPriorityResource()))
				icon = m_plugin_system.getIcon(info->m_x_status);
			else
				icon = QIcon();
		}
	}
	else
		return;

	updateItemIcon(item, icon, pos);
}

void jRoster::updateItemIcon(const TreeModelItem &item, const QIcon &icon, const int &pos)
{
	if (item.m_item_name != m_account_name)
		m_plugin_system.setContactItemIcon(item, icon, pos);
	else
	{
		TreeModelItem tItem = item;
		jBuddy *buddy = m_my_connections;
		QStringList resources = buddy->getResources();
		for(int i=0; i<resources.size(); i++)
		{
			tItem.m_item_name = m_account_name+"/"+resources[i];
			m_plugin_system.setContactItemIcon(tItem, icon, pos);
		}
	}
}

void jRoster::copyJIDToClipboard()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString room = action->data().toString();
		QApplication::clipboard()->setText(room);
	}
}

void jRoster::addToVisibleList()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString jid = action->data().toString();
		m_roster_visible << jid;
		if (m_roster_invisible.contains(jid))
		{
			m_roster_invisible.removeOne(jid);
			emit modifyPrivacyList("invisible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketPresenceOut, utils::toStd(jid)), false);
		}
		emit modifyPrivacyList("visible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionAllow, PrivacyItem::PacketPresenceOut, utils::toStd(jid)), true);
	}
}

void jRoster::deleteFromVisibleList()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString jid = action->data().toString();
		m_roster_visible.removeOne(jid);
		emit modifyPrivacyList("visible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionAllow, PrivacyItem::PacketPresenceOut, utils::toStd(jid)), false);
	}
}

void jRoster::addToInvisibleList()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString jid = action->data().toString();
		m_roster_invisible << jid;
		if (m_roster_visible.contains(jid))
		{
			m_roster_visible.removeOne(jid);
			emit modifyPrivacyList("visible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionAllow, PrivacyItem::PacketPresenceOut, utils::toStd(jid)), false);
		}
		emit modifyPrivacyList("invisible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketPresenceOut, utils::toStd(jid)), true);
	}
}

void jRoster::deleteFromInvisibleList()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString jid = action->data().toString();
		m_roster_invisible.removeOne(jid);
		emit modifyPrivacyList("invisible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketPresenceOut, utils::toStd(jid)), false);
	}
}

void jRoster::addToIgnoreList()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString jid = action->data().toString();
		m_roster_ignore << jid;
		emit modifyPrivacyList("visible", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketMessage, utils::toStd(jid)), true);
		emit modifyPrivacyList("invisible", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketMessage, utils::toStd(jid)), true);
		emit modifyPrivacyList("invisible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketMessage, utils::toStd(jid)), true);
		emit modifyPrivacyList("visible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketMessage, utils::toStd(jid)), true);
	}
}

void jRoster::deleteFromIgnoreList()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString jid = action->data().toString();
		m_roster_ignore.removeOne(jid);
		emit modifyPrivacyList("visible", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketMessage, utils::toStd(jid)), false);
		emit modifyPrivacyList("invisible", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketMessage, utils::toStd(jid)), false);
		emit modifyPrivacyList("invisible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketMessage, utils::toStd(jid)), false);
		emit modifyPrivacyList("visible list", PrivacyItem(PrivacyItem::TypeJid, PrivacyItem::ActionDeny, PrivacyItem::PacketMessage, utils::toStd(jid)), false);
	}
}
