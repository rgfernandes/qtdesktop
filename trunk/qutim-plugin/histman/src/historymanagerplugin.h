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

#ifndef historymanagerplugin_H
#define historymanagerplugin_H
#include <qutim/iconmanagerinterface.h>

using namespace qutim_sdk_0_2;

class HistoryManagerPlugin : public QObject, SimplePluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qutim_sdk_0_2::PluginInterface)
public:
	virtual bool init(PluginSystemInterface *plugin_system);
	virtual void release() {}
	virtual QString name() { return "History Manager"; }
	virtual QString description() { return "qutIM History Manager\nAutor: Nigmatullin Ruslan"; }
	virtual QString type() { return "history"; }
	virtual QIcon *icon() { return &m_plugin_icon; }
	virtual void setProfileName(const QString &profile_name);
	virtual void processEvent(PluginEvent &){}
	virtual QWidget *settingsWidget();	
	virtual void removeSettingsWidget();
	virtual void saveSettings();
public slots:
	void createWidget();
private:
	QIcon m_plugin_icon;
	QString m_profile_name;
	QPointer<QWidget> m_widget;
	PluginSystemInterface *m_plugin_system;
	QAction *m_action;
};

#endif
