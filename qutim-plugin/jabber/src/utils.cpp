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

#include "utils.h"

QString utils::fromStd(const std::string &str)
{
	return QString::fromUtf8(str.c_str());
}

std::string utils::toStd(const QString &str)
{
	return str.toUtf8().constData();
}

QStringList utils::fromStd(const StringList &list)
{
	QStringList list2;
	foreach(const std::string &str, list)
		list2.append(fromStd(str));
	return list2;
}

StringList utils::toStd(const QStringList &list)
{
	StringList list2;
	foreach(const QString &str, list)
		list2.push_back(toStd(str));
	return list2;
}

QStringMap utils::fromStd(const StringMap &map)
{
	QStringMap map2;
	StringMap::const_iterator i = map.begin();
	for(;i != map.end();i++)
		map2[fromStd((*i).first)] = fromStd((*i).second);
	return map2;
}

StringMap utils::toStd(const QStringMap &map)
{
	StringMap map2;
	QStringMap::const_iterator i = map.constBegin();
	for( ; i != map.constEnd(); ++i)
		map2[toStd(i.key())] = toStd(i.value());
	return map2;
}

QStringMultiMap utils::fromStd(const StringMultiMap &map)
{
	QStringMultiMap map2;
	StringMultiMap::const_iterator i = map.begin();
	for( ; i != map.end(); ++i )
		map2.insertMulti(fromStd((*i).first), fromStd((*i).second));
	return map2;
}

StringMultiMap utils::toStd(const QStringMultiMap &map)
{
	StringMultiMap map2;
	QStringMultiMap::const_iterator i = map.constBegin();
	for( ; i != map.constEnd(); i++)
		map2.insert(std::pair<std::string, std::string>(toStd(i.key()), toStd(i.value())));
	return map2;
}
namespace utils
{
	static const QString full_stamp_str( "yyyy-MM-ddThh:mm:ss.zzz" );
	static const QString fullz_stamp_str( "yyyy-MM-ddThh:mm:ss.zzzZ" );
	static const QString date_time_stamp_str( "yyyy-MM-ddThh:mm:ss" );
	static const QString deprecated_stamp_str( "yyyyMMddThh:mm:ss" );
	static const QString time_stamp_str( "hh:mm" );
}

QDateTime utils::fromStamp( const std::string &std_stamp )
{
	QDateTime date_time;
	QString stamp = fromStd( std_stamp );
	if( stamp.contains( QLatin1Char('-') ) )
	{
		int num = stamp.indexOf( QLatin1Char('Z') );
		if(num<0)
			num = stamp.lastIndexOf( QLatin1Char('-') );
		QString time = stamp;
		time.truncate( num );
		if(num==19)
			date_time = QDateTime::fromString( time, date_time_stamp_str );
		else
			date_time = QDateTime::fromString( time, full_stamp_str );
		if(num>19)
		{
			QTime delta = QTime::fromString( stamp.right(5), time_stamp_str );
			int multi = 1;
			if( stamp.at( stamp.length() - 6 ) == '+' )
				multi = -1;
			date_time = date_time.addSecs( multi * ( delta.minute() * 60 + delta.hour() * 3600 ) );
		}
	}
	else // Deprecated format of stamp
		date_time = QDateTime::fromString( stamp, deprecated_stamp_str );
	if( !date_time.isValid() )
		return QDateTime();
	date_time.setTimeSpec( Qt::UTC );
	return date_time.toLocalTime();
}

std::string utils::toStamp( const QDateTime &date_time )
{
	return toStd( date_time.toUTC().toString( fullz_stamp_str ) );
}

QString utils::timeToString(const int &sec)
{
	QString rstr="", str;

	int years = 0;
	int days = 0;
	int hours = 0;
	int minutes = 0;
	int seconds = sec;

	if (seconds / 60 >= 1)
	{
		minutes = seconds / 60;
		seconds %= 60;
	}
	if (minutes / 60 >= 1)
	{
		hours = minutes / 60;
		minutes %= 60;
	}
	if (hours / 24 >= 1)
	{
		days = hours / 24;
		hours %= 24;
	}
	if (days / 365 >= 1)
	{
		years = days / 365;
		days %= 365;
	}

	if (seconds >= 2)
		str = QObject::tr("%1 seconds").arg(QString::number(seconds));
	else
		str = QObject::tr("%1 second").arg(QString::number(seconds));
	rstr += str;
	if (minutes >= 2)
		str = QObject::tr("%1 minutes").arg(QString::number(minutes));
	else if (minutes == 1)
		str = QObject::tr("1 minute");
	else
		str = "";
	rstr = str+" "+rstr;
	if (hours >= 2)
		str = QObject::tr("%1 hours").arg(QString::number(hours));
	else if (hours == 1)
		str = QObject::tr("1 hour");
	else
		str = "";
	rstr = str+" "+rstr;
	if (days >= 2)
		str = QObject::tr("%1 days").arg(QString::number(days));
	else if (days == 1)
		str = QObject::tr("1 day");
	else
		str = "";
	rstr = str+" "+rstr;
	if (years >= 2)
		str = QObject::tr("%1 years").arg(QString::number(years));
	else if (years == 1)
		str = QObject::tr("1 year");
	else
		str = "";
	rstr = str+" "+rstr;

	return rstr;
}

QString utils::getPubsubName(const QString &pubsub)
{
	if (pubsub == "mood")
		return QObject::tr("Mood");
	else if (pubsub == "activity")
		return QObject::tr("Activity");
	else if (pubsub == "tune")
		return QObject::tr("Tune");
	else
		return QObject::tr("Unknown");
}
