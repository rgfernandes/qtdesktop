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

#ifndef JABBERSETTINGS_H
#define JABBERSETTINGS_H

#include <QWidget>
#include "jAccount.h"
#include <QSettings>
#include "ui_JabberSettings.h"

class JabberSettings: public QWidget
{
	Q_OBJECT

	public:
		JabberSettings(const QString &profile_name, QWidget *parent = 0);
		void loadSettings();
		void saveSettings();

	signals:
		void settingsChanged();
		void settingsSaved();

	private slots:
		void widgetStateChanged() {changed = true; emit settingsChanged();}

	private:
		Ui::JabberSettings ui;
		QString m_profile_name;
		bool changed;
};

#endif // JABBERSETTINGS_H
