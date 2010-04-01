/*****************************************************************************
    AddContactWidget

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

#include "addcontactwidget.h"
#include "../coresrc/MRIMUtils.h"
#include "../coresrc/MRIMClient.h"
#include <QRegExpValidator>
#include <QMessageBox>

AddContactWidget::AddContactWidget(MRIMClient* aClient, QWidget *parent)
        : QDialog(parent), m_client(aClient)
{
	ui.setupUi(this);
        move(MRIMCommonUtils::DesktopCenter(size()));
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose, true);
}

AddContactWidget::~AddContactWidget()
{

}


void AddContactWidget::on_pushButton_clicked()
{

}

void AddContactWidget::FillGroups()
{
    QList<MRIMGroup*> grList = m_client->Protocol()->GetAllGroups();
    for (qint32 i=0; i < grList.count(); i++)
    {
            quint32 grId = grList.at(i)->Id().toUInt();
            QVariant val(grId);
            ui.groupComboBox->addItem(grList.at(i)->Name(),val);
    }
}

void AddContactWidget::on_addContactButton_clicked()
{
	QRegExp rx("^[\\w\\d][\\w\\d\\-.]*@[\\w\\d]{2}[\\w\\d\\-]*.[\\w\\d]{2}(\\.?[\\w\\d\\-]+)*$");
	int pos;
	QRegExpValidator emailValidator(rx,0);
	QString email(ui.emailEdit->text());
	if (emailValidator.validate(email,pos) == QValidator::Acceptable)
	{
		m_selctedGrId = ui.groupComboBox->itemData(ui.groupComboBox->currentIndex()).toUInt();		
		m_contactEmail = ui.emailEdit->text();
                m_client->Protocol()->AddContact(GetContactEmail(),GetNickname(),GetSelectedGroupId());
                close();
	}
	else
	{
		QMessageBox::critical(this,tr("Incorrect email"),tr("Email you entered is not valid or empty!"),QMessageBox::Ok,QMessageBox::Ok);
	}
}

quint32 AddContactWidget::GetSelectedGroupId()
{
        return m_selctedGrId;
}

QString AddContactWidget::GetContactEmail()
{
	return m_contactEmail;
}

void AddContactWidget::SetEmail(QString aEmail, bool aReadOnly)
{
	QRegExp rx("^[\\w\\d][\\w\\d\\-.]*@[\\w\\d]{2}[\\w\\d\\-]*.[\\w\\d]{2}(\\.?[\\w\\d\\-]+)*$");
	int pos;
	QRegExpValidator emailValidator(rx,0);
	if (emailValidator.validate(aEmail,pos) == QValidator::Acceptable)
	{
		ui.emailEdit->clear();
		ui.emailEdit->insert(aEmail);
		ui.emailEdit->setReadOnly(aReadOnly);
	}
	else
	{
		ui.emailEdit->clear();
		ui.emailEdit->setReadOnly(false);
	}
}

void AddContactWidget::SetNick(QString aNick, bool aReadOnly)
{
	ui.nickEdit->clear();
	ui.nickEdit->insert(aNick);
	ui.nickEdit->setReadOnly(aReadOnly);
}
