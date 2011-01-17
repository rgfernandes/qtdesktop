/*
    jEventHandler.h

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

#ifndef JEVENTHANDLER_H
#define JEVENTHANDLER_H
#define pEvent qutim_sdk_0_2::Event

#include "jLayer.h"

class jLayer;

class jEventHandler : public qutim_sdk_0_2::EventHandler
{
public:
	jEventHandler(jLayer *parent);
	virtual void processEvent(pEvent &event);
	bool sendEvent(pEvent &event);

	void accountStatusChanged(const QString &account, const QString &status, const QString &message);
	void accountConnected(const QString &account);
	void accountDisconnected(const QString &account);

private:
	jLayer *m_layer;
	PluginSystemInterface *m_plugin_system;

	qint16 m_event_caller,
		m_event_handler;

	qint16 m_event_account_status_changed,
		m_event_account_connected,
		m_event_account_disconnected;

	qint16 m_event_account_status_change,
		m_event_account_mood_change,
		m_event_account_activity_change,
		m_event_account_tune_change;
};
#endif
