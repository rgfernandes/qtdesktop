/*
    noteWidget

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "notewidget.h"

noteWidget::noteWidget(const QString &mu, const QString &cu, const QString &name, const QString &profile_name, QWidget *parent)
    : QWidget(parent)
    , contactUin(cu)
    , mineUin(mu)
    , m_profile_name(profile_name)
{
	ui.setupUi(this);
	setFixedSize(size());
	move(desktopCenter());
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowTitle(name);
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+mineUin, "contactlist");
	ui.noteEdit->setPlainText(settings.value(contactUin + "/note", "").toString());
}

noteWidget::~noteWidget()
{

}

QPoint noteWidget::desktopCenter()
{
	QDesktopWidget &desktop = *QApplication::desktop();
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void noteWidget::on_okButton_clicked()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+mineUin, "contactlist");
	settings.setValue(contactUin + "/note", ui.noteEdit->toPlainText());
	close();
}
