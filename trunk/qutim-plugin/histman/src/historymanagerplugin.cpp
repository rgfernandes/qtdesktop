/*
	HistoryManagerPlugin

	Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "historymanagerplugin.h"
#include "historymanagerwindow.h"
#include <QDebug>
#include <QFile>

bool HistoryManagerPlugin::init(PluginSystemInterface *plugin_system)
{
	PluginInterface::init(plugin_system);
	m_plugin_system = plugin_system;
	m_action = new QAction("Import history", this);
	connect(m_action, SIGNAL(triggered()), this, SLOT(createWidget()));
	m_plugin_system->registerMainMenuAction(m_action);
	m_widget=0;
	return true;
}

void HistoryManagerPlugin::setProfileName(const QString &profile_name)
{
	m_plugin_icon = Icon("history");
	m_profile_name = profile_name;
	m_action->setIcon(m_plugin_icon);
	m_action->setText(tr("Import history"));
}

QWidget *HistoryManagerPlugin::settingsWidget()
{
	//m_widget = new HistoryManagerWidget(m_profile_name, m_plugin_system);
	return new QWidget();//m_widget;
}	

void HistoryManagerPlugin::removeSettingsWidget()
{
	//delete m_widget;
	//m_widget=0;
}

void HistoryManagerPlugin::saveSettings()
{
}

void HistoryManagerPlugin::createWidget()
{
	if(!m_widget)
	{
		m_widget = new HistoryManager::HistoryManagerWindow();
		m_widget->show();
	}
//	m_widget = new HistoryManagerWidget(m_profile_name, m_plugin_system);
//	m_widget->show();
//	connect(m_widget, SIGNAL(destroyed()), this, SLOT(killWidget()));
}

Q_EXPORT_PLUGIN2(historymanager, HistoryManagerPlugin);
