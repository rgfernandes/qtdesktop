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

#ifndef SMSWIDGET_H
#define SMSWIDGET_H

#include "addnumberwidget.h"
#include <QtGui/QWidget>

class MRIMContact;
class MRIMClient;
class QTextCodec;

namespace Ui {
    class SMSWidget;
}

class SMSWidget : public QWidget {
    Q_OBJECT
public:
    explicit SMSWidget(MRIMClient* aClient, QWidget *parent = 0);
    void show(MRIMContact* aCnt);
    virtual ~SMSWidget();

private:
    Ui::SMSWidget *m_ui;
    MRIMClient* m_parentClient;
    MRIMContact* m_cnt;
    QTextCodec* m_latinCodec;
    AddNumberWidget* m_addNumberWidget;
private slots:
    void handleNumbersChanged();
    void on_addNumberButton_clicked();
    void on_sendButton_clicked();
    void on_smsTextEdit_textChanged();
};

#endif // SMSWIDGET_H
