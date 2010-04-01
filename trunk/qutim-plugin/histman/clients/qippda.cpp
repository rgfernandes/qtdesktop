/*
 * qippda.cpp
 * 
 * Copyright (C) 2008  Nigmatullin Ruslan
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
*/

#include "qippda.h"
#include "qipinfium.h"
#include <QDir>
#include <QFileInfo>
#include <QTextDocument>
#include <qutim/iconmanagerinterface.h>

namespace HistoryManager {

qippda::qippda()
{
}

qippda::~qippda()
{
}

void qippda::loadMessages(const QString &path)
{
	QDir dir = path;
	if(!dir.cd("History"))
		return;
	QHash<QString,QString> protocols;
	protocols[QLatin1String("icq")]    = QLatin1String("ICQ");
	protocols[QLatin1String("jabber")] = QLatin1String("Jabber");
	protocols[QLatin1String("mra")]    = QLatin1String("MRIM");
	QStringList filters = QStringList() << QLatin1String("*.qhf") << QLatin1String("*.ahf");
	QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
	setMaxValue(files.size());
	for(int i = 0; i < files.size(); i++)
	{
		setValue(i + 1);
		QString protocol = files[i].fileName().section("_",0,0);
		while(!protocol.isEmpty() && protocol.at(protocol.length() - 1).isDigit())
			protocol.chop(1);
		// Old qip pda has only ICQ support and files are named like 1_myuin_hisuin
		if(protocol.isEmpty())
			protocol = QLatin1String("ICQ");
		else
		{
			protocol = protocols[protocol.toLower()];
			if(protocol.isEmpty())
			{
				qWarning("Unknown protocol: \"%s\"", qPrintable(files[i].fileName()));
				continue;
			}
		}
		setProtocol(protocol);
		QString account = files[i].fileName().section("_",1,1);
		setAccount(account);
		QFile file(files[i].absoluteFilePath());
		if(file.open(QFile::ReadOnly))
		{
			QByteArray bytearray = file.readAll();
			const uchar *data = (const uchar *)bytearray.constData();
			const uchar *end = data + bytearray.size();
			if(memcmp(data, "QHF", 3))
				continue;
			uchar version = data[3];
			data += 44;
			QString uin = qipinfium::getString(data, qipinfium::getUInt16(data));
			QString name = qipinfium::getString(data, qipinfium::getUInt16(data));
			setContact(uin);
//			continue;
			while(data < end)
			{
				quint16 type = qipinfium::getUInt16(data);
				quint32 index = qipinfium::getUInt32(data);
				data += 2;
				const uchar *next = data + index;
				if(type == 1)
				{
					Message message;
					message.type = 1;
					data += 10;
					message.time = QDateTime::fromTime_t(qipinfium::getUInt32(data));
					message.time.setTimeSpec(Qt::LocalTime);
					if(version == 1)
						message.time = message.time.toUTC().addDays(2);
					else
						message.time = message.time.toUTC();
					data += 4;
					message.in = qipinfium::getUInt8(data) == 0;
					data += 4;
					int mes_len = version == 3 ? qipinfium::getUInt32(data) : qipinfium::getUInt16(data);
					QString text = qipinfium::getString(data, mes_len, version > 1);
					message.text = Qt::escape(text).replace("\n", "<br/>");
					appendMessage(message);
				}
				else
					data = next;
			}
		}
	}
}

bool qippda::validate(const QString &path)
{
	QDir dir = path;
	if(!dir.cd("History"))
		return false;
	QStringList files = dir.entryList(QStringList() << "*.qhf" << "*.ahf", QDir::Files);
	return !files.isEmpty();
}

QString qippda::name()
{
	return "QIP PDA";
}

QIcon qippda::icon()
{
	return qutim_sdk_0_2::Icon("qippda", qutim_sdk_0_2::IconInfo::Client);
}

}
