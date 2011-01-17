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

#ifndef MRIMLOGINWIDGET_H
#define MRIMLOGINWIDGET_H

#include <QtGui/QWidget>
#include "ui_mrimloginwidget.h"

class MRIMLoginWidget : public QWidget
{
    Q_OBJECT

public:
    MRIMLoginWidget(QWidget *parent = 0);
    ~MRIMLoginWidget();
    QString GetLoginText();
    QString GetPassText();
private:
    Ui::MRIMLoginWidgetClass ui;
};

#endif // MRIMLOGINWIDGET_H
