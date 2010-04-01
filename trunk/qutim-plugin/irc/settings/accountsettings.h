/*
		ircAccountSettings

		Copyright (c) 2008-2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef ACCOUNTSETTINGS_H
#define ACCOUNTSETTINGS_H
//
#include <QWidget>
#include <QSettings>
#include <QDesktopWidget>
#include <QDebug>
#include "ui_accountsettings.h"
//
class ircAccountSettings : public QWidget, public Ui::ircAccountSettingsClass
{
Q_OBJECT

public:
	ircAccountSettings(QSettings *account_settings, QWidget * parent = 0, Qt::WFlags f = 0 );

private slots:
	void on_btnOK_clicked();
	void on_btnCancel_clicked();
	void on_btnApply_clicked();


private:
	void loadSettings();
	void saveSettings();
	QSettings *settings;

};
#endif





