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

#ifndef JPUBSUBINFO_H
#define JPUBSUBINFO_H

#include <QtGui>
#include "jPluginSystem.h"
#include "ui_jPubsubInfo.h"

class jPluginSystem;

class jPubsubInfo : public QWidget
{
	Q_OBJECT

public:
	jPubsubInfo(const QString &name, const QList<QVariant> &pubsub_info, QWidget *parent=0);
	virtual ~jPubsubInfo();

private:
	Ui::jPubsubInfoClass ui;
};

#endif
