/*
    weatherPlugin.h

    Copyright (c) 2008-2009 by Nikita 'Δz' Belov <null@deltaz.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef weatherPlugin_H
#define weatherPlugin_H

#include <qutim/plugininterface.h>
#include <QtNetwork>
#include <QtXml>
#include <QHash>
#include <QMenu>
#include <QWidgetAction>
#include <QDateTime>

#include "weatherSettings.h"

using namespace qutim_sdk_0_2;

class weatherPlugin : public QObject, SimplePluginInterface, EventHandler
{
	Q_OBJECT
	Q_INTERFACES(qutim_sdk_0_2::PluginInterface)

public:
	virtual bool init(PluginSystemInterface *plugin_system);
	virtual void release();
	virtual void processEvent(Event &event);
	virtual QWidget *settingsWidget();
	virtual void setProfileName(const QString &profileName);
	virtual QString name();
	virtual QString description();
	virtual QString type();
	virtual QIcon *icon();
	virtual void removeSettingsWidget();
	virtual void saveSettings();
	
	void load();
	void loadCityList();
	void getWeather(QString cityId);
	void setWeather(QString cityId);

private slots:
	void getWeatherFinished(QNetworkReply *reply);
	void timerTimeOut();
	void actionGetNow();

private:
	void setMsgTemplate();
	QString msgTemplate;

	struct weather
	{
		QString dnam;
		QString tmp;
		QString ut;
		QString icon;
		QString t;
		QString flik;
		QString uv_i;
		QString uv_t;
		QString wind_t;
		QString wind_s;
		QString hmid;
		QString bar_r;
		QString dewp;
		QString vis;
		QString sunr;
		QString suns;
		QString lsup;
		bool fromContact;
	};

	QIcon *weatherPluginIcon;
	PluginSystemInterface *weatherPluginSystem;
	QString m_profile_name;

	TreeModelItem eventItem;
	
	weatherSettings *m_settings_widget;

	QHash<QString, weather> cityList;
	QNetworkAccessManager *netMan;

	QLabel *m_menu_label;
	QWidgetAction *m_menu_title;
	QMenu *m_item_menu;

	int interval;
	bool showstatus;

	QTimer *timer;

	qint16 m_event_init,
		m_event_context,
		m_event_tooltip,
		m_event_additional_info,
		m_event_receivelevel3;
};

#endif
