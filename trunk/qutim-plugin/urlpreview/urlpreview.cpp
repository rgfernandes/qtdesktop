/*
    urlpreviewPlugin

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

#include "urlpreview.h"

bool urlpreviewPlugin::init(PluginSystemInterface *plugin_system)
{
	qRegisterMetaType<TreeModelItem>("TreeModelItem");

	PluginInterface::init(plugin_system);
	m_plugin_icon = new QIcon(":/icons/urlpreview-plugin.png");
	m_plugin_system = plugin_system;

	netman = new QNetworkAccessManager();
	connect(netman, SIGNAL(finished(QNetworkReply*)), this, SLOT(netmanFinished(QNetworkReply*)));

	m_event_msgin = m_plugin_system->registerEventHandler("Core/ChatWindow/ReceiveLevel3", this);
	m_event_msgout = m_plugin_system->registerEventHandler("Core/ChatWindow/SendLevel1", this);
	
	return true;
}

QIcon *urlpreviewPlugin::icon()
{
	return m_plugin_icon;
}

void urlpreviewPlugin::saveSettings()
{
	settingswidget->saveSettings();
}

void urlpreviewPlugin::loadSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "plugin_urlpreview");

	QString template_default = "<br><b>"+tr("URL Preview")+"</b>: <i>%TYPE%, %SIZE% "+tr("bytes")+"</i>";
	if (settings.value("template").isNull()) settings.setValue("template", template_default);

	QString template_image_default = "<br><img src=\"%URL%\" style=\"display: none;\" onload=\"if (this.width>%MAXW%) this.style.width='%MAXW%px'; if (this.height>%MAXH%) { this.style.width=''; this.style.height='%MAXH%px'; } this.style.display='';\">";
	if (settings.value("image/template").isNull()) settings.setValue("image/template", template_image_default);

	cfg_enable_in = settings.value("enable_in", true).toBool();
	cfg_enable_out = settings.value("enable_out", false).toBool();
	cfg_disable_text_html = settings.value("disable_text_html", true).toBool();
	cfg_str["template"] = settings.value("template", template_default).toString();
	cfg_image_maxfilesize = settings.value("image/maxfilesize", 100000).toUInt();
	cfg_str["image_maxwidth"] = settings.value("image/maxwidth", "400").toString();
	cfg_str["image_maxheight"] = settings.value("image/maxheight", "600").toString();
	cfg_str["image_template"] = settings.value("image/template", template_image_default).toString();
}

QWidget *urlpreviewPlugin::settingsWidget()
{
	settingswidget = new urlpreviewSettings(m_profile_name);
	connect(settingswidget, SIGNAL(settingsChanged()), this, SLOT(loadSettings()));
	return settingswidget;
}

void urlpreviewPlugin::removeSettingsWidget()
{
	delete settingsWidget();
}

void urlpreviewPlugin::setProfileName(const QString &profile_name)
{
	m_profile_name = profile_name;
	loadSettings();
}


void urlpreviewPlugin::processEvent(Event &event)
{
	if ((event.id==m_event_msgin && cfg_enable_in) ||
			(event.id==m_event_msgout && cfg_enable_out)) {
		TreeModelItem item = *(TreeModelItem*)(event.args.at(0));
		QString *msg = (QString*)(event.args.at(1));

		QRegExp urlRegExp("(\\<a\\s+href='([^'\\s]+)[^<]+</a>)");
		int pos = 0;
		QString smsg = *msg;
		QString rmsg = *msg;
		while ((pos = urlRegExp.indexIn(smsg, pos)) != -1) {
			QString url = urlRegExp.cap(2);
			QString uid;
			while (uid.isEmpty() || uidurls.contains(uid)) {
				uid = QString::number(rand() % 0x8000);
			}
			uidurls[uid] = url;
			uiditems[uid] = item;

			rmsg.replace(urlRegExp.cap(1), urlRegExp.cap(1)+"<span id='urlpreview"+uid+"'></span>");
			pos += urlRegExp.matchedLength();

			QNetworkRequest netreq;
			netreq.setUrl(QUrl(url));
			netreq.setRawHeader("Ranges", "bytes=0-0");
			netman->head(netreq);

//			qDebug()<<"URLPreview request"<<url<<uid<<rmsg;
//			qDebug()<<"URLPreview event"<<item.m_account_name<<item.m_item_history<<item.m_item_type<<item.m_parent_name<<item.m_protocol_name;
		}
		*msg = rmsg;
	}
}

void urlpreviewPlugin::netmanFinished( QNetworkReply* reply )
{
	QString type = reply->rawHeader("Content-Type");
	quint64 size=0;
	if (reply->rawHeaderList().contains("Content-Range")) {
		QString rangestr = reply->rawHeader("Content-Range");
		QRegExp rangerx("/(\\d+)");
		if (rangerx.indexIn(rangestr)>-1) {
			size = rangerx.cap(1).toInt();
		}
	}
	else if (reply->rawHeaderList().contains("Content-Length")) {
		size = reply->rawHeader("Content-Length").toInt();
	}

	if (cfg_disable_text_html) {
		QRegExp rx("^text/html");
		if (rx.indexIn(type)==0) {
			type.clear();
		}
	}

	if (!type.isEmpty() && size>0) {
		QNetworkRequest netreq = reply->request();
		foreach (QString uid, uidurls.keys(netreq.url().toString())) {
			QString url = netreq.url().toString();

			if (uidurls[uid]==url) {
				QString pstr = cfg_str["template"];
				pstr.replace("%TYPE%", type);
				pstr.replace("%SIZE%", QString::number(size));

				QRegExp rx("^image/");
				if (rx.indexIn(type)==0 && size<cfg_image_maxfilesize) {
					QString amsg = cfg_str["image_template"];
					amsg.replace("%URL%", url);
					amsg.replace("%UID%", uid);
					amsg.replace("%MAXW%", cfg_str["image_maxwidth"]);
					amsg.replace("%MAXH%", cfg_str["image_maxheight"]);
					pstr += amsg;
				}

				QString js = "urlpreview"+uid+".innerHTML = \""+pstr.replace("\"", "\\\"")+"\";";
				TreeModelItem item = uiditems[uid];
				quint16 event_evaljs_id = SystemsCity::PluginSystem()->registerEventHandler("Core/ChatWindow/EvaluateJavaScript");

//				qDebug()<<"URLPreview reply"<<item.m_account_name<<item.m_item_history<<item.m_item_type<<item.m_parent_name<<item.m_protocol_name;

				if (item.m_item_type==34) {
					item.m_item_type = 32;
					item.m_item_name = item.m_parent_name;
					item.m_parent_name = item.m_account_name;
				}

				Event(event_evaljs_id, 2, &item, &js).send();

				uidurls[uid].clear();
				uiditems.remove(uid);
			}

		}
	}

	delete reply;
}


Q_EXPORT_PLUGIN2(urlpreviewPlugin,urlpreviewPlugin);
