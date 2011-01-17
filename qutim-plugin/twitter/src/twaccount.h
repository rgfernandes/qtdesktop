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

#ifndef TWACCOUNT_H
#define TWACCOUNT_H
#include <QHBoxLayout>
#include <QMenu>
#include "twstatusobject.h"
#include "twapiwrap.h"
#include "twcontactlist.h"

class twAccount : public QObject
{
    Q_OBJECT
public:
		twAccount(const QString &account, const QString &m_profile_name);
		~twAccount();
    void createAccountButton(QHBoxLayout *layout);
    QMenu *getAccountMenu();
    void editAccountSettings();
    bool m_edit_dialog_opened;
    QIcon getCurrentStatusIcon();
    void loadSettings();
    void removeCL();
    QString getToolTip(const QString &buddy_name);
    void sendMessage(const QString &buddy_id, const QString &message);
    QStringList getBuddyInfo(const QString &buddy_id);
private slots:
    void editAccountSettingsClosed(QObject*);

private:
    QString m_account_name;
    QString m_profile_name;
		twStatusObject *m_status_object;
		twApiWrap *m_api_wrapper;
		twContactList *m_cl_object;
};

#endif // TWACCOUNT_H
