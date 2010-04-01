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

#include "ircconsole.h"
#include <qutim/plugininterface.h>
//
ircConsole::ircConsole( QWidget * parent, Qt::WFlags f) 
	: QWidget(parent, f)
{
	ui.setupUi(this);

	connect(ui.editIn, SIGNAL(returnPressed()), this, SLOT(onPressEnter()));

	qutim_sdk_0_2::SystemsCity::PluginSystem()->centerizeWidget(this);
}

void ircConsole::addMessage(QString msg, QString type = QString())
{
	QDateTime dt(QDateTime::currentDateTime());
	QString tbstr = msg;
	if (type == "Notice") tbstr = "<font color='magenta'>[Notice] " + msg + "</font>";
	else if (type == "MOTDSTART") tbstr = "<font color='green'>["+type+"] <b>" + msg + "</b></font>";
	else if (type == "MOTD") tbstr = "<font color='green'>["+type+"] <b>" + msg + "</b></font>";
	else if (type == "ENDOFMOTD") tbstr = "<font color='green'>["+type+"] <b>" + msg + "</b></font>";
	else if (type == "Reply") tbstr = "<font color='Blue'>[Reply] " + msg + "</font>";
	else if (type == "error") tbstr = "<font color='red'>[ERROR] " + msg + "</font>";
	else if (type.length()>0) tbstr = "["+type+"] " + msg;
	else tbstr = msg;
	ui.textBrowser->append("["+dt.toString("hh:mm:ss")+"] "+tbstr);

}

void ircConsole::onPressEnter()
{
	emit cmdEntered(ui.editIn->text());
	ui.editIn->clear();
}

