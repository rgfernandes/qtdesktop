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

#include "ContactSettings.h"

ContactSettings::ContactSettings(const QString &profile_name, QWidget *parent) : QWidget(parent)
{
	m_profile_name = profile_name;
	ui.setupUi(this);
	changed = false;
	loadSettings();
	connect(ui.mainStatusShow, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.mainMoodShow, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.mainGeneralActivityShow, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.mainExtendedActivityShow, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.mainTuneShow, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.mainAuthShow, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.mainQIPStatusShow, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
	connect(ui.mainResNotifyShow, SIGNAL(stateChanged(int)), this, SLOT(widgetStateChanged()));
}

void ContactSettings::loadSettings()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	settings.beginGroup("roster");
	ui.mainStatusShow->setChecked(settings.value("showmessagestatus",true).toBool());
	ui.mainMoodShow->setChecked(settings.value("showmood",false).toBool());
	ui.mainGeneralActivityShow->setChecked(settings.value("showactivity",true).toBool());
	if (ui.mainGeneralActivityShow->isChecked())
		ui.mainExtendedActivityShow->setChecked(settings.value("showbothactivity",false).toBool());
	ui.mainTuneShow->setChecked(settings.value("showtune",false).toBool());
	ui.mainAuthShow->setChecked(settings.value("showauth",true).toBool());
	ui.mainQIPStatusShow->setChecked(settings.value("showxstatus",true).toBool());
	ui.mainResNotifyShow->setChecked(!settings.value("showmainresnotify",true).toBool());
	settings.endGroup();
}

void ContactSettings::saveSettings()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	settings.beginGroup("roster");
	settings.setValue("showmessagestatus", ui.mainStatusShow->isChecked());
	settings.setValue("showmood", ui.mainMoodShow->isChecked());
	settings.setValue("showactivity", ui.mainGeneralActivityShow->isChecked());
	if (ui.mainGeneralActivityShow->isChecked())
		settings.setValue("showbothactivity", ui.mainExtendedActivityShow->isChecked());
	else
		settings.setValue("showbothactivity", false);
	settings.setValue("showtune", ui.mainTuneShow->isChecked());
	settings.setValue("showauth", ui.mainAuthShow->isChecked());
	settings.setValue("showxstatus", ui.mainQIPStatusShow->isChecked());
	settings.setValue("showmainresnotify", !ui.mainResNotifyShow->isChecked());
	settings.endGroup();
	if (changed)
	{
		emit settingsSaved();
	}
	changed = false;
}
