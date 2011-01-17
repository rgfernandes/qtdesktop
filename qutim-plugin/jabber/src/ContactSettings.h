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

#ifndef CONTACTSETTINGS_H
#define CONTACTSETTINGS_H

#include <QWidget>
#include "jAccount.h"
#include <QSettings>
#include "ui_ContactSettings.h"

class ContactSettings: public QWidget
{
	Q_OBJECT

	public:
		ContactSettings(const QString &profile_name, QWidget *parent = 0);
		void loadSettings();
		void saveSettings();

	signals:
		void settingsChanged();
		void settingsSaved();

	private slots:
		void widgetStateChanged() {changed = true; emit settingsChanged();}

	private:
		Ui::Contacts ui;
		QString m_profile_name;
		bool changed;
};

#endif // CONTACTSETTINGS_H
