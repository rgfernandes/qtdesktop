/*
    eventer.h

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

#ifndef EVENTER_H
#define EVENTER_H
#define pEvent qutim_sdk_0_2::Event

#include "connectioncheck.h"

// Предварительная декларация
class connectioncheckPlugin;

// Класс обработчика событий
class Eventer : public qutim_sdk_0_2::EventHandler
{
public:
	// В конструктор передаем указатель на плагин - хозяина
	Eventer(connectioncheckPlugin *parent);

	// Обработка поступающих событий	
	virtual void processEvent(Event &event);

private:
	connectioncheckPlugin *m_parent_plugin;
	PluginSystemInterface *m_plugin_system;

	quint16 m_event_handler;

	quint16 m_event_inicialized,
		m_event_jabber_account_connected,
		m_event_jabber_account_disconnected,
		m_event_jabber_account_status_changed;

	bool previusRun;
};


#endif
