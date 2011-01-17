/*
    connectioncheck.h

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

#ifndef CONNECTIONCHECK_H
#define CONNECTIONCHECK_H

#include "qutim/plugininterface.h"

#include <QDebug>
#include <QAction>
#include <QTimer>
#include <QProcess>

using namespace qutim_sdk_0_2;

typedef struct accData
{
	QString account;
	QString status;
} accData;

typedef QHash<QString, QString> AccountData;

#include "connectionchecksettings.h"
#include "eventer.h"

class Eventer;

// Класс плагина
class connectioncheckPlugin : public QObject, SimplePluginInterface
{
    Q_OBJECT
    Q_INTERFACES ( qutim_sdk_0_2::PluginInterface )
public:
    bool init ( PluginSystemInterface *plugin_system );
    void release();

    virtual QWidget *settingsWidget();
    virtual void setProfileName ( const QString &profile_name );
    QString name(); 
    QString description(); 
    QString type(); 
    QIcon *icon(); 
    virtual void removeSettingsWidget();
    virtual void saveSettings();
	
	Eventer *eventHandler;

	// Загрузка настроек и включение/выключение плагина
	void load();

	PluginSystemInterface *getMainPluginSystemPointer();
	AccountData accountData;
	bool disconnectedFromUs;

private slots:
	// Слот вызываемый по таймеру
	void timerTimeOut();

	void slotProcessStdoutRoute();
	void slotProcessStdoutPing();

	void slotProcessExitPing();
	void slotProcessExitRoute();
	

private:
	// Передача команды на установку статуса плагинам
	void setConnectedStatus(bool connected);
	// Проверка по таблице роутинга
	void routeCheck();
	// Проверка пингами
	void pingCheck();

    QIcon *m_plugin_icon;
    PluginSystemInterface *m_plugin_system;
    QString m_profile_name;
    QString m_account_name;
	connectioncheckSettings *connectioncheckSettingsWidget;

	QTimer *timer;
	QProcess *m_process_route;
	QProcess *m_process_ping;

	QString m_buffer_route;
	QString m_buffer_ping;

	bool m_connected;
	// Период проверки
	int period;

	// Надо ли проверять по таблице
	bool routecheck;
	// Надо ли проверять пингами
	bool pingcheck;

	bool Ok,
		routeOk,
		pingOk;

	// Идентификаторы событий
	quint16 m_set_status_icq,
		m_set_status_jabber,
		m_restore_status_icq,
		m_send_notification;
};


#endif
