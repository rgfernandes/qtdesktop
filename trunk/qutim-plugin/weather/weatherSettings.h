/*
    weatherSettings.h

    Copyright (c) 2008 by Nikita 'Δz' Belov <null@deltaz.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef weatherSettings_H
#define weatherSettings_H

#include "ui_weatherSettings.h"

#include <QSettings>
#include <QtNetwork>
#include <QtXml>

class weatherSettings  : public QWidget
{
	Q_OBJECT;

public:
	weatherSettings(QString profileName);
	virtual ~weatherSettings();
	void saveSettings();

private slots:
	void on_addCityButton_clicked();
	void on_deleteCityButton_clicked();
	void on_searchButton_clicked();

	void searchFinished(QNetworkReply *reply);
	void searchListRowChanged(int row);
	void searchListItemDoubleClicked(QListWidgetItem *);
	
private:
	Ui::weatherSettingsClass ui;
	QString m_profile_name;

	QNetworkAccessManager *netMan;

	QList<QString> searchResults;
};

#endif
