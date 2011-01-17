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

#ifndef MSNCONTACTLIST_H
#define MSNCONTACTLIST_H
#include "msnpluginsystem.h"
#include <QObject>
#include "msnprotocolwrapper.h"
#include "msngroup.h"
#include "msnbuddy.h"


class MSNContactList : public QObject
{
    Q_OBJECT
public:
    MSNContactList(const QString &account_name,const QString &profile_name,
		     MSNProtocolWrapper *protocol_wrapper, QObject *parent = 0);
    ~MSNContactList();
    void removeContactList();
    void validateBuddy(const QString &buddy_id, const QString &buddy_friendly_name);
    QString getBuddGroup(const QString &buddy_id);
private slots:
    void groupsArriving(std::map<std::string,MSN::Group> &groups);
    void buddiesArriving(std::map<std::string,MSN::Buddy*> &buddies);
    void buddyChangeStatus(const QString &buddy_id,const QString &friendly_name, MSN::BuddyStatus status);
    void buddyOffline(const QString &buddy_id);
    void iAmDisconnected();
private:
    void createGroup(const QString &group_name, const QString &group_id);
    void renameGroup(const QString &group_id, const QString group_name);
    void removeGroup(const QString &group_id);
    void loadGroups();
    void loadBuddies();
    void createContact(const QString &contact_id, const QString &contact_name,
		       const QString &group_id);
    void moveContact(const QString &contact_id, const QString &old_group_id,
		     const QString &new_group_id);
    void setContactName(const QString &buddy_id, const QString &name);

    QString m_account_name;
    QString m_profile_name;
    MSNProtocolWrapper *m_protocol_wrapper;
    MSNPluginSystem &m_ps;
    QHash<QString,MSNGroup> m_group_list;
    QString m_global_group_id;
    QHash<QString,MSNBuddy> m_buddies_list;

    QIcon *m_online_icon;
    QIcon *m_offline_icon;
    QIcon *m_connecting_icon;
    QIcon *m_busy_icon;
    QIcon *m_idle_icon;
    QIcon *m_rightback_icon;
    QIcon *m_away_icon;
    QIcon *m_onphone_icon;
    QIcon *m_lunch_icon;
    QIcon *m_invisible_icon;
};

#endif // MSNCONTACTLIST_H
