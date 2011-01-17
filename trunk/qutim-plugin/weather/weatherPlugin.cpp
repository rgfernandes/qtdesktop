/*
    weatherPlugin.cpp

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

#include "weatherPlugin.h"

bool weatherPlugin::init(PluginSystemInterface *plugin_system)
{
	qRegisterMetaType<TreeModelItem>("TreeModelItem");

	PluginInterface::init(plugin_system);
	weatherPluginIcon = new QIcon(":/icons/weather.png");
	weatherPluginSystem = plugin_system;

	setMsgTemplate();

	m_event_init =  weatherPluginSystem->registerEventHandler("Core/Layers/Initialized", this);
	m_event_context =  weatherPluginSystem->registerEventHandler("Core/ContactList/ContactContext", this);
	m_event_tooltip = weatherPluginSystem->registerEventHandler("Core/ContactList/AskTooltip", this);
	m_event_additional_info = weatherPluginSystem->registerEventHandler("Core/ContactList/AskItemAdditionalInfo", this);
	m_event_receivelevel3 = weatherPluginSystem->registerEventHandler("Core/ChatWindow/ReceiveLevel3", this);

	QAction *getNowAction = new QAction(*weatherPluginIcon, "Get weather now", this);
	connect(getNowAction, SIGNAL(triggered()), this,  SLOT(actionGetNow()));

	netMan = new QNetworkAccessManager();
	connect(netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(getWeatherFinished(QNetworkReply*)));


	m_menu_label = new QLabel;
	m_menu_label->setAlignment(Qt::AlignCenter);
	m_menu_title = new QWidgetAction(this);
	m_menu_title->setDefaultWidget(m_menu_label);

	m_item_menu = new QMenu();
	m_item_menu->addAction(m_menu_title);
	m_item_menu->addAction(getNowAction);

	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));

	return true;
}

void weatherPlugin::setMsgTemplate()
{
	msgTemplate = QString::fromLocal8Bit(
		"<table width=100% border=0>"
		"<tr>"
		"<td width=15% rowspan=8 align=\"center\"><img src=\"%1\"/><br/>%2</td>"
		"<td width=15% rowspan=8 align=\"center\"><h1>%3 °%16</h1>Feels Like<br/>%4</td>"
		"<td width=15% align=\"right\">UV Index:</td>"
		"<td width=55%>%5 %6</td>"
		"</tr>"
		"<tr>"
		"<td align=\"right\">Wind:</td>"
		"<td>From %7 at %8 km/h</td>"
		"</tr>"
		"<tr>"
		"<td align=\"right\">Humidity:</td>"
		"<td>%9</td>"
		"</tr>"
		"<tr>"
		"<td align=\"right\">Pressure:</td>"
		"<td>%10 mb.</td>"
		"</tr>"
		"<tr>"
		"<td align=\"right\">Dew Point:</td>"
		"<td>%11 °%16</td>"
		"</tr>"
		"<tr>"
		"<td align=\"right\">Visibility:</td>"
		"<td>%12 kilometers</td>"
		"</tr>"
		"<tr>"
		"<td align=\"right\">Sunrise:</td>"
		"<td>%13</td>"
		"</tr>"
		"<tr>"
		"<td align=\"right\">Sunset:</td>"
		"<td>%14</td>"
		"</tr>"
		"<tr><td colspan=3 width=100% align=\"center\">Updated: %15</td></tr>"
		"</table>"
	);
}

void weatherPlugin::release()
{
}

void weatherPlugin::processEvent(Event &event)
{
	if (event.id == m_event_init)
		load();
	else if (event.id == m_event_context)
	{
		eventItem = *(TreeModelItem *)(event.args.at(0));
		if (eventItem.m_protocol_name == "weatherPlugin" && eventItem.m_item_type == 0)
		{
			weather o_weather = cityList.value(eventItem.m_item_name);
			m_menu_label->setText(QString("<b>%1</b>").arg(o_weather.dnam));
			m_item_menu->popup(QCursor::pos());
		}
	}
	else if (event.id == m_event_receivelevel3)
	{
		TreeModelItem item = *(TreeModelItem *)(event.args.at(0));
		QString *msg = (QString *)(event.args.at(1));
		if (item.m_protocol_name == "weatherPlugin")
		{
			msg->replace("&lt;", "<");
			msg->replace("&gt;", ">");
		}
	}
	else if (event.id == m_event_tooltip)
	{
		TreeModelItem item = *(TreeModelItem *)(event.args.at(0));
		QString *tooltip = (QString *)(event.args.at(1));
		if (item.m_protocol_name == "weatherPlugin")
		{
			weather o_weather = cityList.value(item.m_item_name);
			QString s_weather = QString::fromLocal8Bit("Weather: %1 °%2");
			if (!o_weather.tmp.isEmpty() && !o_weather.ut.isEmpty())
				*tooltip = s_weather.arg(o_weather.tmp).arg(o_weather.ut);
			else
				*tooltip = "Weather not loaded yet";
		}
	}
	else if (event.id == m_event_additional_info)
	{
		TreeModelItem item = *(TreeModelItem *)(event.args.at(0));
		QStringList *info = (QStringList *)(event.args.at(1));
		if (item.m_protocol_name == "weatherPlugin")
		{
			if (item.m_item_name == "weather")
				*info << "You";
			else
			{
				weather o_weather = cityList.value(item.m_item_name);
				QString s_weather = QString::fromLocal8Bit("Weather: %1 °%2");
				if (!o_weather.dnam.isEmpty())
					*info << o_weather.dnam << QString() << "Weather v0.1" << item.m_item_name;
				else
					*info << item.m_item_name;
			}
		}
	}
}

QWidget *weatherPlugin::settingsWidget()
{
	m_settings_widget = new weatherSettings(m_profile_name);
	return m_settings_widget;
}

QString weatherPlugin::name()
{
	return "Weather";
}

QString weatherPlugin::description()
{
	return "Weather plugin";
}

QIcon *weatherPlugin::icon()
{
	return weatherPluginIcon;
}

QString weatherPlugin::type()
{
	return "other";
}

void weatherPlugin::setProfileName(const QString &profileName)
{
	m_profile_name = profileName;
}

void weatherPlugin::removeSettingsWidget()
{
	delete settingsWidget();
}

void weatherPlugin::saveSettings()
{
	m_settings_widget->saveSettings();

	TreeModelItem contact;
	contact.m_protocol_name = "weatherPlugin";
	contact.m_account_name = "weather";
	contact.m_parent_name = "groupWeatherPlugin";
	contact.m_item_type = 0;

	QStringList contacts = cityList.keys();
	foreach(QString cityId, contacts)
	{
		contact.m_item_name = cityId;
		emit weatherPluginSystem->removeItemFromContactList(contact);

		cityList.remove(cityId);
	}

	loadCityList();
}

void weatherPlugin::load()
{
	TreeModelItem contact;
	contact.m_protocol_name = "weatherPlugin";
	contact.m_account_name = "weather";
	contact.m_item_name = "accountWeatherPlugin";
	contact.m_item_type = 2;
	emit weatherPluginSystem->addItemToContactList(contact, "Weather");
	emit weatherPluginSystem->setAccountIsOnline(contact, true);

	contact.m_item_name = "groupWeatherPlugin";
	contact.m_parent_name = "accountWeatherPlugin";
	contact.m_item_type = 1;
	emit weatherPluginSystem->addItemToContactList(contact, "Cities");

	loadCityList();
}

void weatherPlugin::loadCityList()
{
	timer->stop();

	TreeModelItem contact;
	contact.m_protocol_name = "weatherPlugin";
	contact.m_account_name = "weather";
	contact.m_parent_name = "groupWeatherPlugin";
	contact.m_item_type = 0;

	QSettings settings(QSettings::defaultFormat(),QSettings::UserScope, "qutim/qutim."+m_profile_name, "weatherPlugin");
	int count = settings.value("main/countItems", 0).toInt();
	for (int i=0; i<count; i++)
	{
		weather o_weather;
		o_weather.dnam = settings.value("main/item_"+QString::number(i)).toString();
		o_weather.fromContact = false;
		cityList.insert(o_weather.dnam, o_weather);
		contact.m_item_name = o_weather.dnam;
		emit weatherPluginSystem->addItemToContactList(contact, o_weather.dnam);
		emit weatherPluginSystem->setContactItemStatus(contact, *weatherPluginIcon, "", 0);

		getWeather(o_weather.dnam);
	}

	interval = settings.value("main/interval", 25).toInt();
	showstatus = settings.value("main/showstatus", 2).toBool();
	timer->start(interval*60*1000);
}

void weatherPlugin::getWeather(QString cityId)
{
	netMan->get(QNetworkRequest(QUrl(QString("http://xoap.weather.com/weather/local/%1?cc=*&link=xoap&prod=xoap&par=1085658115&key=b3fcda23f931ed87&unit=m").arg(cityId))));
}

void weatherPlugin::getWeatherFinished(QNetworkReply *reply)
{
	QDomDocument doc;
	if(doc.setContent(reply->readAll()))
	{
		QDomElement rootElement = doc.documentElement();

		QString cityId = rootElement.firstChildElement("loc").toElement().attributes().namedItem("id").nodeValue();
		weather o_weather = cityList.value(cityId);

		QString dnam = rootElement.firstChildElement("loc").toElement().firstChildElement("dnam").toElement().text();
		if (o_weather.dnam != dnam)
		{
			o_weather.dnam = dnam;
			TreeModelItem contact;
			contact.m_protocol_name = "weatherPlugin";
			contact.m_account_name = "weather";
			contact.m_parent_name = "groupWeatherPlugin";
			contact.m_item_name = cityId;
			contact.m_item_type = 0;
			emit weatherPluginSystem->setContactItemName(contact, dnam);
		}

		o_weather.ut = rootElement.firstChildElement("head").toElement().firstChildElement("ut").toElement().text();
		o_weather.tmp = rootElement.firstChildElement("cc").toElement().firstChildElement("tmp").toElement().text();
		o_weather.icon = rootElement.firstChildElement("cc").toElement().firstChildElement("icon").toElement().text();
		if (o_weather.icon == "-")
			o_weather.icon = "25";
		o_weather.t = rootElement.firstChildElement("cc").toElement().firstChildElement("t").toElement().text();
		o_weather.flik = rootElement.firstChildElement("cc").toElement().firstChildElement("flik").toElement().text();
		o_weather.uv_i = rootElement.firstChildElement("cc").toElement().firstChildElement("uv").toElement().firstChildElement("i").toElement().text();
		o_weather.uv_t = rootElement.firstChildElement("cc").toElement().firstChildElement("uv").toElement().firstChildElement("t").toElement().text();
		o_weather.wind_t = rootElement.firstChildElement("cc").toElement().firstChildElement("wind").toElement().firstChildElement("t").toElement().text();
		o_weather.wind_s = rootElement.firstChildElement("cc").toElement().firstChildElement("wind").toElement().firstChildElement("s").toElement().text();
		o_weather.hmid = rootElement.firstChildElement("cc").toElement().firstChildElement("hmid").toElement().text();
		o_weather.bar_r = rootElement.firstChildElement("cc").toElement().firstChildElement("bar").toElement().firstChildElement("r").toElement().text();
		o_weather.dewp = rootElement.firstChildElement("cc").toElement().firstChildElement("dewp").toElement().text();
		o_weather.vis = rootElement.firstChildElement("cc").toElement().firstChildElement("vis").toElement().text();
		o_weather.sunr = rootElement.firstChildElement("loc").toElement().firstChildElement("sunr").toElement().text();
		o_weather.suns = rootElement.firstChildElement("loc").toElement().firstChildElement("suns").toElement().text();
		o_weather.lsup = rootElement.firstChildElement("cc").toElement().firstChildElement("lsup").toElement().text();

		cityList.insert(cityId, o_weather);
		setWeather(cityId);
	}
}

void weatherPlugin::setWeather(QString cityId)
{
	weather o_weather = cityList.value(cityId);
	QString s_weather = QString::fromLocal8Bit("Weather: %1 °%2");
	s_weather = s_weather.arg(o_weather.tmp).arg(o_weather.ut);

	QList<QVariant> list;
	list.append("     "+s_weather);

	TreeModelItem contact;
	contact.m_protocol_name = "weatherPlugin";
	contact.m_account_name = "weather";
	contact.m_parent_name = "groupWeatherPlugin";
	contact.m_item_name = cityId;
	contact.m_item_type = 0;
	if (showstatus)
		emit weatherPluginSystem->setContactItemRow(contact, list, 1);
	emit weatherPluginSystem->systemNotifiacation(contact, QString("%1:<br/>%2").arg(o_weather.dnam).arg(s_weather));
	emit weatherPluginSystem->setContactItemIcon(contact, QIcon(QString(":/icons/%1.png").arg(o_weather.icon)), 12);

	if (o_weather.fromContact)
	{
		o_weather.fromContact = false;
		cityList.insert(cityId, o_weather);

		s_weather = msgTemplate.arg(QString("qrc:/icons/%1.png").arg(o_weather.icon)).arg(o_weather.t).arg(o_weather.tmp).arg(o_weather.flik).arg(o_weather.uv_i).arg(o_weather.uv_t).arg(o_weather.wind_t).arg(o_weather.wind_s).arg(o_weather.hmid).arg(o_weather.bar_r).arg(o_weather.dewp).arg(o_weather.vis).arg(o_weather.sunr).arg(o_weather.suns).arg(o_weather.lsup).arg(o_weather.ut);

		emit weatherPluginSystem->addMessageFromContact(contact, s_weather, QDateTime::currentDateTime());
	}
}

void weatherPlugin::timerTimeOut()
{
	QStringList contacts = cityList.keys();
	foreach(QString cityId, contacts)
		getWeather(cityId);
}

void weatherPlugin::actionGetNow()
{
	QString cityId = eventItem.m_item_name;

	weather o_weather = cityList.value(cityId);
	o_weather.fromContact = true;
	cityList.insert(cityId, o_weather);

	getWeather(cityId);
}

Q_EXPORT_PLUGIN2(weatherPlugin, weatherPlugin);
