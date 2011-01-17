/*
    MSNContactList

    Copyright (c) 2009 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "msncontactlist.h"
#include "msnstatusicons.h"

MSNContactList::MSNContactList(const QString &account_name,const QString &profile_name,
		     MSNProtocolWrapper *protocol_wrapper, QObject *parent)
		    :QObject(parent),
		    m_account_name(account_name),
		    m_profile_name(profile_name),
		    m_protocol_wrapper(protocol_wrapper),
		    m_ps(MSNPluginSystem::instance())

{
    connect(m_protocol_wrapper, SIGNAL(groupsArriving(std::map<std::string,MSN::Group> &)),
	    this, SLOT(groupsArriving(std::map<std::string,MSN::Group> &)));
    connect(m_protocol_wrapper, SIGNAL(buddiesArriving(std::map<std::string,MSN::Buddy*>&)),
	    this, SLOT(buddiesArriving(std::map<std::string,MSN::Buddy*>&)));
    connect(m_protocol_wrapper, SIGNAL(buddyChangeStatus(const QString &,const QString &,MSN::BuddyStatus)),
	    this, SLOT(buddyChangeStatus(const QString &,const QString &,MSN::BuddyStatus)));
    connect(m_protocol_wrapper, SIGNAL(buddyOffline(const QString&)),
	    this, SLOT(buddyOffline(const QString &)));
    connect(m_protocol_wrapper, SIGNAL(iAmDisconnected()),
	    this, SLOT(iAmDisconnected()));

    TreeModelItem root_item;
    root_item.m_protocol_name = "MSN";
    root_item.m_account_name = m_account_name;
    root_item.m_item_name = m_account_name;
    root_item.m_item_type = 2;
    m_ps.addItemToContactList(root_item, m_account_name);
    m_global_group_id = "msnDead";

    MSNStatusIcons &msi = MSNStatusIcons::instance();

    m_online_icon = &msi.m_online_icon;

    m_offline_icon = &msi.m_offline_icon;

    m_connecting_icon = &msi.m_connecting_icon;

    m_busy_icon = &msi.m_busy_icon;

    m_idle_icon = &msi.m_idle_icon;

    m_rightback_icon = &msi.m_rightback_icon;

    m_away_icon = &msi.m_away_icon;

    m_onphone_icon = &msi.m_onphone_icon;

    m_lunch_icon = &msi.m_lunch_icon;

    m_invisible_icon = &msi.m_invisible_icon;

    loadGroups();
    loadBuddies();
}

MSNContactList::~MSNContactList()
{

}

void MSNContactList::groupsArriving(std::map<std::string,MSN::Group> &groups)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "contactlist");
    QStringList groups_to_remove = m_group_list.keys();

    std::map < std::string, MSN::Group >::iterator g = groups.begin();
    for (; g != groups.end (); g++)
    {
	QString group_id((*g).second.groupID.c_str());
	QString group_name((*g).second.name.c_str ());
	if( !m_group_list.contains(group_id) )
	{
	    settings.beginGroup(group_id);
	    settings.setValue("id",group_id);
	    settings.setValue("name",group_name);
	    settings.endGroup();

	    m_group_list.insert(group_id,MSNGroup(group_id,group_name));

	    createGroup(group_name,group_id);
	} else {
	    groups_to_remove.removeAll(group_id);
	    MSNGroup &temp_group = m_group_list[group_id];
	    if(group_name != temp_group.m_name)
	    {
		settings.setValue(group_id+"/name",group_name);
		renameGroup(group_id, group_name);
		temp_group.m_name = group_name;
	    }
	}

    }
    foreach(QString remove_id, groups_to_remove)
    {
	if(remove_id != m_global_group_id)
	{
	    settings.remove(remove_id);
	    m_group_list.remove(remove_id);
	    removeGroup(remove_id);
	}
    }

    QStringList group_list = m_group_list.keys();
    group_list.removeAll(m_global_group_id);
    settings.setValue("list/groups",group_list);
}

void MSNContactList::buddiesArriving(std::map<std::string,MSN::Buddy*> &buddies)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "contactlist");
    QStringList buddies_to_remove = m_buddies_list.keys();

    std::map < std::string, MSN::Buddy * >::iterator i =
	buddies.begin ();

    for (; i != buddies.end (); ++i)
    {
	MSN::Buddy * contact = (*i).second;

	QString buddy_id(contact->userName.c_str());
	QString buddy_friendly_name(contact->friendlyName.c_str());
	QString buddy_group_id(contact->groups.size()?
			       contact->groups.front()->groupID.c_str():
			       m_global_group_id);
	buddy_group_id = m_group_list.contains(buddy_group_id)?buddy_group_id:m_global_group_id;

	if (contact->lists & MSN::LST_AB       // only if it is the address book
	   && contact->properties["isMessengerUser"] == "true")
	{

	    if( !m_buddies_list.contains(buddy_id) )
	    {
		settings.beginGroup(buddy_id);
		settings.setValue("id",buddy_id);
		settings.setValue("name",buddy_friendly_name);
		settings.setValue("group",buddy_group_id);
		settings.endGroup();

		MSNBuddy new_buddy(buddy_id,buddy_group_id);
		new_buddy.m_friendly_name = buddy_friendly_name;
		m_buddies_list.insert(buddy_id,new_buddy);

		createContact(buddy_id,buddy_friendly_name,buddy_group_id);
	    } else {
		buddies_to_remove.removeAll(buddy_id);
		MSNBuddy &temp_buddy = m_buddies_list[buddy_id];
		if(temp_buddy.m_group_id != buddy_group_id)
		{
		    settings.setValue(buddy_id+"/group",buddy_group_id);
		    moveContact(buddy_id,temp_buddy.m_group_id,buddy_group_id);
		    temp_buddy.m_group_id = buddy_group_id;

		}
	    }


	    /*allContacts[contact->userName.c_str ()] = 0;
	    allContacts[contact->userName.c_str ()] |= MSN::LST_AB;
	    std::list < MSN::Buddy::PhoneNumber >::iterator pns =
		contact->phoneNumbers.begin ();
	    std::list < MSN::Group * >::iterator g = contact->groups.begin ();*/


	}
	if (contact->lists & MSN::LST_AL)
	{
	    //allContacts[contact->userName.c_str ()] |= MSN::LST_AL;
	}

	if (contact->lists & MSN::LST_BL)
	{
	    //allContacts[contact->userName.c_str ()] |= MSN::LST_BL;
	}

	if (contact->lists & MSN::LST_RL)
	{
	    //printf ("-RL %s \n", contact->userName.c_str ());
	}
	if (contact->lists & MSN::LST_PL)
	{
	    //printf ("-PL %s \n", contact->userName.c_str ());
	}
    }

    foreach(QString remove_id, buddies_to_remove)
    {
	MSNBuddy &temp_buddy = m_buddies_list[remove_id];
	settings.setValue(remove_id+"/group","");
	moveContact(remove_id,temp_buddy.m_group_id,"");
	temp_buddy.m_group_id = "";
    }

    QStringList buddies_list = m_buddies_list.keys();
    settings.setValue("list/buddies",buddies_list);

    TreeModelItem root_item;
    root_item.m_protocol_name = "MSN";
    root_item.m_account_name = m_account_name;
    root_item.m_item_name = m_account_name;
    root_item.m_item_type = 2;
    m_ps.setAccountIsOnline(root_item,true);
}

void MSNContactList::removeContactList()
{
    TreeModelItem root_item;
    root_item.m_protocol_name = "MSN";
    root_item.m_account_name = m_account_name;
    root_item.m_item_name = m_account_name;
    root_item.m_item_type = 2;
    m_ps.removeItemFromContactList(root_item);
}

void MSNContactList::createGroup(const QString &group_name, const QString &group_id) {
    TreeModelItem group_item;
    group_item.m_protocol_name = "MSN";
    group_item.m_account_name = m_account_name;
    group_item.m_item_name = group_id;
    group_item.m_parent_name = m_account_name;
    group_item.m_item_type = 1;
    m_ps.addItemToContactList(group_item, group_name);
}

void MSNContactList::renameGroup(const QString &group_id, const QString group_name)
{
    TreeModelItem group_item;
    group_item.m_protocol_name = "MSN";
    group_item.m_account_name = m_account_name;
    group_item.m_item_name = group_id;
    group_item.m_parent_name = m_account_name;
    group_item.m_item_type = 1;
    m_ps.setContactItemName(group_item, group_name);
}

void MSNContactList::removeGroup(const QString &group_id)
{
    TreeModelItem group_item;
    group_item.m_protocol_name = "MSN";
    group_item.m_account_name = m_account_name;
    group_item.m_item_name = group_id;
    group_item.m_parent_name = m_account_name;
    group_item.m_item_type = 1;
    m_ps.removeItemFromContactList(group_item);
}

void MSNContactList::loadGroups()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "contactlist");
    foreach(QString group_id, settings.value("list/groups",QStringList()).toStringList())
    {
	    QString group_name(settings.value(group_id+"/name","noname").toString());
	    m_group_list.insert(group_id,MSNGroup(group_id,group_name));
	    createGroup(group_name,group_id);
    }

    QString global_group_name(tr("Without group"));
    m_group_list.insert(m_global_group_id,MSNGroup(m_global_group_id,global_group_name));
    createGroup(global_group_name,m_global_group_id);
}

void MSNContactList::loadBuddies()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "contactlist");
    foreach(QString buddy_id, settings.value("list/buddies",QStringList()).toStringList())
    {
	    QString buddy_friendly_name(settings.value(buddy_id+"/name",buddy_id).toString());
	    QString buddy_group_id(settings.value(buddy_id+"/group","").toString());

	    MSNBuddy new_buddy(buddy_id,buddy_group_id);
	    new_buddy.m_friendly_name = buddy_friendly_name;
	    m_buddies_list.insert(buddy_id,new_buddy);

	    createContact(buddy_id,buddy_friendly_name,buddy_group_id);
    }
}


void MSNContactList::createContact(const QString &contact_id, const QString &contact_name,
		       const QString &group_id)
{
    TreeModelItem contact_item;
    contact_item.m_protocol_name = "MSN";
    contact_item.m_account_name = m_account_name;
    contact_item.m_item_name = contact_id;
    contact_item.m_parent_name = group_id;
    contact_item.m_item_type = 0;
    m_ps.addItemToContactList(contact_item, contact_name);

    m_ps.setContactItemStatus(contact_item, *m_offline_icon, "offline",1000);
}


void MSNContactList::moveContact(const QString &contact_id, const QString &old_group_id,
		     const QString &new_group_id)
{
    TreeModelItem old_item;
    old_item.m_protocol_name = "MSN";
    old_item.m_account_name = m_account_name;
    old_item.m_item_name = contact_id;
    old_item.m_parent_name = old_group_id;
    old_item.m_item_type = 0;

    TreeModelItem new_item;
    new_item.m_protocol_name = "MSN";
    new_item.m_account_name = m_account_name;
    new_item.m_item_name = contact_id;
    new_item.m_parent_name = new_group_id;
    new_item.m_item_type = 0;

    m_ps.moveItemInContactList(old_item, new_item);
}

void MSNContactList::setContactName(const QString &buddy_id, const QString &name)
{
    TreeModelItem contact_item;
    contact_item.m_protocol_name = "MSN";
    contact_item.m_account_name = m_account_name;
    contact_item.m_item_name = buddy_id;
    contact_item.m_parent_name = m_buddies_list[buddy_id].m_group_id;
    contact_item.m_item_type = 0;

    m_ps.setContactItemName(contact_item,name);
}

void MSNContactList::buddyChangeStatus(const QString &buddy_id,const QString &friendly_name, MSN::BuddyStatus status)
{
    if(m_buddies_list.contains(buddy_id))
    {
	MSNBuddy &temp_buddy = m_buddies_list[buddy_id];

	TreeModelItem contact_item;
	contact_item.m_protocol_name = "MSN";
	contact_item.m_account_name = m_account_name;
	contact_item.m_item_name = buddy_id;
	contact_item.m_parent_name = temp_buddy.m_group_id;
	contact_item.m_item_type = 0;

	if(temp_buddy.m_friendly_name != friendly_name)
	{
	    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
			       "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "contactlist");
	    settings.setValue(buddy_id+"/name",friendly_name);
	    temp_buddy.m_friendly_name = friendly_name;
	    m_ps.setContactItemName(contact_item,friendly_name);
	}

	QIcon buddy_status_icon(*m_online_icon);
	QString buddy_status_name("online");
	int mass = 0;

	switch(status)
	{
	    case MSN::STATUS_BUSY:
		buddy_status_icon = *m_busy_icon;
		buddy_status_name = "busy";
		mass = 1;
		break;
	    case MSN::STATUS_IDLE:
		buddy_status_icon = *m_idle_icon;
		buddy_status_name = "idle";
		mass = 2;
		break;
	    case MSN::STATUS_BERIGHTBACK:
		buddy_status_icon = *m_rightback_icon;
		buddy_status_name = "rightback";
		mass = 3;
		break;
	    case MSN::STATUS_AWAY:
		buddy_status_icon = *m_away_icon;
		buddy_status_name = "away";
		mass = 4;
		break;
	    case MSN::STATUS_ONTHEPHONE:
		buddy_status_icon = *m_onphone_icon;
		buddy_status_name = "onphone";
		mass = 5;
		break;
	    case MSN::STATUS_OUTTOLUNCH:
		buddy_status_icon = *m_lunch_icon;
		buddy_status_name = "lunch";
		mass = 6;
		break;
	    case MSN::STATUS_INVISIBLE:
		buddy_status_icon = *m_invisible_icon;
		buddy_status_name = "invisible";
		mass = 7;
		break;
	    default:;
	}

	temp_buddy.m_online = true;
	m_ps.setContactItemStatus(contact_item, buddy_status_icon, buddy_status_name, mass);

    }
}

void MSNContactList::buddyOffline(const QString &buddy_id)
{
    if(m_buddies_list.contains(buddy_id))
    {
	MSNBuddy &temp_buddy = m_buddies_list[buddy_id];
	temp_buddy.m_online = false;
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "MSN";
	contact_item.m_account_name = m_account_name;
	contact_item.m_item_name = buddy_id;
	contact_item.m_parent_name = temp_buddy.m_group_id;
	contact_item.m_item_type = 0;
	m_ps.setContactItemStatus(contact_item, *m_offline_icon, "offline", 1000);
    }
}

void MSNContactList::iAmDisconnected()
{
    TreeModelItem root_item;
    root_item.m_protocol_name = "MSN";
    root_item.m_account_name = m_account_name;
    root_item.m_item_name = m_account_name;
    root_item.m_item_type = 2;
    m_ps.setAccountIsOnline(root_item,false);
    QStringList buddies = m_buddies_list.keys();

    foreach(QString buddy_id, buddies)
    {
	MSNBuddy &temp_buddy = m_buddies_list[buddy_id];
	if(temp_buddy.m_online)
	{
	    temp_buddy.m_online = false;

	    TreeModelItem contact_item;
	    contact_item.m_protocol_name = "MSN";
	    contact_item.m_account_name = m_account_name;
	    contact_item.m_item_name = buddy_id;
	    contact_item.m_parent_name = temp_buddy.m_group_id;
	    contact_item.m_item_type = 0;
	    m_ps.setContactItemStatus(contact_item, *m_offline_icon, "offline", 1000);
	}
    }
}

void MSNContactList::validateBuddy(const QString &buddy_id,const QString &buddy_friendly_name)
{
    if(!m_buddies_list.contains(buddy_id))
    {
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "contactlist");
	settings.beginGroup(buddy_id);
	settings.setValue("id",buddy_id);
	settings.setValue("name",buddy_friendly_name);
	settings.setValue("group","");
	settings.endGroup();

	MSNBuddy new_buddy(buddy_id,"");
	new_buddy.m_friendly_name = buddy_friendly_name;
	m_buddies_list.insert(buddy_id,new_buddy);

	createContact(buddy_id,buddy_friendly_name,"");
    }
}

QString MSNContactList::getBuddGroup(const QString &buddy_id)
{
    if(!m_buddies_list.contains(buddy_id))
	return QString();
    else
	return m_buddies_list[buddy_id].m_group_id;
}
