/*
Utils

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

#ifndef UTILS_H
#define UTILS_H

#include "../include/qutim/jabber.h"
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>
#include <QMultiMap>
#include <gloox/gloox.h>
#include <gloox/stanzaextension.h>

using namespace gloox;

typedef QMap<QString, QString> QStringMap;
typedef QMultiMap<QString, QString> QStringMultiMap;

enum SEType
{
	SExtVersion = ExtUser,
	SExtGmail,
	SExtActivity,
	SExtTune,
	SExtXPresence,
	SExtXStatus,
	SExtFileTransfer,
	SExtMood
};

namespace utils
{
	QString fromStd(const std::string &str);
	std::string toStd(const QString &str);
	QStringList fromStd(const StringList &str);
	StringList toStd(const QStringList &str);
	QStringMap fromStd(const StringMap &map);
	StringMap toStd(const QStringMap &map);
	QStringMultiMap fromStd(const StringMultiMap &map);
	StringMultiMap toStd(const QStringMultiMap &map);
	QDateTime fromStamp(const std::string &stamp);
	std::string toStamp(const QDateTime &datetime);
	QString timeToString(const int &sec);
	QString getPubsubName(const QString &pubsub);
}

#endif // UTILS_H
