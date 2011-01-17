/*
    connectionchecksettings.cpp

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

#include "connectionchecksettings.h"

// Конструктор
connectioncheckSettings::connectioncheckSettings(const QString &profile_name) 
{
	ui.setupUi(this);
	m_profile_name = profile_name;

	// Обьект для чтения настроек
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "connectioncheck");

	// Читаем и устанавливаем активный элемент в группе кнопок active
	switch ( settings.value("enabled", true).toBool() )
	{
		case true:
			ui.activeRadio->setChecked(true);
			break;
		case false:
			ui.deactiveRadio->setChecked(true);
			break;
	}

	// Читаем период проверки
	ui.periodBox->setValue(settings.value("period",10).toInt());

	if ( settings.value("routecheck", true).toBool() )
		ui.routeBox->setCheckState(Qt::Checked);
	if ( settings.value("pingcheck", true).toBool() )
		ui.pingBox->setCheckState(Qt::Checked);
}

// Деструктор
connectioncheckSettings::~connectioncheckSettings()
{
	
}

// Функция записи настроек в файл
void connectioncheckSettings::saveSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "connectioncheck");

	// Пишем настройки в файл
	settings.setValue("enabled",ui.activeRadio->isChecked());
	settings.setValue("period", ui.periodBox->value());

	if ( ui.routeBox->checkState() == Qt::Checked)
		settings.setValue("routecheck", true);
	else
		settings.setValue("routecheck", false);

	if ( ui.pingBox->checkState() == Qt::Checked )
		settings.setValue("pingcheck", true);
	else
		settings.setValue("pingcheck", false);
}
