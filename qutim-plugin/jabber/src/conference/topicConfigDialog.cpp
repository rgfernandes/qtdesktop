/*
    topicConfigDialog.cpp

    Copyright (c) 2008 by Nikita '∆z' Belov <null@deltaz.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "topicConfigDialog.h"

topicConfigDialog::topicConfigDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
//	setWindowIcon(IcqPluginSystem::instance().getIcon("add_user.png"));
	setAttribute(Qt::WA_QuitOnClose, false);
}

topicConfigDialog::~topicConfigDialog()
{
}
