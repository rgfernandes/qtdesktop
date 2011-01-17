/*****************************************************************************
    MoveToGroupWidget

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

#include "movetogroupwidget.h"
#include "ui_movetogroupwidget.h"
#include <QTextDocument>
#include "../coresrc/MRIMUtils.h"

MoveToGroupWidget::MoveToGroupWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MoveToGroupWidget)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton,SIGNAL(clicked()),this,SLOT(EmitMoveToGroup()));
}

MoveToGroupWidget::~MoveToGroupWidget()
{
    delete m_ui;
}

void MoveToGroupWidget::show(QString aEmail, QList<MRIMGroup*> aGrList, QString aNick)
{
    move(MRIMCommonUtils::DesktopCenter(size()));
    m_email = aEmail;
    QString titleName = (aNick.length() > 0) ? aNick : aEmail;
    setWindowTitle(tr("Move")+" "+Qt::escape(titleName));
    m_ui->groupComboBox->clear();
    for (qint32 i=0; i < aGrList.count(); i++)
    {
        quint32 grId = aGrList.at(i)->Id().toUInt();
        QVariant val(grId);
        m_ui->groupComboBox->addItem(aGrList.at(i)->Name(),val);
    }
    QWidget::show();
}

void MoveToGroupWidget::EmitMoveToGroup()
{
    hide();
    emit MoveContactToGroup(m_email,m_ui->groupComboBox->itemData(m_ui->groupComboBox->currentIndex()).toString());
}
