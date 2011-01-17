/*
		ircConsole

		Copyright (c) 2008-2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef IRCCONSOLE_H
#define IRCCONSOLE_H
//
#include <QWidget>
#include <QListView>
#include <QDateTime>
#include "ui_ircconsole.h"
//
class ircConsole : public QWidget
{
Q_OBJECT
public:
	ircConsole( QWidget * parent = 0, Qt::WFlags f = 0 );

private slots:
	void addMessage(QString, QString);
	void onPressEnter();

private:
	Ui::IrcConsoleClass ui;

signals:
	void cmdEntered(QString);

};
#endif





