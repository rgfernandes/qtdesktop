/*
  Gajim

  Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "gajim.h"
#include <QFileInfo>
#include <QDir>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <qutim/iconmanagerinterface.h>

namespace HistoryManager {

gajim::gajim()
{
}

void gajim::loadMessages(const QString &path)
{
	QDir dir = path;
	QFileInfo info(dir.filePath("logs.db"));
	if(!info.exists())
		return;
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(info.absoluteFilePath());
	if(!db.open())
		return;
	setProtocol("Jabber");
	setAccount(m_account);
	QSqlQuery jid_query("SELECT jid_id, jid FROM jids", db);
	setMaxValue(jid_query.size());
	int num = 0;
	while(jid_query.next())
	{
		QString jid_id = jid_query.value(0).toString();
		QString jid = jid_query.value(1).toString();
		setContact(jid);
		static QString query_str = "SELECT time, message, kind FROM logs "
								   "WHERE jid_id = %1 AND (kind = 4 OR kind = 6) "
								   "ORDER BY time ASC";
		QSqlQuery query(query_str.arg(jid_id), db);
		while(query.next())
		{
			Message message;
			message.type = 1;
			message.time = QDateTime::fromTime_t(query.value(0).toInt());
			message.in = query.value(2).toInt() == 4;
			message.text = Qt::escape(query.value(1).toString()).replace("\n", "<br/>");
			appendMessage(message);
		}
		setValue(++num);
	}
}

bool gajim::validate(const QString &path)
{
	QDir dir = path;
	QFileInfo info(dir.filePath("logs.db"));
	return info.exists();
}

QString gajim::name()
{
	return "Gajim";
}

QIcon gajim::icon()
{
	return qutim_sdk_0_2::Icon("gajim", qutim_sdk_0_2::IconInfo::Client);
}

QList<ConfigWidget> gajim::config()
{
	return QList<ConfigWidget>() << (m_config = createAccountWidget("Jabber"));
}

bool gajim::useConfig()
{
	m_account = m_config.second->property("currentText").toString();
	return true;
}

QString gajim::additionalInfo()
{
	return QCoreApplication::translate("ClientConfigPage", "Select your Jabber account.");
}

}
