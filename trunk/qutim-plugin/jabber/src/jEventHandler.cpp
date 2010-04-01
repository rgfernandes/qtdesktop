/*
    jEventHandler.cpp

    Copyright (c) 2009 by Nikita Belov <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "jEventHandler.h"
#include "jProtocol.h"

jEventHandler::jEventHandler(jLayer *parent) : m_layer(parent)
{
	m_plugin_system = m_layer->getMainPluginSystemPointer();

	m_event_account_status_changed = m_plugin_system->registerEventHandler("Jabber/Account/Status/Changed");
	m_event_account_connected = m_plugin_system->registerEventHandler("Jabber/Account/Connected");
	m_event_account_disconnected = m_plugin_system->registerEventHandler("Jabber/Account/Disconnected");

	m_event_account_status_change = m_plugin_system->registerEventHandler("Jabber/Account/Status/Change", this);
	m_event_account_mood_change = m_plugin_system->registerEventHandler("Jabber/Account/Mood/Change", this);
	m_event_account_activity_change = m_plugin_system->registerEventHandler("Jabber/Account/Activity/Change", this);
	m_event_account_tune_change = m_plugin_system->registerEventHandler("Jabber/Account/Tune/Change", this);

	m_event_handler = m_plugin_system->registerEventHandler("Core/OpenWidget/About", this);
	m_event_caller = m_plugin_system->registerEventHandler("Jabber/Account/Mood/Change");
}

void jEventHandler::processEvent(pEvent &event)
{
	if (event.args.size() < 1)
		return;
	
	QStringList *acc_list = acc_list = (QStringList *)event.args.at(0);
	if (acc_list == 0x0)
			return;
	jAccount *account;

	if (event.id == m_event_account_status_change)
	{
		QString status = *(QString *)event.args.at(1);

		foreach (QString acc, *acc_list)
			if ((account=m_layer->getAccount(acc)) != 0x0)
				account->setStatusP(jAccount::getPresence(status), true);
	}
	else if (event.id == m_event_account_mood_change)
	{
		QStringList list;
		list.append(*(QString *)event.args.at(1)); //name
		list.append(*(QString *)event.args.at(2)); //text

		foreach (QString acc, *acc_list)
			if ((account=m_layer->getAccount(acc)) != 0x0)
				account->getProtocol()->setMood(list);
	}
	else if (event.id == m_event_account_activity_change)
	{
		QStringList list;
		list.append(*(QString *)event.args.at(1)); //general
		list.append(*(QString *)event.args.at(2)); //specific
		list.append(*(QString *)event.args.at(3)); //text

		foreach (QString acc, *acc_list)
			if ((account=m_layer->getAccount(acc)) != 0x0)
				account->getProtocol()->setActivity(list);
	}
	else if (event.id == m_event_account_tune_change)
	{
		QStringList list;
		list.append(*(QString *)event.args.at(1)); //artist
		list.append(QString::number(*(int *)event.args.at(2))); //length
		list.append(QString::number(*(int *)event.args.at(3))); //rating
		list.append(*(QString *)event.args.at(4)); //source
		list.append(*(QString *)event.args.at(5)); //title
		list.append(*(QString *)event.args.at(6)); //track
		list.append(*(QString *)event.args.at(7)); //uri

		foreach (QString acc, *acc_list)
			if ((account=m_layer->getAccount(acc)) != 0x0)
				account->getProtocol()->setTune(list);
	}
	else if(event.id == m_event_handler)
	{
		qDebug() << "event.id = m_event_handler";

		//pEvent e(m_event_caller, 2, &list, &status);
		//sendEvent(e);
	}
}

bool jEventHandler::sendEvent(pEvent &event)
{
	return m_plugin_system->sendEvent(event);
}

void jEventHandler::accountStatusChanged(const QString &account, const QString &status, const QString &message)
{
	pEvent event(m_event_account_status_changed, 3, &account, &status, &message);
	sendEvent(event);
}

void jEventHandler::accountConnected(const QString &account)
{
	pEvent event(m_event_account_connected, 1, &account);
	sendEvent(event);
}

void jEventHandler::accountDisconnected(const QString &account)
{
	pEvent event(m_event_account_disconnected, 1, &account);
	sendEvent(event);
}
