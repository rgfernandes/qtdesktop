/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JSERVICEDISCOVERY_H
#define JSERVICEDISCOVERY_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <gloox/discohandler.h>
#include <gloox/disco.h>
#include "jDiscoItem.h"

using namespace gloox;

class jServiceDiscovery : public QObject, DiscoHandler
{
	Q_OBJECT

	public:
		jServiceDiscovery(const QString &type);
		~jServiceDiscovery();
		void search(jDiscoItem *disco_item);
		void handleDiscoInfo(const JID &from, const Disco::Info &info, int context);
		void handleDiscoItems(const JID &from, const Disco::Items &items, int context);
		void handleDiscoError(const JID &from, const Error *error, int context);
		bool handleDiscoSet(const IQ &iq);

	signals:
		void getDiscoInfo(const QString &server, const QString &node, DiscoHandler *dh);
		void getDiscoItem(const QString &server, const QString &node, DiscoHandler *dh);
		//void finishSearch(jDiscoItem *disco_item);
		void finishSearch(const QList<jDiscoItem*> &disco_item, const QString&key);
		void finishSelfSearch(jDiscoItem *disco_item);

	private:
		void modifyItem(jDiscoItem *disco_item, Disco::Identity *identity);
		void setActions(jDiscoItem *disco_item);
		QString m_service_type;
		jDiscoItem *m_disco_item;
		QString m_category;
		QString m_type;
		QHash<QString, QString> m_name_library;
		int m_count;

};

#endif //JSERVICEDISCOVERY_H
