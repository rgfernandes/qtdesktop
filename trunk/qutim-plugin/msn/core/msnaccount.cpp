/*
    MSNaccount

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

#include "msnaccount.h"
#include "edditaccount.h"


MSNaccount::MSNaccount(const QString &account, const QString &profile_name):
	m_profile_name(profile_name),
	m_account_name(account)
{
    m_edit_dialog_opened = false;
    m_protocol_wrapper = new MSNProtocolWrapper(this);
    m_status_object = new MSNConnStatusBox(account, profile_name, m_protocol_wrapper, this);
    m_contact_list = new MSNContactList(account, profile_name, m_protocol_wrapper,this);
    m_messaging = new MSNMessaging(profile_name, account, m_protocol_wrapper, m_contact_list, this);
}

MSNaccount::~MSNaccount()
{
    delete m_protocol_wrapper;
    delete m_status_object;
    delete m_messaging;
    delete m_contact_list;
}

void MSNaccount::editAccountSettings()
{
    if ( !m_edit_dialog_opened )
    {
	EdditAccount *m_edit_dialog = new EdditAccount(m_account_name, m_profile_name);
	connect(m_edit_dialog, SIGNAL(destroyed(QObject*)), this, SLOT(editAccountSettingsClosed(QObject*)));
	m_edit_dialog->show();
	m_edit_dialog_opened = true;
    }
}

void MSNaccount::editAccountSettingsClosed(QObject*){
    m_edit_dialog_opened = false;
    //m_protocol_wrapper->loadSettings();
}

void MSNaccount::createAccountButton(QHBoxLayout *layout){
  m_status_object->addButtonToLayout(layout);
}

QMenu *MSNaccount::getAccountMenu()
{
return m_status_object->getAccountMenu();
}

QIcon MSNaccount::getCurrentStatusIcon()
{
    return m_status_object->getCurrentIcon();
}

void MSNaccount::removeContactList()
{
    if ( m_contact_list ) m_contact_list->removeContactList();
}

void MSNaccount::sendMessage(const QString &buddy_id, const QString &message, int icon_position)
{
    m_messaging->sendMessage(buddy_id, message,icon_position);
}
