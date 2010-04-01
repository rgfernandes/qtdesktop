/*
		twAccount

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

#include "twaccount.h"
//#include "editaccount.h"

twAccount::twAccount(const QString &account, const QString &profile_name):
        m_profile_name(profile_name),
        m_account_name(account)
{
	m_edit_dialog_opened = false;
	m_api_wrapper = new twApiWrap(m_account_name, m_profile_name);
	m_status_object = new twStatusObject(account, profile_name, m_api_wrapper, this);
	m_cl_object = new twContactList(account, profile_name,m_api_wrapper);
//	connect(m_status_object, SIGNAL(iMOffline()), m_cl_object, SLOT(setAllOffline()));
}

twAccount::~twAccount()
{
	delete m_status_object;
	delete m_cl_object;
	delete m_api_wrapper;
}

void twAccount::loadSettings()
{


}

void twAccount::createAccountButton(QHBoxLayout *layout){
	m_status_object->addButtonToLayout(layout);
}

QMenu *twAccount::getAccountMenu()
{
	return m_status_object->getAccountMenu();
}

void twAccount::editAccountSettings()
{/*
    if ( !m_edit_dialog_opened )
    {
				EditAccount *m_edit_dialog = new EditAccount(m_account_name, m_profile_name);
        connect(m_edit_dialog, SIGNAL(destroyed(QObject*)), this, SLOT(editAccountSettingsClosed(QObject*)));
        m_edit_dialog->show();
        m_edit_dialog_opened = true;
    }
*/}

void twAccount::editAccountSettingsClosed(QObject*)
{
    m_edit_dialog_opened = false;
		m_api_wrapper->loadSettings();
}

QIcon twAccount::getCurrentStatusIcon()
{
		return m_status_object->getCurrentIcon();
}

void twAccount::removeCL()
{
		m_cl_object->removeCL();
}

QString twAccount::getToolTip(const QString &buddy_name)
{
	return m_cl_object->getToolTip(buddy_name);
}

void twAccount::sendMessage(const QString &buddy_id, const QString &message)
{
	m_api_wrapper->sendMessage(buddy_id, message);
}

QStringList twAccount::getBuddyInfo(const QString &buddy_id)
{
//    return m_cl_object->getInfoAboutBuddy(buddy_id);
return QStringList();
}
