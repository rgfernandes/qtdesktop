/*
    connectioncheck.cpp

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

#include "connectioncheck.h"

//Инициализация плагина
bool connectioncheckPlugin::init ( PluginSystemInterface *plugin_system )
{
    qRegisterMetaType<TreeModelItem> ( "TreeModelItem" );

    PluginInterface::init ( plugin_system );
    m_plugin_icon = new QIcon ( ":/icons/connectioncheck.png" );
    m_plugin_system = plugin_system;

	// Экзкмпляр обработчика событий
	eventHandler = new Eventer(this);

	// Вешаемся на евент установки статуса ICQ
	m_set_status_icq = m_plugin_system->registerEventHandler( "ICQ/Account/Status/Set" );
	// Вешаемся на евент восстановления статуса ICQ
	m_restore_status_icq = m_plugin_system->registerEventHandler("ICQ/Account/Status/Restore");
	// Вешаемся на евент установки статуса джаббера
	m_set_status_jabber = m_plugin_system->registerEventHandler( "Jabber/Account/Status/Change" );

	// Создаем таймер и привязываем его к нашему слоту
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));

	m_process_route = 0L;
	m_process_ping = 0L;
	m_connected = true;
	disconnectedFromUs = false;

	return true;
}

PluginSystemInterface *connectioncheckPlugin::getMainPluginSystemPointer()
{
	return m_plugin_system;
}

void connectioncheckPlugin::release()
{

}

// Создание виджета настроек
QWidget *connectioncheckPlugin::settingsWidget()
{
	connectioncheckSettingsWidget = new connectioncheckSettings(m_profile_name);
	return connectioncheckSettingsWidget;
}

// Получаем имя активного профиля
void connectioncheckPlugin::setProfileName ( const QString &profile_name )
{
    m_profile_name = profile_name;
}

// Имя плагина
QString connectioncheckPlugin::name()
{
    return "Connection check";
}

// Описание плагина
QString connectioncheckPlugin::description()
{
    return "Plugin for check network connection status";
}

// Возвращаем иконку плагина
QIcon *connectioncheckPlugin::icon()
{
    return m_plugin_icon;
}

// Тип плагина
QString connectioncheckPlugin::type()
{
    return "simple";
}


// Удаление виджета настроек
void connectioncheckPlugin::removeSettingsWidget()
{
	delete settingsWidget();
}

// Сохранение настроек
void connectioncheckPlugin::saveSettings()
{
	connectioncheckSettingsWidget->saveSettings();
    load();
}

// Загрузка настроек и включение/выключение плагина
void connectioncheckPlugin::load()
{
	timer->stop();

	routeOk = true;
	pingOk = true;

	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "connectioncheck");

	period = settings.value("period", 10).toInt();

	routecheck = settings.value("routecheck", true).toBool();
	pingcheck = settings.value("pingcheck", true).toBool();

	if ( settings.value("enabled",true).toBool() )
	{
		qDebug("Enable network checking");
		timer->start(period*1000);
	}
	else
		qDebug("Disable network checking");
}

// Проверка по таблице роутинга
void connectioncheckPlugin::routeCheck()
{
	// Вызываем внешний netstat и парсим его вывод, надо бы переделать :)

	if ( m_process_route )
	{
		qDebug("Previous netstat process is still running!");
		return;
	}

	m_buffer_route = QString::null;

	QStringList params;
	params.append("-r");

	m_process_route = new QProcess( this );
	connect( m_process_route, SIGNAL(readyReadStandardOutput()), this, SLOT(slotProcessStdoutRoute()) );
	connect( m_process_route, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotProcessExitRoute()) );
	m_process_route->start("netstat", params);
}

// Проверка пингами
void connectioncheckPlugin::pingCheck()
{
	//Бля хз как пинговать прямо отсюда, поэтому будем пока вызывать внешний пинг и парсить его вывод. 
	// Пинговать будем гугль :)

	if ( m_process_ping )
	{
		qDebug("Previous ping process is still running!");
		return;
	}

	QStringList params;
	params.append("-c 1");
	params.append("-w 9");
	params.append("209.85.129.99");

	m_buffer_ping = QString::null;

	m_process_ping = new QProcess( this );
	connect( m_process_ping, SIGNAL(readyReadStandardOutput()), this, SLOT(slotProcessStdoutPing()) );
	connect( m_process_ping, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotProcessExitPing()) );
	m_process_ping->start("ping",params);
}

// Слот вызываемый по таймеру
void connectioncheckPlugin::timerTimeOut()
{
	Ok = routeOk && pingOk;
	setConnectedStatus(Ok);

	if ( pingcheck )
	{
		pingCheck();
	}

	if ( routecheck )
	{
		routeCheck();
	}
}

void connectioncheckPlugin::slotProcessExitRoute()
{
	m_buffer_route = QString::null;
	delete m_process_route;
	m_process_route = 0L;
}

void connectioncheckPlugin::slotProcessExitPing()
{
	m_buffer_ping = QString::null;
	delete m_process_ping;
	m_process_ping = 0L;
}

// Парсинг вывода для ping проверки
void connectioncheckPlugin::slotProcessStdoutPing()
{
	m_buffer_ping += m_process_ping->readAllStandardOutput();
	pingOk = m_buffer_ping.contains( "time=" );
}

// Парсинг вывода для route проверки
void connectioncheckPlugin::slotProcessStdoutRoute()
{
	m_buffer_route += m_process_route->readAllStandardOutput();
	routeOk = m_buffer_route.contains( "default" );
}

void connectioncheckPlugin::setConnectedStatus(bool connected)
{
	// qDebug() << "connectioncheckPlugin::setConnectedStatus";

	TreeModelItem item;
	QString message;
	
	item.m_protocol_name = "connectioncheckPlugin";
	item.m_account_name = "Connection check plugin";
	item.m_parent_name = "test";
	item.m_item_name = "test";
	item.m_item_type = 0;

	QStringList iAccounts;
	int iStatus;
	QString iStatusText;
	QString jStatus;

	if ( connected && !m_connected )
	{
		m_connected = true;

		// Показываем нотификацию
		message = "Network is active! Auto reconnect.";
		m_plugin_system->systemNotifiacation(item, message);

		// Отсылаем восстановить статус всем аськам
		Event iEvent(m_restore_status_icq, 1, &iAccounts);
		m_plugin_system->sendEvent(iEvent);

		QStringList jAcc = accountData.keys();
		foreach (QString acc, jAcc)
		{
			QStringList i;
			i.append(acc);
			Event jEvent(m_set_status_jabber, 2, &i, &accountData.value(acc));
			m_plugin_system->sendEvent(jEvent);
		}
	}
	else if ( !connected && m_connected )
	{
		m_connected = false;

		// Показываем нотификацию
		message = "Network is not active! Set offline.";
		m_plugin_system->systemNotifiacation(item, message);

		iStatus = 12;
		jStatus = "offline";

		// Отсылаем дисконнект всем аськам
		Event iEvent(m_set_status_icq, 3, &iAccounts, &iStatus, &iStatusText);
		m_plugin_system->sendEvent(iEvent);

		disconnectedFromUs = true;
		QStringList jAcc = accountData.keys();
		Event jEvent(m_set_status_jabber, 2, &jAcc, &jStatus);
		m_plugin_system->sendEvent(jEvent);
	}
}

Q_EXPORT_PLUGIN2 ( connectioncheck,connectioncheckPlugin );
