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

#ifndef ADDNUMBERWIDGET_H
#define ADDNUMBERWIDGET_H

#include <QtGui/QWidget>

class MRIMContact;
class MRIMClient;

namespace Ui
{ class AddNumberWidget; }

class AddNumberWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(AddNumberWidget)
signals:
    void numbersChanged();
public:
    explicit AddNumberWidget(MRIMClient* aClient, QWidget *parent = 0);
    virtual ~AddNumberWidget();
    void show(MRIMContact* aCnt);
private:
    Ui::AddNumberWidget *m_ui;
    MRIMContact* m_cnt;
    MRIMClient* m_client;
private slots:
    void on_saveButton_clicked();
};

#endif // ADDNUMBERWIDGET_H
