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

#include "msnconnstatusbox.h"
#include "msnstatusicons.h"

MSNConnStatusBox::MSNConnStatusBox(const QString &account_name,const QString &profile_name,
		     MSNProtocolWrapper *protocol_wrapper, QObject *parent)
	: QObject(parent),
	m_profile_name(profile_name),
	m_account_name(account_name),
	m_protocol_wrapper(protocol_wrapper),
	m_mps(MSNPluginSystem::instance())
{
    m_current_connection_state = connection_offline;

    m_status_menu = new QMenu;
    m_status_menu->setTitle(account_name);

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


    m_status_menu->setIcon(*m_offline_icon);
    QVector<QAction *> statusMenuActions;

    m_online_action = new QAction(*m_online_icon,tr("Online"),this);
    statusMenuActions.push_back(m_online_action);
    m_busy_action = new QAction(*m_busy_icon,tr("Busy"),this);
    statusMenuActions.push_back(m_busy_action);
    m_idle_action = new QAction(*m_idle_icon,tr("Idle"),this);
    statusMenuActions.push_back(m_idle_action);
    m_rightback_action = new QAction(*m_rightback_icon,tr("Will be right back"),this);
    statusMenuActions.push_back(m_rightback_action);
    m_away_action = new QAction(*m_away_icon,tr("Away"),this);
    statusMenuActions.push_back(m_away_action);
    m_onphone_action = new QAction(*m_onphone_icon,tr("On the phone"),this);
    statusMenuActions.push_back(m_onphone_action);
    m_lunch_action = new QAction(*m_lunch_icon,tr("Out to lunch"),this);
    statusMenuActions.push_back(m_lunch_action);
    m_invisible_action = new QAction(*m_invisible_icon,tr("Invisible"),this);
    statusMenuActions.push_back(m_invisible_action);    
    m_offline_action = new QAction(*m_offline_icon,tr("Offline"),this);
    statusMenuActions.push_back(m_offline_action);

    m_current_action = m_offline_action;
    m_account_button = new QToolButton;
    m_account_button->setMinimumSize(QSize(22, 22));
    m_account_button->setMaximumSize(QSize(22, 22));
    m_account_button->setAutoRaise(true);
    m_account_button->setIcon(*m_offline_icon);
    m_account_button->setToolTip(account_name);

    foreach(QAction *tmp_action, statusMenuActions)
    {
		tmp_action->setCheckable(true);
		if(tmp_action == m_offline_action)
		{
		    tmp_action->setChecked(true);
		    connect(m_protocol_wrapper, SIGNAL(iAmDisconnected()),
			    tmp_action, SLOT(trigger()));
		}

		m_status_menu->addAction(tmp_action);
		connect(tmp_action, SIGNAL(triggered()), this, SLOT(changeStatus()));
    }

    connect(m_protocol_wrapper, SIGNAL(iAmConnected()), this, SLOT(iAmConnected()));
    m_ignore_status_message_setting = false;
    loadSettings();
}

MSNConnStatusBox::~MSNConnStatusBox()
{
    delete m_online_action;
    delete m_offline_action;
    delete m_status_menu;
    delete m_account_button;
}

void MSNConnStatusBox::addButtonToLayout(QHBoxLayout *layout)
{

#if defined(Q_OS_MAC)
	layout->addWidget(m_account_button, 0, Qt::AlignLeft);
#else
	layout->addWidget(m_account_button, 0, Qt::AlignRight);
#endif

	m_account_button->setPopupMode(QToolButton::InstantPopup);
	m_account_button->setMenu(m_status_menu);
}

void MSNConnStatusBox::changeStatus()
{
    QAction *tmp_action = qobject_cast<QAction*>(sender());

    if(tmp_action != m_invisible_action && tmp_action != m_offline_action && !m_ignore_status_message_setting)
    {

	    if(!checkStatusText(tmp_action))
	    {
		tmp_action->setChecked(false);
		return;
	    }
    }
    if(m_current_connection_state == connection_offline && tmp_action != m_offline_action)
    {
	m_ignore_status_message_setting = true;
	tmp_action->setChecked(false);
	m_current_action = tmp_action;
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "accountsettings");
	m_protocol_wrapper->tryToConnect(settings.value("main/name").toString(),settings.value("main/password").toString());
	m_account_button->setIcon(*m_connecting_icon);
	m_status_menu->setIcon(*m_connecting_icon);
	m_mps.updateStatusIcons();
	m_current_connection_state = connection_connecting;
    }
    else
    {
	if(m_current_action != tmp_action)
	    m_current_action->setChecked(false);
	m_current_action = tmp_action;
	if(m_current_action == m_offline_action)
	{


	    m_ignore_status_message_setting = false;
	    m_current_connection_state = connection_offline;
	    m_protocol_wrapper->tryToDisconnect();
	}
	else if(m_current_connection_state == connection_connected)
	{
	    if(m_current_action == m_online_action) m_protocol_wrapper->setStatus(MSN::STATUS_AVAILABLE);
	    else if(m_current_action == m_busy_action) m_protocol_wrapper->setStatus(MSN::STATUS_BUSY);
	    else if(m_current_action == m_idle_action) m_protocol_wrapper->setStatus(MSN::STATUS_IDLE);
	    else if(m_current_action == m_rightback_action) m_protocol_wrapper->setStatus(MSN::STATUS_BERIGHTBACK);
	    else if(m_current_action == m_away_action) m_protocol_wrapper->setStatus(MSN::STATUS_AWAY);
	    else if(m_current_action == m_onphone_action) m_protocol_wrapper->setStatus(MSN::STATUS_ONTHEPHONE);
	    else if(m_current_action == m_lunch_action) m_protocol_wrapper->setStatus(MSN::STATUS_OUTTOLUNCH);
	    else if(m_current_action == m_invisible_action) m_protocol_wrapper->setStatus(MSN::STATUS_INVISIBLE);
	    m_protocol_wrapper->setStatusMessage(m_current_action->property("smessage").toString());
	}
	m_current_action->setChecked(true);
	m_account_button->setIcon(m_current_action->icon());
	m_status_menu->setIcon(m_current_action->icon());
	m_mps.updateStatusIcons();
    }
}



void MSNConnStatusBox::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "accountsettings");
    if (settings.value("main/autoconnect",false).toBool() )
	m_online_action->trigger();
}

void MSNConnStatusBox::iAmConnected()
{
    m_current_connection_state = connection_connected;
    m_offline_action->setChecked(false);
    m_current_action->trigger();
    m_ignore_status_message_setting = false;
}

bool MSNConnStatusBox::checkStatusText(QAction *status_action)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "accountsettings");
    QString status_name;
    if(status_action == m_online_action) status_name = "online";
    else if(status_action == m_busy_action) status_name = "busy";
    else if(status_action == m_idle_action) status_name = "idle";
    else if(status_action == m_rightback_action) status_name = "rightback";
    else if(status_action == m_away_action) status_name = "away";
    else if(status_action == m_onphone_action) status_name = "onphone";
    else if(status_action == m_lunch_action) status_name = "lunch";
    QString restore_status_text = settings.value("autoreply/" + status_name
				+ "msg", "").toString();

    if (!settings.value("autoreply/" + status_name + "dshow", false).toBool())
    {
	    QString status_message = settings.value("autoreply/" + status_name
			    + "msg", "").toString();
	    bool do_not_show = false;
	    if ( MSNPluginSystem::instance().setStatusMessage(status_message, do_not_show) )
	    {
		settings.setValue("autoreply/" + status_name + "dshow", do_not_show);
		settings.setValue("autoreply/" + status_name
			+ "msg", status_message.left(1000));
		status_action->setProperty("smessage",status_message);
		return true;
	    } else {
		return false;
	    }
    } else
    {
	status_action->setProperty("smessage",restore_status_text);
	return true;
    }
}
