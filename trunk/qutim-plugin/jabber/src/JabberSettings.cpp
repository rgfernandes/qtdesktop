/*
Copyright (c) 2009 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "JabberSettings.h"

JabberSettings::JabberSettings(const QString &profile_name, QWidget *parent) : QWidget(parent)
{
	m_profile_name = profile_name;
	ui.setupUi(this);
	changed = false;
	loadSettings();
	connect(ui.resourceEdit, SIGNAL(textChanged(QString)), this, SLOT(widgetStateChanged()));
	connect(ui.reconnectCheck, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.avatarRequestCheck, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.transferPostEdit, SIGNAL(valueChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.onlinePriority, SIGNAL(valueChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.ffchatPriority, SIGNAL(valueChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.awayPriority, SIGNAL(valueChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.naPriority, SIGNAL(valueChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.dndPriority, SIGNAL(valueChanged(int)), this, SLOT(widgetStateChanged()));
}

void JabberSettings::loadSettings()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	settings.beginGroup("main");
	ui.resourceEdit->setText(settings.value("defaultresource","qutIM").toString());
	ui.reconnectCheck->setChecked(settings.value("reconnect",true).toBool());
	ui.avatarRequestCheck->setChecked(!settings.value("getavatars",true).toBool());
	settings.endGroup();
	/*settings.beginGroup("xeps");
	ui.mainTuneShow->setChecked(settings.value("showtune",false).toBool());
	settings.endGroup();*/
	ui.transferPostEdit->setValue(settings.value("filetransfer/socks5port", 8010).toInt());
	settings.beginGroup("priority");
	ui.onlinePriority->setValue(settings.value("online",30).toInt());
	ui.ffchatPriority->setValue(settings.value("ffchat",30).toInt());
	ui.awayPriority->setValue(settings.value("away",20).toInt());
	ui.naPriority->setValue(settings.value("na",10).toInt());
	ui.dndPriority->setValue(settings.value("dnd",5).toInt());
	settings.endGroup();
}

void JabberSettings::saveSettings()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	settings.beginGroup("main");
	settings.setValue("defaultresource", ui.resourceEdit->text());
	settings.setValue("reconnect", ui.reconnectCheck->isChecked());
	settings.setValue("getavatars", !ui.avatarRequestCheck->isChecked());
	settings.endGroup();
	settings.beginGroup("priority");
	settings.setValue("online", ui.onlinePriority->value());
	settings.setValue("ffchat", ui.ffchatPriority->value());
	settings.setValue("away", ui.awayPriority->value());
	settings.setValue("na", ui.naPriority->value());
	settings.setValue("dnd", ui.dndPriority->value());
	settings.endGroup();
	/*settings.beginGroup("xeps");
	ui.mainTuneShow->setChecked(settings.value("showtune",false).toBool());
	settings.endGroup();*/
	settings.setValue("filetransfer/socks5port", ui.transferPostEdit->value());
	if (changed)
		emit settingsSaved();
	changed = false;
}
