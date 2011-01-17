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

#ifndef RENAMEWIDGET_H
#define RENAMEWIDGET_H

#include <QtGui/QWidget>

class MRIMContact;
class MRIMClient;

namespace Ui {
    class RenameWidget;
}

class RenameWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(RenameWidget)
public:
    explicit RenameWidget(QWidget *parent = 0);
    virtual ~RenameWidget();
    void show(MRIMContact* aCnt);

private:
    Ui::RenameWidget *m_ui;
    MRIMContact* m_cnt;
private slots:
    void on_okButton_clicked();
};

#endif // RENAMEWIDGET_H
