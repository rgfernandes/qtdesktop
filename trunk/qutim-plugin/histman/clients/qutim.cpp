/*
  qutIM

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

#include "qutim.h"
#include "QDir"
#include <QDateTime>
#include <QDomDocument>

#include <QDebug>
#include <qutim/iconmanagerinterface.h>

namespace HistoryManager {

qutim::qutim()
{
}

qutim::~qutim()
{
}

bool qutim::guessXml(const QString &path, QFileInfoList &files, int &num)
{
	QDir dir = path;
	if(!dir.cd("history"))
		return false;
	files = dir.entryInfoList(QStringList() << "*.*.xml", QDir::Readable | QDir::Files);
	num += files.size();
	return !files.isEmpty();
}

bool qutim::guessBin(const QString &path, QFileInfoList &files, int &num)
{
	QDir dir = path;
	if(!dir.cd("history"))
		return false;
	static const QStringList filter = QStringList() << "*.*.log";
	QFileInfoList accounts = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo &info, accounts)
	{
		QStringList logs = QDir(info.absoluteFilePath()).entryList(filter, QDir::Files);
		if(!logs.isEmpty())
		{
			num += logs.size();
			files << info;
		}
	}
	return !files.isEmpty();
}

bool qutim::guessJson(const QString &path, QFileInfoList &files, int &num)
{
	QDir dir = path;
	if(!dir.cd("history"))
		return false;
	static const QStringList filter = QStringList() << "*.*.json";
	QFileInfoList accounts = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo &info, accounts)
	{
		QStringList logs = QDir(info.absoluteFilePath()).entryList(filter, QDir::Files);
		if(!logs.isEmpty())
		{
			num += logs.size();
			files << info;
		}
	}
	return !files.isEmpty();
}

void qutim::loadXml(const QFileInfoList &files)
{
	QDir tmp_dir = files.first().absoluteDir();
	tmp_dir.cdUp();
	QString account = tmp_dir.dirName().section(".", 1);
	setProtocol("ICQ");
	setAccount(account);
	for(int i=0;i<files.size();i++)
	{
		setValue(++m_value);
		if(!files[i].fileName().startsWith("log.20"))
		{
			QFile file(files[i].absoluteFilePath());
			if (file.open(QIODevice::ReadOnly) )
			{
				QDomDocument doc;
				if(doc.setContent(&file))
				{
					QDomElement rootElement = doc.documentElement();
					int msgCount = rootElement.elementsByTagName("msg").count();
					QDomElement dateElement = rootElement.firstChildElement("date");
					QString otherDate = "/"+dateElement.attribute("month")+"/"+dateElement.attribute("year");
					QDomElement msg = rootElement.firstChildElement("msg");
					QString uin = files[i].fileName();
					uin.truncate(uin.indexOf('.'));
					setContact(uin);
					for ( int j = 0; j < msgCount ; j++ )
					{
						Message message;
						message.time = QDateTime().fromString(msg.attribute("time")+" "+msg.attribute("day")+otherDate,"h:m:s d/M/yyyy");
						message.text = msg.text();
						message.in = msg.attribute("in").compare("1")==0;
						message.type = 1;
						appendMessage(message);
						msg = msg.nextSiblingElement("msg");
					}

				}
			}
		}
	}
}

void qutim::loadBin(const QFileInfoList &acc_files)
{
	foreach(const QFileInfo &info, acc_files)
	{
		QString protocol = info.fileName().section(".",0,0);
		QString account_name = QString().fromUtf8(QByteArray::fromHex(info.fileName().section(".",1,1).toLatin1()));
		setProtocol(protocol);
		setAccount(account_name);
		QDir dir(info.absoluteFilePath());
		QFileInfoList files = dir.entryInfoList(QDir::Readable | QDir::Files | QDir::NoDotAndDotDot,QDir::Name);
		for(int i=0;i<files.size();i++)
		{
			setValue(++m_value);
			if(!files[i].fileName().startsWith("sys."))
			{
				QFile file(files[i].absoluteFilePath());
				if (file.open(QIODevice::ReadOnly))
				{
					QString uin = files[i].fileName().section(".",0,0);
					uin = QString().fromUtf8(QByteArray::fromHex(uin.toLatin1()));
					setContact(uin);
					QDataStream in(&file);
					Message msg;
					while(!file.atEnd())
					{
						in >> msg.time >> msg.type >>msg.in >> msg.text;
						appendMessage(msg);
					}
				}
			}
		}
	}
}

QString qutim::quote(const QString &str)
{
	const static bool true_chars[128] =
	{// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
/* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 2 */ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
/* 3 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0,
/* 4 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 5 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0,
/* 6 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 7 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0
	};
	QString result;
	result.reserve(str.size() * 5); // the worst variant
	const QChar *s = str.data();
	while(!s->isNull())
	{
		if(s->unicode() < 0xff && true_chars[s->unicode()])
			result += *s;
		else
		{
			result += '%';
			if(s->unicode() < 0x1000)
				result += '0';
			if(s->unicode() < 0x100)
				result += '0';
			if(s->unicode() < 0x10)
				result += '0';
			result += QString::number(s->unicode(), 16);
		}
		s++;
	}
	return result;
}

QString qutim::unquote(const QString &str)
{
	QString result;
	bool ok = false;
	result.reserve(str.size()); // the worst variant
	const QChar *s = str.data();
	while(!s->isNull())
	{
		if(s->unicode() == L'%')
		{
			result += QChar(QString(++s, 4).toUShort(&ok, 16));
			s += 3;
		}
		else
			result += *s;
		s++;
	}
	return result;
}

void qutim::loadJson(const QFileInfoList &acc_files)
{
	foreach(const QFileInfo &info, acc_files)
	{
		QString protocol = info.fileName().section(".",0,0);
		QString account = unquote(info.fileName().section(".",1));
		setProtocol(protocol);
		setAccount(account);
		QDir dir(info.absoluteFilePath());
		QFileInfoList files = dir.entryInfoList(QDir::Readable | QDir::Files | QDir::NoDotAndDotDot,QDir::Name);
		for(int i=0;i<files.size();i++)
		{
			setValue(++m_value);
			QString contact = unquote(files[i].fileName().section(".", 0, -3));
			setContact(contact);
			QFile file(files[i].absoluteFilePath());
			if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
				continue;
			QTime timer;
			timer.start();
			int len = file.size();
			QByteArray array;
			const uchar *fmap = file.map(0, file.size());
			if(!fmap)
			{
				array = file.readAll();
				fmap = (uchar *)array.constData();
			}
			const uchar *s = K8JSON::skipBlanks(fmap, &len);
			QVariant val;
			uchar qch = *s;
			if(!s || (qch != '[' && qch != '{'))
				continue;
			qch = (qch == '{' ? '}' : ']');
			s++;
			len--;
			bool first = true;
			while(s)
			{
				s = K8JSON::skipBlanks(s, &len);
				if (len < 2 || (s && *s == qch))
					break;
				if(!s)
					break;
				if ((!first && *s != ',') || (first && *s == ',')) {
//					fprintf(stderr, "ERROR: invalid JSON file (delimiter)!\n");
					break;
				}
				first = false;
				if (*s == ',')
				{
					s++;
					len--;
				}
				val.clear();
				s = K8JSON::parseRec(val, s, &len);
				if (!s) {
//					fprintf(stderr, "ERROR: invalid JSON file!\n");
					break;
				}
				QVariantMap message = val.toMap();
				Message msg;
				msg.time = QDateTime::fromString(message.value("datetime").toString(), Qt::ISODate);
				msg.in = message.value("in", false).toBool();
				msg.type = message.value("type", 1).toInt();
				msg.text = message.value("text").toString();
				appendMessage(msg);
			}
		}
	}
}

void qutim::loadMessages(const QString &path)
{
	int num = 0;
	Types types;
	QVector<QFileInfoList> lists(3);
	if(guessXml(path, lists[0], num))
		types |= Xml;
	if(guessBin(path, lists[1], num))
		types |= Bin;
	if(guessJson(path, lists[2], num))
		types |= Json;
	setMaxValue(num);
	m_value = 0;
	if(types & Xml)
		loadXml(lists[0]);
	if(types & Bin)
		loadBin(lists[1]);
	if(types & Json)
		loadJson(lists[2]);
}

bool qutim::validate(const QString &path)
{
	int num = 0;
	Types types;
	QVector<QFileInfoList> lists(3);
	return guessXml(path, lists[0], num)
			|| guessBin(path, lists[1], num)
			|| guessJson(path, lists[2], num);
}

QString qutim::name()
{
	return "qutIM";
}

QIcon qutim::icon()
{
	return qutim_sdk_0_2::Icon("qutim", qutim_sdk_0_2::IconInfo::Client);
}

QString qutimExporter::name()
{
	return "qutIM (Json)";
}

void qutimExporter::writeMessages(const QHash<QString, Protocol> &data)
{
}

}
