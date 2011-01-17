/*
		twApiWrap

		Copyright (c) 2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include <QSettings>
#include <QNetworkCookieJar>
#include <QMessageBox>
#include <QTextDocument>

#include "twpluginsystem.h"
#include "twapiwrap.h"

twApiWrap::twApiWrap(const QString &account_name, const QString &profile_name) :
	m_account_name(account_name),
	m_profile_name(profile_name)
{
	m_network_handler = new QNetworkAccessManager(this);
	connect(m_network_handler, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReply(QNetworkReply*)));
	m_current_state = Offline;
	loadSettings();
}

twApiWrap::~twApiWrap()
{
	delete m_network_handler;
}

void twApiWrap::loadSettings()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/twitter."+m_account_name, "accountsettings");
	QString username = settings.value("main/name").toString();
	QString password = settings.value("main/password").toString();

	if (!username.isNull() && !password.isNull())
		m_network_request.setRawHeader("Authorization", "Basic " +
			 QByteArray(QString("%1:%2").arg(username).arg(password).toAscii()).toBase64());

	m_network_request.setRawHeader("User-Agent", "qutIM Twitter plugin");
	m_network_request.setRawHeader("Accept-Charset", "utf-8");
	m_network_request.setRawHeader("Pragma", "no-cache");
	m_network_request.setRawHeader("Cache-control", "no-cache");

	m_check_every = settings.value("main/check_every", 600).toUInt();
	m_last_direct_message_id = settings.value("main/last_direct_message_id").toString();

}

void twApiWrap::startRequests()
{
	m_current_state = Online;
	emit iMConnected();
	sendRequests();
}

void twApiWrap::sendRequests()
{
	if (m_check_every && m_current_state == Online) {
		m_network_request.setUrl(QUrl("http://twitter.com/statuses/friends.json?callback=friends"));
		m_network_handler->get(m_network_request);

		m_network_request.setUrl(QUrl("http://twitter.com/statuses/followers.json?callback=followers"));
		m_network_handler->get(m_network_request);

		m_network_request.setUrl(QUrl("http://twitter.com/direct_messages.json?since_id="+m_last_direct_message_id+"&callback=direct_messages"));
		m_network_handler->get(m_network_request);

		QTimer::singleShot(m_check_every * 1000, this, SLOT(sendRequests()));
	}
}

void twApiWrap::stopAll()
{
	m_current_state = Offline;
	emit iMDisconnected();
}

void twApiWrap::getReply(QNetworkReply *reply)
{
	QString data = reply->readAll();
	QRegExp rxcb("^(\\w+)\\((.+)\\)\\;?$");
	QString content;
	if (rxcb.indexIn(data)==0) {
		QString callback = rxcb.cap(1);
		content = rxcb.cap(2);

		QScriptEngine sc_engine;
		QScriptValue sc_data = sc_engine.evaluate(content);
		if (sc_data.property("error").isValid())
			twPluginSystem::instance().sendSystemMessage(m_account_name, tr("Twitter protocol error:")+"<br>"+ sc_data.property("error").toString());
		else {
			if (callback=="friends"
				|| callback=="followers")
							processBuddies(content, callback);
			else if (callback=="direct_messages") {
				processDirectMessages(content);
			}
			else
				qDebug()<<"Twitter API unknown reply"<<data;
		}
	}
	else content = data;

	delete reply;
}

void twApiWrap::processBuddies(QString data, QString callback)
{
	QScriptEngine sc_engine;
	QScriptValue sc_data = sc_engine.evaluate(data);

	QList<Buddy> buddies;
	QHash<QString, BuddyStatus> buddy_statuses;

	int i=0;
	while (sc_data.property(i).property("id").isValid()) {
		QScriptValue sc_item = sc_data.property(i);

		Buddy buddy;
		buddy.id = sc_item.property("id").toString();
		buddy.name = Qt::escape(sc_item.property("name").toString());
		buddy.screen_name = Qt::escape(sc_item.property("screen_name").toString());
		buddy.location = Qt::escape(sc_item.property("location").toString());
		buddy.description = Qt::escape(sc_item.property("description").toString());
		buddy.profile_image_url = sc_item.property("profile_image_url").toString();
		buddy.url = sc_item.property("url").toString();
		buddy.profile_image_url = sc_item.property("profile_image_url").toString();
		buddy.followers_count = sc_item.property("followers_count").toNumber();
		buddy.friends_count = sc_item.property("friends_count").toNumber();
		buddy.favourites_count = sc_item.property("favourites_count").toNumber();
		buddy.statuses_count = sc_item.property("statuses_count").toNumber();

		if (sc_item.property("status").property("id").isValid()) {
			buddy.status_id = sc_item.property("status").property("id").toString();
			buddy.status_text = Qt::escape(sc_item.property("status").property("text").toString());
		}

		buddies.append(buddy);

		i++;
	}

	if (buddies.count()) emit arrivedBuddies(buddies, callback);

}

void twApiWrap::processDirectMessages(QString data) {
	QScriptEngine sc_engine;
	QScriptValue sc_data = sc_engine.evaluate(data);

	QList<DirectMessage> direct_messages;

	int i=0;
	while (sc_data.property(i).property("id").isValid()) {
		QScriptValue sc_item = sc_data.property(i);
		if (i==0) m_last_direct_message_id = sc_item.property("id").toString();

		DirectMessage direct_message;
		direct_message.id = sc_item.property("id").toString();
		direct_message.sender = sc_item.property("sender").property("screen_name").toString();
		direct_message.text = Qt::escape(sc_item.property("text").toString());
		direct_message.datetime = QDateTime::currentDateTime(); // fromString(sc_item.property("created_at").toString());
		direct_messages.append(direct_message);

		i++;
	}

	emit arrivedDirectMessages(direct_messages);

	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/twitter."+m_account_name, "accountsettings");
	settings.setValue("main/last_direct_message_id", m_last_direct_message_id);

}

void twApiWrap::sendMessage(const QString &buddy_id, const QString &message) {
		QByteArray postdata = "user="+buddy_id.toUtf8()+"&text="+message.toUtf8();
		m_network_request.setUrl(QUrl("http://twitter.com/direct_messages/new.json"));
		m_network_handler->post(m_network_request, postdata);
}
