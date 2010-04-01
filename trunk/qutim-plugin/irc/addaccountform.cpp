/*
   AddAccountForm

    Copyright (c) 2008 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "addaccountform.h"

AddAccountForm::AddAccountForm(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	
	ui.labelPasswd->hide();
	ui.editPasswd->hide();
	ui.checkboxSavePasswd->hide();
}

AddAccountForm::~AddAccountForm()
{

}
