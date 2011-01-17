/*
    Сopyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JCLIENTIDENTIFICATION_H_
#define JCLIENTIDENTIFICATION_H_

#include <QString>
#include <QStringList>
#include <QIcon>
#include <gloox/disco.h>
#include <gloox/client.h>
#include <gloox/iq.h>
#include "jBuddy.h"
#include "xeps/versionextension.h"

class jClientIdentification
{
public:
	jClientIdentification();
	virtual ~jClientIdentification();
	static jClientIdentification &instance();
	void loadSettings();
	void init(const QString profile_name);
	bool ifBase64(const QString &ver);
	void setClient(void *info, const JID& jid, Client *jClient, DiscoHandler *disco_handler);
	void newInfo(const VersionExtension *version, void *info);
	void newInfo(const Disco::Info &disco_info, void *info);
	QIcon clientIcon(const QString &name);
private:
	struct ClientInfo
	{
		QString m_node;		
		QString m_ver;
		QString m_name;
		QString m_version;
		QString m_os;
		QStringList m_features;
	};
//	QHash <QPair<QString, QString>, QList<jBuddy::ResourceInfo *> > m_buddies_features;
//	QHash <QPair<QString, QString>, QList<jBuddy::ResourceInfo *> > m_buddies_version;
	bool m_auto_request;
	QString m_hash_path;
	QString m_profile_name;
	QHash <QPair<QString, QString>, ClientInfo> m_clients_info;
	QHash <QString, QString> m_client_nodes;
};

#endif /*JCLIENTIDENTIFICATION_H_*/
