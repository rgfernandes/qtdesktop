/*
 * qip.cpp
 * 
 * Copyright (C) 2008  Nigmatullin Ruslan
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
*/

#include "qip.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include <qutim/iconmanagerinterface.h>

namespace HistoryManager {

qip::qip()
{
}

qip::~qip()
{
}

void qip::loadMessages(const QString &path)
{
	QDir dir = path;
	QString account = dir.dirName();
	if(!dir.cd("History"))
		return;
	setProtocol("ICQ");
	setAccount(account);
	QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);
	setMaxValue(files.size());
	for(int i = 0; i < files.size(); i++)
	{
		setValue(i + 1);
		if(files[i] == "_srvlog.txt"  || files[i] == "_botlog.txt" || files[i].startsWith("."))
			continue;
		QFile file(dir.filePath(files[i]));
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			Message message;
			message.type = 1;
			QTextStream in(&file);
			in.setAutoDetectUnicode(false);
			in.setCodec("cp1251");
			QString uin = files[i];
			uin.chop(4);
			setContact(uin);
			message.in = true;
			QDateTime date;
			QString from;
			QString msg;
			bool first = true;
			bool point = false;
			while(!in.atEnd())
			{
				QString line = in.readLine();
				if(line == "-------------------------------------->-"
				   || line == "--------------------------------------<-")
				{
					if(!first)
					{
						message.text.chop(10);
						appendMessage(message);
					}
					else
						first=false;
					if(line[38] == '<')
						message.in = true;
					else
						message.in = false;
					point = true;
					message.text.clear();
				}
				else if(point)
				{
					if(account.isEmpty())
						account = line.section(' ',0,-3);
					message.time = QDateTime().fromString(line.section(' ',-2),"(hh:mm:ss d/MM/yyyy)");
					point=false;
				}
				else
				{
					message.text += Qt::escape(line);
					message.text += "<br/>";
				}
			}
		}
	}
}

bool qip::validate(const QString &path)
{
	QDir dir = path;
	if(!dir.cd("History"))
		return false;
	QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);
	return !files.isEmpty();
}

QString qip::name()
{
	return "QIP 2005";
}

QIcon qip::icon()
{
	return qutim_sdk_0_2::Icon("qip", qutim_sdk_0_2::IconInfo::Client);
}

}
