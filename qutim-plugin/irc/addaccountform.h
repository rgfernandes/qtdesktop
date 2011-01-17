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

#ifndef ADDACCOUNTFORM_H
#define ADDACCOUNTFORM_H

#include <QWidget>
#include <QRegExp>
#include <QValidator>
#include "ui_addaccountform.h"

class AddAccountForm : public QWidget
{
    Q_OBJECT

public:
    AddAccountForm(QWidget *parent = 0);
    ~AddAccountForm();
	QString getServer() const {return ui.editServer->text();}
	QString getPort() const {return ui.editPort->text();}
	QString getNick() const {return ui.editNick->text();}
	QString getPasswd() const {return ui.editPasswd->text();}
	QString getRealName() const {return ui.editRealName->text();}

	bool getSavePasswd() const {return ui.checkboxSavePasswd->isChecked(); }

private:
    Ui::AddAccountFormClass ui;
};

#endif // ADDACCOUNTFORM_H
