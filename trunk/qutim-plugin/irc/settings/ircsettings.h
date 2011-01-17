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


#ifndef IRCSETTINGS_H
#define IRCSETTINGS_H

#include <QtGui/QWidget>
#include <QSettings>
#include "ui_ircsettings.h"

class ircSettings : public QWidget
{
    Q_OBJECT

public:
    ircSettings(const QString &profile_name,
    		QWidget *parent = 0);
    ~ircSettings();
    void loadSettings();
    void saveSettings();

private slots:
	void widgetStateChanged() {   changed = true; emit settingsChanged();  }
    
signals:
    void settingsChanged();
    void settingsSaved();

private:
    Ui::ircSettingsClass ui;
    bool changed;
    QString m_profile_name;
};

#endif // IRCSETTINGS_H
