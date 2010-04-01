/*****************************************************************************
    MRIMLoginWidget

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

#include "mrimloginwidget.h"

MRIMLoginWidget::MRIMLoginWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

MRIMLoginWidget::~MRIMLoginWidget()
{

}

QString MRIMLoginWidget::GetLoginText()
{
	return ui.loginEdit->text();
}

QString MRIMLoginWidget::GetPassText()
{
	return ui.passEdit->text();
}
