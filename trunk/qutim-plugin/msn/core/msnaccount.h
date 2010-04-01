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

#ifndef MSNACCOUNT_H
#define MSNACCOUNT_H

#include <QObject>
#include "msnconnstatusbox.h"
#include "msnprotocolwrapper.h"
#include "msncontactlist.h"
#include "msnmessaging.h"

class MSNaccount : public QObject
{
    Q_OBJECT
public:
    MSNaccount(const QString &account, const QString &profile_name);
    ~MSNaccount();
    void editAccountSettings();
    bool m_edit_dialog_opened;
    void createAccountButton(QHBoxLayout *layout);
    QMenu *getAccountMenu();
    QIcon getCurrentStatusIcon();
    void removeContactList();
    void sendMessage(const QString &buddy_id, const QString &message, int icon_position);
private slots:
    void editAccountSettingsClosed(QObject*);

private:
    QString m_account_name;
    QString m_profile_name;
    MSNConnStatusBox *m_status_object;
    MSNProtocolWrapper *m_protocol_wrapper;
    MSNContactList *m_contact_list;
    MSNMessaging *m_messaging;
};

#endif // MSNACCOUNT_H
