/*
    eventer.cpp

    Copyright (c) 2009 by Igor 'Sqee' Syromyatnikov <sqee@olimp.ua>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "eventer.h"

Eventer::Eventer(connectioncheckPlugin *parent) : m_parent_plugin(parent)
{
	// Получаем ссылку на родительский плагин
	m_plugin_system = m_parent_plugin->getMainPluginSystemPointer();

	// Вешаемся на евенты
	m_event_inicialized = m_plugin_system->registerEventHandler("Core/Layers/Initialized", this);
	m_event_jabber_account_connected = m_plugin_system->registerEventHandler("Jabber/Account/Connected", this);
	m_event_jabber_account_disconnected = m_plugin_system->registerEventHandler("Jabber/Account/Disconnected", this);
	m_event_jabber_account_status_changed = 
	m_plugin_system->registerEventHandler("Jabber/Account/Status/Changed", this);

	previusRun = false;
}

void Eventer::processEvent(Event &event)
{
	/* Странно почему то Jabber/Account/Status/Changed приходит не для всех акков
	кажется баг не у меня, надо бы поспрашивать, на всякий случай сделаем workaround */

	if ( event.id == m_event_inicialized )
	{
		m_parent_plugin->load();
	}
	else if ( event.id == m_event_jabber_account_connected )
	{
		// Пришел евент коннекта аккакунта, заносим его в список со статусом Online

		// qDebug() << "Received Jabber/Account/Connected";

		QString acc = *(QString *)event.args.at(0);
		QString status = "online";

		if ( (!m_parent_plugin->accountData.contains(acc)) )
			m_parent_plugin->accountData.insert(acc, status);
	}
	else if ( event.id == m_event_jabber_account_status_changed )
	{
		// Пришел евент смены статуса, сменим статус в списке аккаунтов.
		// Но только если это не диссконект от нашего плагина
		// qDebug() << "Received Jabber/Account/Status/Changed";

		QString acc = *(QString *)event.args.at(0);
		QString status = *(QString *)event.args.at(1);

		if ( (m_parent_plugin->accountData.contains(acc)) && (!m_parent_plugin->disconnectedFromUs) )
		{
			// Бля replace() почемуто сегфолтит при определенных условиях, будем проще)
			m_parent_plugin->accountData.remove(acc);
			m_parent_plugin->accountData.insert(acc, status);
		}

		m_parent_plugin->disconnectedFromUs = false;
	}
	else if ( event.id == m_event_jabber_account_disconnected )
	{
		// Евент дисконнекта аккаунта
		// если дисконнект не из-за нашего плагина - удаляем его из списка

		// qDebug() << "Recieved Jabber/Account/Disconnected";

		QString acc = *(QString *)event.args.at(0);

		if ( (m_parent_plugin->accountData.contains(acc)) && (!m_parent_plugin->disconnectedFromUs) )
			m_parent_plugin->accountData.remove(acc);
	}
}
