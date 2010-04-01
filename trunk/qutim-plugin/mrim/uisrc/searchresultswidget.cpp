/*****************************************************************************
    SearchResultsWidget

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

#include "searchresultswidget.h"
#include "contactdetails.h"
#include "../coresrc/MRIMUtils.h"
#include "../coresrc/MRIMClient.h"
#include "../coresrc/avatarfetcher.h"
#include <QDate>

ContactWidgetItem::ContactWidgetItem(QString aEmail, bool aShowAvatar, QTreeWidget* aParent) : QTreeWidgetItem(aParent), m_email(aEmail)
{
    if (!aShowAvatar) return;

    bool hasAvatar = QFile::exists(AvatarFetcher::SmallAvatarPath(m_email));
    if (!hasAvatar)
    {
        connect(AvatarFetcher::Instance(),SIGNAL(SmallAvatarFetched(QString)),this,SLOT(HandleSmallAvatarFetched(QString)));
        AvatarFetcher::Instance()->FetchSmallAvatar(m_email);
    }
    else
    {
        SetAvatar();
    }
}

void ContactWidgetItem::SetAvatar()
{
    qint32 size = 32;
    QSize ava_size(size,size);
    QIcon icon(AvatarFetcher::SmallAvatarPath(m_email));
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(65535,65535)),QIcon::Normal,QIcon::On);
    if(!pixmap.isNull())
    {
        QPixmap alpha (ava_size);
        alpha.fill(QColor(0,0,0));
        QPainter painter(&alpha);
        QPen pen(QColor(127,127,127));
        painter.setRenderHint(QPainter::Antialiasing);
        pen.setWidth(0);
        painter.setPen(pen);
        painter.setBrush(QBrush(QColor(255,255,255)));
        painter.drawRoundedRect(QRectF(QPointF(0,0),QSize(size-1,size-1)),5,5);
        painter.end();
        pixmap = pixmap.scaled(ava_size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        pixmap.setAlphaChannel(alpha);
        setIcon(1,pixmap);
    }
}

void ContactWidgetItem::HandleSmallAvatarFetched(QString aEmail)
{
    if (m_email != aEmail) return;
    SetAvatar();
    disconnect(AvatarFetcher::Instance(),SIGNAL(SmallAvatarFetched(QString)),this,SLOT(HandleSmallAvatarFetched(QString)));
}

ContactWidgetItem::~ContactWidgetItem()
{
    MRIMSearchParams* info = (MRIMSearchParams*)qVariantValue<qptrdiff>(data(0,Qt::UserRole));
    delete info;
}

SearchResultsWidget::SearchResultsWidget(MRIMClient* aClient, QWidget *parent)
        : QWidget(parent), m_client(aClient)
{
    ui.setupUi(this);
    move(MRIMCommonUtils::DesktopCenter(size()));
    ui.contactsTreeWidget->headerItem()->setText(0," ");
    ui.contactsTreeWidget->headerItem()->setText(1," ");
    ui.contactsTreeWidget->setColumnWidth(0,20);
    ui.contactsTreeWidget->setColumnWidth(1,32);
    ui.contactsTreeWidget->setColumnWidth(2,130);
    ui.contactsTreeWidget->setColumnWidth(3,130);
    ui.contactsTreeWidget->setColumnWidth(4,130);
    ui.contactsTreeWidget->setColumnWidth(5,130);
    ui.contactsTreeWidget->setColumnWidth(6,30);
    ui.contactsTreeWidget->setColumnWidth(7,30);
    ui.contactsTreeWidget->setColumnWidth(8,50);
}

SearchResultsWidget::~SearchResultsWidget()
{

}

void SearchResultsWidget::AddContacts(QList<MRIMSearchParams*> aFoundContacts, bool aShowAvatars)
{
    //add to ui
    if (aShowAvatars)
        ui.contactsTreeWidget->showColumn(1);
    else
        ui.contactsTreeWidget->hideColumn(1);
    Status stat;
    foreach (MRIMSearchParams* foundContact,aFoundContacts)
    {
        QString email = foundContact->EmailAddr+"@"+foundContact->EmailDomain;
        ContactWidgetItem* contact = new ContactWidgetItem(email,aShowAvatars,ui.contactsTreeWidget);        

        if (foundContact->Status != -1)
	{
            contact->setIcon(0,Status::GetIcon(foundContact->Status));
	}
	else
	{
            contact->setIcon(0,Status::GetIcon(STATUS_UNDETERMINATED));
	}

        if (foundContact->EmailAddr.length() > 0)
	{
            contact->setText(3,email);
	}

        if (foundContact->Nick.length() > 0)
	{
            contact->setText(2,foundContact->Nick);
	}

        if (foundContact->Name.length() > 0)
	{
            contact->setText(4,foundContact->Name);
	}

        if (foundContact->Surname.length() > 0)
	{
            contact->setText(5,foundContact->Surname);
	}

        if (foundContact->Sex == -1)
	{
            contact->setText(6,"-");
	}
		
        if (foundContact->Sex == 1)
	{
            contact->setText(6,tr("M"));
	}

        if (foundContact->Sex == 2)
        {
            contact->setText(6,tr("F"));
	}

        if (foundContact->BirthDay != -1 && foundContact->BirthdayMonth != -1 && foundContact->BirthYear != -1)
	{
            QDate currDate = QDate::currentDate();
            QDate contactBDay = QDate(foundContact->BirthYear,foundContact->BirthdayMonth,foundContact->BirthDay);
            int age = contactBDay.daysTo(currDate) / 365;
            contact->setText(7,QString::number(age));
	}        
        contact->setIcon(8,MRIMPluginSystem::PluginSystem()->getIcon("contactinfo"));
        contact->setData(0,Qt::UserRole,(qptrdiff)foundContact);
    }
    aFoundContacts.clear();
}

void SearchResultsWidget::on_addCntButton_clicked()
{
    QTreeWidgetItem* currItem = ui.contactsTreeWidget->currentItem();
    if (currItem)
    {
        m_client->HandleAddContact(currItem->text(3),currItem->text(2));
    }
}

void SearchResultsWidget::Reset()
{
    ui.contactsTreeWidget->clear();
}

void SearchResultsWidget::show(QList<MRIMSearchParams*> aFoundList, bool aShowAvatars)
{
    AddContacts(aFoundList, aShowAvatars);
    QWidget::show();
}

void SearchResultsWidget::on_contactsTreeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
    if (column != 8) return;
    ContactDetails* cntDetails = new ContactDetails(m_client);
    MRIMSearchParams* info = (MRIMSearchParams*)qVariantValue<qptrdiff>(item->data(0,Qt::UserRole));
    if (!info) return;
    cntDetails->show(*info);
}
