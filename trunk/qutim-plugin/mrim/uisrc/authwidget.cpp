/*****************************************************************************
    authwidget

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

#include "authwidget.h"
#include "addcontactwidget.h"
#include "../coresrc/MRIMUtils.h"
#include "../coresrc/MRIMClient.h"

authwidget::authwidget(MRIMClient* aClient, QWidget *parent)
        : QWidget(parent), m_client(aClient)
{
        ui.setupUi(this);
        move(MRIMCommonUtils::DesktopCenter(size()));
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose, true);
}

authwidget::~authwidget()
{

}

void authwidget::SetupAuthRequest(QString aText, const QString& aContact)
{
	ui.authTextBox->setText(aText);
	m_contact = aContact;
}

void authwidget::on_rejectButton_clicked()
{
        close();
}

void authwidget::on_authButton_clicked()
{
        AcceptAuth();
}

void authwidget::AcceptAuth()
{
        m_client->Protocol()->SendAuthorizationTo(m_contact);

        hide();
        if (!m_client->Protocol()->IsInList(m_contact))
        {
                AddContactWidget* cntAddWidget = new AddContactWidget(m_client);
                cntAddWidget->FillGroups();
                cntAddWidget->SetEmail(m_contact,true);
                cntAddWidget->show();
        }
        close();
}
