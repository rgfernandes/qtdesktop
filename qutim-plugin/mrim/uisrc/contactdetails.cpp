/*****************************************************************************
    ContactDetails

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

#include "contactdetails.h"
#include "../coresrc/MRIMUtils.h"
#include "../coresrc/avatarfetcher.h"
#include "addcontactwidget.h"
#include <QDate>

ContactDetails::ContactDetails(MRIMClient* aClient, QWidget *parent)
        : QWidget(parent), m_client(aClient)
{
	ui.setupUi(this);
        connect(AvatarFetcher::Instance(),SIGNAL(BigAvatarFetched(QString)),this,SLOT(SetAvatarLabelText(QString)));
}

ContactDetails::~ContactDetails()
{

}

void ContactDetails::SetInfo(MRIMSearchParams aInfo)
{
    ui.nameLabel->setText(aInfo.Name);
    ui.surnameLabel->setText(aInfo.Surname);
    ui.nickLabel->setText(aInfo.Nick);
    m_email = aInfo.EmailAddr+"@"+aInfo.EmailDomain;
    ui.emailLabel->setText("<a href=\"mailto:"+m_email+"\">"+m_email+"</a>");
    QDate currDate = QDate::currentDate();
    QDate contactBDay = QDate(aInfo.BirthYear,aInfo.BirthdayMonth,aInfo.BirthDay);
    int age = contactBDay.daysTo(currDate) / 365;
    ui.ageLabel->setText(QString::number(age));
    ui.birthdayLabel->setText(contactBDay.toString());
    ui.placeLabel->setText(aInfo.LocationText);
    QString sex = "-";

    if (aInfo.Sex == 1)
    {
            sex=tr("M");
    }

    if (aInfo.Sex == 2)
    {
            sex=tr("F");
    }
    ui.sexLabel->setText(sex);

    bool hasAvatar = QFile::exists(AvatarFetcher::BigAvatarPath(m_email));
    if (!hasAvatar)
    {
        AvatarFetcher::Instance()->FetchBigAvatar(m_email);
    }
    else
    {
        SetAvatarLabelText(m_email);
    }
}

void ContactDetails::ResetInfo()
{	
	ui.nameLabel->clear();
	ui.surnameLabel->clear();
	ui.nickLabel->clear();
	ui.emailLabel->clear();
	ui.ageLabel->clear();
	ui.birthdayLabel->clear();
	ui.placeLabel->clear();
	ui.zodiacLabel->clear();
}

void ContactDetails::show(const MRIMSearchParams& aInfo)
{
	ResetInfo();
        SetInfo(aInfo);
        move(MRIMCommonUtils::DesktopCenter(size()));
        ui.addToCLButton->setVisible(!m_client->Protocol()->IsInList(m_email));
        QWidget::show();
}

void ContactDetails::SetAvatarLabelText(QString aEmail)
{
    if (m_email != aEmail) return;
    bool hasAvatar = QFile::exists(AvatarFetcher::BigAvatarPath(aEmail));
    QString text = (hasAvatar) ? "<img src='"+AvatarFetcher::BigAvatarPath(aEmail)+"' align='center'/>" : tr("No avatar");
    ui.avatarLabel->setText(text);
}

void ContactDetails::on_addToCLButton_clicked()
{
    AddContactWidget *addCntWidget = new AddContactWidget(m_client,this);
    connect(addCntWidget,SIGNAL(accepted()),ui.addToCLButton,SLOT(hide()));
    addCntWidget->FillGroups();
    addCntWidget->SetEmail(m_email,true);
    addCntWidget->SetNick(ui.nickLabel->text());
    addCntWidget->show();
}

