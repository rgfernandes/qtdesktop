/*
    addRenameDialog

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



#include "addrenamedialog.h"
#include <qutim/iconmanagerinterface.h>

addRenameDialog::addRenameDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(size());
	move(desktopCenter());
	ui.pushButton->setIcon(qutim_sdk_0_2::Icon("apply"));
}

addRenameDialog::~addRenameDialog()
{

}

QPoint addRenameDialog::desktopCenter()
{
	QDesktopWidget &desktop = *QApplication::desktop();
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void addRenameDialog::on_lineEdit_textChanged(const QString &t)
{
	name = t;	
	ui.pushButton->setDisabled(t.isEmpty());
}
