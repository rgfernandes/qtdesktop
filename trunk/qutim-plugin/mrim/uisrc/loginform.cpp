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

#include "loginform.h"
#include "../coresrc/MRIMClient.h"

LoginForm::LoginForm(MRIMClient *aClient, QWidget *parent) : QWidget(parent), m_client(aClient)
{
    ui.setupUi(this);
    m_profileName = aClient->ProfileName();
}

LoginForm::LoginForm(QString aProfile, QWidget *parent)  : QWidget(parent), m_client(0), m_profileName(aProfile)
{
    ui.setupUi(this);
}

LoginForm::~LoginForm()
{
}

void LoginForm::SaveSettings()
{
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName, "mrimsettings");
    QStringList accounts = settings.value("accounts/list").toStringList();
    QString email = GetEmail(), pass = GetPass();

    if( !accounts.contains(email) )
    {
        accounts<<email;
        accounts.sort();
        settings.setValue("accounts/list", accounts);
    }    
    QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+email, "accountsettings");
    account_settings.setValue("main/login", email);
    account_settings.setValue("main/password", pass);
}

void LoginForm::LoadSettings()
{
    if (!m_client) return;

    QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_client->AccountName(), "accountsettings");
    ui.emailEdit->setText(account_settings.value("main/login").toString());
    ui.emailEdit->setReadOnly(true);
    ui.passEdit->setText(account_settings.value("main/password").toString());
}

