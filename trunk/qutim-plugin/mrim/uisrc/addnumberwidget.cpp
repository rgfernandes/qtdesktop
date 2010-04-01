/*****************************************************************************
    AddNumberWidget

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

#include "addnumberwidget.h"
#include "ui_addnumberwidget.h"
#include "../coresrc/MRIMUtils.h"
#include "../coresrc/mrimcontact.h"
#include "../coresrc/MRIMClient.h"
#include <QRegExpValidator>

AddNumberWidget::AddNumberWidget(MRIMClient* aClient, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::AddNumberWidget), m_client(aClient)
{
    m_ui->setupUi(this);
    QRegExp rx("[\\+0-9]+");
    QRegExpValidator* valid = new QRegExpValidator(rx,this);
    m_ui->homeEdit->setValidator(valid);
    m_ui->workEdit->setValidator(valid);
    m_ui->mobileEdit->setValidator(valid);
}

AddNumberWidget::~AddNumberWidget()
{
    delete m_ui;
}

void AddNumberWidget::show(MRIMContact* aCnt)
{
    m_cnt = aCnt;
    QStringList numbers = m_cnt->Phone();
    if (numbers.count() > 0)
    {
        m_ui->homeEdit->setText(numbers[0]);
    }

    if (numbers.count() > 1)
    {
        m_ui->workEdit->setText(numbers[1]);
    }

    if (numbers.count() > 2)
    {
        m_ui->mobileEdit->setText(numbers[2]);
    }

    move(MRIMCommonUtils::DesktopCenter(size()));
    QWidget::show();
}


void AddNumberWidget::on_saveButton_clicked()
{
        QStringList numbers;
    if (m_ui->homeEdit->text().length() > 0)
    {
        numbers.append(m_ui->homeEdit->text());
    }

    if (m_ui->workEdit->text().length() > 0)
    {
        numbers.append(m_ui->workEdit->text());
    }

    if (m_ui->mobileEdit->text().length() > 0)
    {
        numbers.append(m_ui->mobileEdit->text());
    }

    m_cnt->SetPhone(numbers);
    m_client->Protocol()->SendModifyContact(m_cnt->Email(),"",0,0,MRIMProto::EKeepOldValues);
    emit numbersChanged();
    hide();
}
