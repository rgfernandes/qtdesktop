/*
    Сopyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JACCOUNTSETTINGS_H
#define JACCOUNTSETTINGS_H

#include <QtGui/QWidget>
#include <QSettings>
#include <QMessageBox>
#include "ui_jAccountSettings.h"
#include "jAccount.h"

class jAccountSettings : public QWidget
{
    Q_OBJECT

public:
    jAccountSettings(const QString &profile_name, const QString &account_name, jAccount *account, QWidget *parent = 0);
    ~jAccountSettings();
signals:
	void settingsSaved();

private slots:
	void okButtonClicked();
	void applyButtonClicked();
	void changeEnabled(int);
	void stateChange() {ui.applyButton->setEnabled(true);}

private:
	void loadSettings();
	void saveSettings();
	Ui::jAccountSettingsClass ui;
	QString m_profile_name;
	QString m_account_name;
	jAccount *m_account;
};

#endif // JACCOUNTSETTINGS_H
