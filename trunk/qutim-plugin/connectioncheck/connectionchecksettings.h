/*
    connectionchecksettings.h

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

#ifndef CONNECTIONCHECKSETTINGS_H
#define CONNECTIONCHECKSETTINGS_H
#include "ui_connectionchecksettings.h"
#include <QSettings>
#include <QRadioButton>

// Ксласс виджета настроек
class connectioncheckSettings : public QWidget
{
	Q_OBJECT;
public:
	connectioncheckSettings(const QString &profile_name);
	~connectioncheckSettings();

	void saveSettings();

private:
	Ui::connectioncheckSettings ui;
	QString m_profile_name;
};

#endif