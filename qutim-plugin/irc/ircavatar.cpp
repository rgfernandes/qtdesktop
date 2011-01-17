/*
		ircAvatar

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

#include "ircavatar.h"

ircAvatar::ircAvatar(const QString &account_name, const QString &profile_name, QObject *parent) : QObject(parent),
	m_account_name(account_name), m_profile_name(profile_name)
{
}

ircAvatar::~ircAvatar() {
}

void ircAvatar::requestForAvatar(const QString &buddy_id, const QString &avatar_url)
{
	if (QUrl(avatar_url).isValid() && !m_request_list.contains(buddy_id)) {
		QNetworkAccessManager *manager = new QNetworkAccessManager(this);
		connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
		manager->setProperty("avatar_url",avatar_url);
		manager->setProperty("buddy_id",buddy_id);
		manager->get(QNetworkRequest(QUrl(avatar_url)));
		m_request_list.insert(buddy_id, manager);
	}
}

void ircAvatar::replyFinished(QNetworkReply *reply)
{
	QNetworkAccessManager *manager = reply->manager();
	if (reply->rawHeader("Content-Length").toInt()<256000) {
		if (m_request_list.contains(manager->property("buddy_id").toString())) {
			QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/IRC."+m_account_name, "contactlist");
			QDir dir(settings.fileName().section('/', 0, -3) + "/ircicons");
			if (!dir.exists()) dir.mkdir(dir.path());
			QFile file(dir.path() + "/" + QCryptographicHash::hash(manager->property("avatar_url").toString().toUtf8(), QCryptographicHash::Md5).toHex());
			if (!file.exists())	{
					if (!file.open(QIODevice::WriteOnly))	return;
				file.write(reply->readAll());
				file.close();
			}
			emit avatarDownloaded(manager->property("buddy_id").toString(),
														QCryptographicHash::hash(manager->property("avatar_url").toString().toUtf8(),
														 QCryptographicHash::Md5));
			m_request_list.remove(manager->property("buddy_id").toString());
		}
	}
}
