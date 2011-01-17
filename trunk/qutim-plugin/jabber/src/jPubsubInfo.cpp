/*****************************************************************************
    jPubsubInfo

    Copyright (c) 2009 by Belov Nikita <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "jPubsubInfo.h"
#include "utils.h"

jPubsubInfo::jPubsubInfo(const QString &name, const QList<QVariant> &pubsub_info, QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);

	QString text="";
	int count = pubsub_info.at(0).toInt();

	if (name == "mood")
	{
		text += tr("<h3>Mood info:</h3>");
		text += "<br/><img src=\""+jPluginSystem::instance().getIconFileName(pubsub_info.at(count+2).toString())+"\"/>";
		QString name = pubsub_info.at(1).toString();
		if (!name.isEmpty())
		{
			text += "<br/>"+tr("Name: %1").arg(name);
			QString mtext = pubsub_info.at(2).toString();
			if (!mtext.isEmpty())
				text += "<br/>"+tr("Text: %1").arg(mtext);
		}
	}
	else if (name == "activity")
	{
		text += tr("<h3>Activity info:</h3>");
		text += "<br/><img src=\""+jPluginSystem::instance().getIconFileName(pubsub_info.at(count+2).toString())+"\"/>";
		QString general = pubsub_info.at(1).toString();
		if (!general.isEmpty())
		{
			text += "<br/>"+tr("General: %1").arg(general);
			QString specific = pubsub_info.at(2).toString();
			if (!specific.isEmpty())
				text += "<br/>"+tr("Specific: %1").arg(specific);
			QString atext = pubsub_info.at(3).toString();
			if (!atext.isEmpty())
				text += "<br/>"+tr("Text: %1").arg(atext);
		}
	}
	else if (name == "tune")
	{
		text += tr("<h3>Tune info:</h3>");
		text += "<br/><img src=\""+jPluginSystem::instance().getIconFileName(pubsub_info.at(count+2).toString())+"\"/>";
		QString artist = pubsub_info.at(1).toString();
		QString title = pubsub_info.at(2).toString();
		QString source = pubsub_info.at(3).toString();
		QString track = pubsub_info.at(4).toString();
		QString uri = pubsub_info.at(7).toString();
		int length = pubsub_info.at(5).toInt();
		int rating = pubsub_info.at(6).toInt();
		if (!artist.isEmpty())
			text += "<br/>"+tr("Artist: %1").arg(artist);
		if (!title.isEmpty())
			text += "<br/>"+tr("Title: %1").arg(title);
		if (!source.isEmpty())
			text += "<br/>"+tr("Source: %1").arg(source);
		if (!track.isEmpty())
			text += "<br/>"+tr("Track: %1").arg(track);
		if (!uri.isEmpty())
			text += "<br/>"+tr("Uri: <a href=\"%1\">link</a>").arg(uri);
		if (length != -1)
			text += "<br/>"+tr("Length: %1").arg(utils::timeToString(length));
		if (rating != -1)
			text += "<br/>"+tr("Rating: %1").arg(QString::number(rating));
	}

	ui.info->setText(text);
}

jPubsubInfo::~jPubsubInfo()
{
}
