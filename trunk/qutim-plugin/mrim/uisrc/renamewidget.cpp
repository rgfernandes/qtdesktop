/*****************************************************************************
    RenameWidget

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

#include "renamewidget.h"
#include "ui_renamewidget.h"
#include "../coresrc/mrimcontact.h"
#include "../coresrc/MRIMClient.h"
#include "../coresrc/MRIMUtils.h"

RenameWidget::RenameWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::RenameWidget),
    m_cnt(0)
{
    m_ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

RenameWidget::~RenameWidget()
{
    delete m_ui;
}

void RenameWidget::show(MRIMContact* aCnt)
{
    if (!aCnt) return;

    m_cnt = aCnt;
    setWindowTitle(tr("Rename")+" "+m_cnt->Name());
    m_ui->newNameEdit->clear();
    move(MRIMCommonUtils::DesktopCenter(size()));
    QWidget::show();
}

void RenameWidget::on_okButton_clicked()
{
    m_cnt->Rename(m_ui->newNameEdit->text());
    close();
}


