/*****************************************************************************
    SMSWidget

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

#include "smswidget.h"
#include "ui_smswidget.h"
#include "../coresrc/mrimcontact.h"
#include "../coresrc/MRIMClient.h"
#include "../coresrc/MRIMUtils.h"
#include "../coresrc/MRIMPluginSystem.h"
#include <QTextCodec>

SMSWidget::SMSWidget(MRIMClient* aClient, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::SMSWidget), m_parentClient(aClient)
{
    m_ui->setupUi(this);
    QString codec("Latin1");
    m_latinCodec = QTextCodec::codecForName(codec.toLocal8Bit());
    m_ui->addNumberButton->setIcon(MRIMPluginSystem::PluginSystem()->getIcon("add"));
    m_addNumberWidget = new AddNumberWidget(aClient);
    connect(m_addNumberWidget,SIGNAL(numbersChanged()),this,SLOT(handleNumbersChanged()));
}

SMSWidget::~SMSWidget()
{
    delete m_ui;
    disconnect(m_addNumberWidget,SIGNAL(numbersChanged()),this,SLOT(handleNumbersChanged()));
}

void SMSWidget::show(MRIMContact* aCnt)
{
    m_cnt = aCnt;
    m_ui->smsTextEdit->clear();
    m_ui->counterLabel->clear();
    m_ui->recieverLabel->setText(m_cnt->Name());
    handleNumbersChanged();
    move(MRIMCommonUtils::DesktopCenter(size()));
    QWidget::show();
}

void SMSWidget::on_smsTextEdit_textChanged()
{
    QString text = m_ui->smsTextEdit->toPlainText();
    bool latin = m_latinCodec->canEncode(text);
    int maxLen = (latin)? 144 : 44;
    if (text.length() > maxLen)
    {
        text.truncate(maxLen);
        m_ui->smsTextEdit->setPlainText(text);
    }
    m_ui->counterLabel->setText(QString("%1/%2").arg(text.length()).arg(maxLen));
}

void SMSWidget::on_sendButton_clicked()
{
    m_parentClient->Protocol()->SendSMS(m_ui->numbersComboBox->currentText(),m_ui->smsTextEdit->toPlainText());
    hide();
}

void SMSWidget::on_addNumberButton_clicked()
{
    m_addNumberWidget->show(m_cnt);
}

void SMSWidget::handleNumbersChanged()
{
    m_ui->numbersComboBox->clear();
    m_ui->numbersComboBox->addItems(m_cnt->Phone());
}
