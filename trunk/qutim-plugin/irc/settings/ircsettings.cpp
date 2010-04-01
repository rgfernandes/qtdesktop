/*
    ircSettings

    Copyright (c) 2008 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#include "ircsettings.h"

ircSettings::ircSettings(const QString &profile_name,
		QWidget *parent)
    : QWidget(parent),
    m_profile_name(profile_name)
{
	ui.setupUi(this);
	changed = false;
	/*
	QRegExp rx("[ABCDEFabcdef0123456789]{32,32}");
	QValidator *validator = new QRegExpValidator(rx, this);
	ui.capEdit1->setValidator(validator);
	ui.capEdit2->setValidator(validator);
	ui.capEdit3->setValidator(validator);
	*/
	loadSettings();
/*	
	connect( ui.autoBox, SIGNAL(stateChanged(int)),
			this, SLOT(widgetStateChanged()));
*/
}

ircSettings::~ircSettings()
{

}

void ircSettings::loadSettings()
{
        QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "ircsettings");

}

void ircSettings::saveSettings()
{
        QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "ircsettings");
	
	if (changed) emit settingsSaved();
	changed = false;
}

