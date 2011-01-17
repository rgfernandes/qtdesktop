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

#ifndef MSNMESSAGING_H
#define MSNMESSAGING_H

#include <QObject>
#include "msnprotocolwrapper.h"
#include "msnpluginsystem.h"

class MSNContactList;

class MSNMessaging : public QObject
{
    Q_OBJECT
public:
    MSNMessaging(const QString &profile_name, const QString &account_name,
		 MSNProtocolWrapper *protocol_wrapper, MSNContactList *contact_list,
		 QObject *parent = 0);
    void sendMessage(const QString &buddy_id, const QString &message, int position);
private slots:
    void messageArrived(const QString &buddy_id,const QString &buddy_friendly_name,
			const QString &message,const QDateTime &date,
			MSN::SwitchboardServerConnection *conn);
    void switchBoardCreated(const QString &buddy_id, MSN::SwitchboardServerConnection *conn);
private:
    QString m_profile_name;
    QString m_account_name;
    MSNProtocolWrapper *m_protocol_wrapper;
    MSNContactList *m_contact_list;
    MSNPluginSystem &m_ps;
    QHash<QString, MSN::SwitchboardServerConnection *> m_switchboard_list;
    QHash<QString, QStringList> m_pending_messages;
};

#endif // MSNMESSAGING_H
