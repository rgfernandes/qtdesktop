/*****************************************************************************
    Protocol Icon Plugin

	Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef protocolicon_H
#define protocolicon_H

#include <QtCore>
#include <QtGui>
#include <QtXml>

#include <qutim/plugininterface.h>
#include "ui_pluginsettings.h"

using namespace qutim_sdk_0_2;

class ProtocolIcon : public QObject, public SimplePluginInterface, public EventHandler
{
	Q_OBJECT
	Q_INTERFACES(qutim_sdk_0_2::PluginInterface)
public:

	virtual bool init(PluginSystemInterface *plugin_system);
	virtual void release() {}
	virtual QString name() { return "Protocol Icon plugin"; }
	virtual QString description() { return "Plugin for input Icons of protocol into Contact List.\nAutor: Nigmatullin Ruslan"; }
	virtual QString type() { return "sample"; }
	virtual QIcon *icon() { return &m_plugin_icon; }
	virtual void setProfileName(const QString &profile_name);
	virtual void processEvent(Event &event);
	virtual QWidget *settingsWidget();	
	virtual void removeSettingsWidget();
	virtual void saveSettings();
private:
	QHash <QString,QIcon> m_icons;
	QIcon m_default_icon;
	QIcon m_plugin_icon;
	QString m_profile_name;
	QWidget *m_widget;
	QString m_ya;
	Ui::PluginSettings ui;


protected:
	PluginSystemInterface *m_plugin_system;
	quint16 m_event_id;
	bool m_account;
	bool m_contact;
};

#endif
