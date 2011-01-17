/*
    MSNConnStatusBox

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

#ifndef MSNCONNSTATUSBOX_H
#define MSNCONNSTATUSBOX_H

#include <QObject>
#include <QHBoxLayout>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include "msnpluginsystem.h"
#include "msnprotocolwrapper.h"


enum ConnectionState {
    connection_offline = 0,
    connection_connecting,
    connection_connected
};

class MSNConnStatusBox : public QObject
{
    Q_OBJECT
public:
    MSNConnStatusBox(const QString &account_name,const QString &profile_name,
		     MSNProtocolWrapper *protocol_wrapper, QObject *parent = 0);
    ~MSNConnStatusBox();
    void addButtonToLayout(QHBoxLayout *layout);
    QMenu* getAccountMenu() {return m_status_menu;}
    QIcon getCurrentIcon(){return m_account_button->icon();}
    void loadSettings();
private slots:
    void changeStatus();
    void iAmConnected();

private:
    bool checkStatusText(QAction *status_action);

    QMenu *m_status_menu;
    QAction *m_online_action;
    QAction *m_offline_action;
    QAction *m_busy_action;
    QAction *m_idle_action;
    QAction *m_rightback_action;
    QAction *m_away_action;
    QAction *m_onphone_action;
    QAction *m_lunch_action;
    QAction *m_invisible_action;
    QAction *m_current_action;

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

    QToolButton *m_account_button;
    QString m_account_name;
    QString m_profile_name;

    MSNPluginSystem &m_mps;
    MSNProtocolWrapper *m_protocol_wrapper;
    ConnectionState m_current_connection_state;

    bool m_ignore_status_message_setting;
};

#endif // MSNCONNSTATUSBOX_H
