/*
    MSNMessaging

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


#include "msnmessaging.h"
#include "msncontactlist.h"
MSNMessaging::MSNMessaging(const QString &profile_name, const QString &account_name,
		 MSNProtocolWrapper *protocol_wrapper, MSNContactList *contact_list,
		 QObject *parent)
    :m_profile_name(profile_name),
    m_account_name(account_name),
    m_protocol_wrapper(protocol_wrapper),
    m_contact_list(contact_list),
    m_ps(MSNPluginSystem::instance())

{
    connect(m_protocol_wrapper, SIGNAL(messageArrived(const QString &,const QString &,
						      const QString &, const QDateTime &, MSN::SwitchboardServerConnection *)),
	    this, SLOT(messageArrived(const QString &, const QString &,
				      const QString &, const QDateTime &, MSN::SwitchboardServerConnection *)));

    connect(m_protocol_wrapper, SIGNAL(switchBoardCreated(const QString &,MSN::SwitchboardServerConnection*)),
	    this, SLOT(switchBoardCreated(const QString &,MSN::SwitchboardServerConnection*)));
}

void MSNMessaging::messageArrived(const QString &buddy_id,const QString &buddy_friendly_name,
				  const QString &message,const QDateTime &date,
				  MSN::SwitchboardServerConnection *conn)
{
    m_contact_list->validateBuddy(buddy_id, buddy_friendly_name);

    TreeModelItem contact_item;
    contact_item.m_protocol_name = "MSN";
    contact_item.m_account_name = m_account_name;
    contact_item.m_item_name = buddy_id;
    contact_item.m_parent_name = m_contact_list->getBuddGroup(buddy_id);
    contact_item.m_item_type = 0;

    m_ps.addMessageFromContact(contact_item,message,date);

    if(!m_switchboard_list.contains(buddy_id)) m_switchboard_list.insert(buddy_id,conn);

}

void MSNMessaging::sendMessage(const QString &buddy_id, const QString &message, int position)
{
    if ( !m_switchboard_list.contains(buddy_id))
    {
	m_protocol_wrapper->requestSwitchBoard(buddy_id);
	if(m_pending_messages.contains(buddy_id)) m_pending_messages[buddy_id].append(message);
	else m_pending_messages.insert(buddy_id,QStringList(message));
    } else
	m_switchboard_list.value(buddy_id)->sendMessage(m_protocol_wrapper->toStd(message));
	//m_protocol_wrapper->sendMessage(buddy_id,message);
}

void MSNMessaging::switchBoardCreated(const QString &buddy_id, MSN::SwitchboardServerConnection *conn)
{
    if(!m_switchboard_list.contains(buddy_id))
    {
	m_switchboard_list.insert(buddy_id,conn);
	if(m_pending_messages.contains(buddy_id))
	{
	    foreach(QString message, m_pending_messages[buddy_id])
		conn->sendMessage(m_protocol_wrapper->toStd(message));
	    m_pending_messages.remove(buddy_id);
	}
    }
}
