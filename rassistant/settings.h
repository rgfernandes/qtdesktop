/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "ui_settings.h"
#include "fontsettingsdialog.h"

class AppSettings : public QDialog
{
     Q_OBJECT

public:
	AppSettings(QWidget *parent = 0);
	
signals:
	void showContentsAV(bool b);
	void showContentsAVHeader(bool b);
	void updateApplicationFontSettings(FontSettings);

public slots:
	void accept();
	void reject();
	void apply();
	void set();
	
private:
	Ui::AppSettings ui; 
	
private slots:
	void chooseEditor();
	void chooseBrowser();
	void chooseArchiver();
	void chooseBackupDir();
	void showFontSettingsDialog();
};
#endif // __SETTINGS_H__
