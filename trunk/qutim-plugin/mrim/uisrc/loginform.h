/*****************************************************************************
    LoginForm

    Copyright (c) 2009 by Rusanov Peter <tazkrut@mail.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QtGui/QWidget>
#include "ui_loginform.h"

class MRIMClient;

class LoginForm : public QWidget
{
    Q_OBJECT

public:
        LoginForm(MRIMClient *aClient, QWidget *parent = 0);
        LoginForm(QString aProfile, QWidget *parent = 0);
        ~LoginForm();
	QString GetEmail() const {return ui.emailEdit->text();}
	QString GetPass() const {return ui.passEdit->text();}
        void SaveSettings();
        void LoadSettings();
private:
    Ui::LoginFormClass ui;
    MRIMClient* m_client;
    QString m_profileName;
};

#endif
