/*
 * kopete.cpp
 * 
 * Copyright (C) 2008  Nigmatullin Ruslan
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
*/

#include "kopete.h"
#include <QtXml>
#include <QTextDocument>
#include <qutim/iconmanagerinterface.h>

using namespace qutim_sdk_0_2;

namespace HistoryManager {

kopete::kopete()
{
}

kopete::~kopete()
{
}

bool kopete::validate(const QString &kopete_id, const QString &id)
{
	if(kopete_id.size() == id.size())
	{
//		FIXME: It's too slow
		static const QRegExp regexp("[./~?*]");
		QString tmp_id = id;
		if(tmp_id.replace(regexp, "-") == kopete_id)
			return true;
	}
	return false;
}

QString kopete::guessFromList(const QString &kopete_id, const QStringList &list)
{
	for(int i = 0; i < list.size(); i++)
	{
		if(validate(kopete_id, list[i]))
			return list[i];
	}
	return QString();
}

inline void kopete_id_helper(QHash<QString, QString> &hash, const QString &id)
{
	static const QRegExp regexp("[./~?*]");
	if(id.contains(regexp))
	{
		QString tmp_id = id;
		hash.insert(tmp_id.replace(regexp, "-"), id);
	}
}

QString kopete::guessID(const QString &kopete_id)
{
	static PluginSystemInterface *ps = SystemsCity::PluginSystem();
	static QHash<QString, QString> hash;
	if(hash.isEmpty())
	{
		QList<TreeModelItem> accounts = ps->getItemChildren();
		foreach(const TreeModelItem &account, accounts)
		{
			kopete_id_helper(hash, account.m_protocol_name);
			kopete_id_helper(hash, account.m_account_name);
			QList<TreeModelItem> groups = ps->getItemChildren(account);
			foreach(const TreeModelItem &group, groups)
			{
				QList<TreeModelItem> contacts = ps->getItemChildren(group);
				foreach(const TreeModelItem &contact, contacts)
					kopete_id_helper(hash, contact.m_item_name);
			}
		}
	}
	return hash.value(kopete_id, kopete_id);
}

void kopete::loadMessages(const QString &path)
{
	QDir dir = path;
	if(!dir.cd("logs"))
		return;
	int num=0;
	QStringList protocol_dirs = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
	foreach(QString protocol, protocol_dirs)
	{
		QDir protocol_dir(dir.filePath(protocol));
		QStringList account_dirs = protocol_dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
		foreach(QString account, account_dirs)
		{
			QDir dir(protocol_dir.filePath(account));
			QStringList files = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
			num+=files.size();
		}
	}
	setMaxValue(num);
	num = 0;

	foreach(QString protocol_name, protocol_dirs)
	{
		QDir protocol_dir(dir.filePath(protocol_name));
		QStringList account_dirs = protocol_dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
		QString protocol = protocol_name.remove("Protocol");
		setProtocol(guessID(protocol));
		foreach(QString account, account_dirs)
		{
			QFileInfoList files = QDir(protocol_dir.filePath(account)).entryInfoList(QDir::Files|QDir::NoDotAndDotDot);
			setAccount(guessID(account));
			for(int i=0;i<files.size();i++)
			{
				setValue(++num);
				if(!files[i].fileName().startsWith("log.20"))
				{
					QFile file(files[i].absoluteFilePath());
					if (file.open(QIODevice::ReadOnly) )
					{
						QDomDocument doc;
						if(doc.setContent(&file))
						{
							QDomElement rootElement = doc.documentElement();
							QDomElement headElement = rootElement.firstChildElement("head");
							QDomElement dateElement = headElement.firstChildElement("date");
							int msgCount = rootElement.elementsByTagName("msg").count();
							QString otherDate = dateElement.attribute("month")+"/"+dateElement.attribute("year");
							QDomElement msg = rootElement.firstChildElement("msg");
							QString uin = files[i].fileName();
							uin.truncate(uin.indexOf('.'));
							setContact(guessID(uin));
							for ( int j = 0; j < msgCount ; j++ )
							{
								QString datetime = msg.attribute("time");
								datetime += " ";
								datetime += otherDate;
								Message message;
								message.type=1;
								message.time = QDateTime().fromString(datetime,"d h:m:s M/yyyy");
								message.text = Qt::escape(msg.text()).replace("\n", "<br/>");
								message.in = msg.attribute("in") == "1";
								appendMessage(message);
								msg = msg.nextSiblingElement("msg");
							}
						}
					}
				}
			}
		}
	}
}

// ~/.kde/share/apps/kopete/
bool kopete::validate(const QString &path)
{
	QDir dir = path;
	if(!dir.cd("logs"))
		return false;
	QStringList protocol_dirs = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
	foreach(const QString &protocol, protocol_dirs)
	{
		QDir protocol_dir = dir.filePath(protocol);
		QStringList account_dirs = protocol_dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
		static QStringList filter = QStringList() << "*.xml";
		foreach(const QString &account, account_dirs)
		{
			if(!QDir(protocol_dir.filePath(account)).entryList(filter, QDir::Files).isEmpty())
				return true;
		}
	}
	return false;
}

QString kopete::name()
{
	return "Kopete";
}

QIcon kopete::icon()
{
	return Icon("kopete", IconInfo::Client);
}

}
