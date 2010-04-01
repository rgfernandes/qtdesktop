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

#ifndef MOVETOGROUPWIDGET_H
#define MOVETOGROUPWIDGET_H

#include <QtGui/QWidget>
#include "../coresrc/mrimgroup.h"

namespace Ui {
    class MoveToGroupWidget;
}

class MoveToGroupWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MoveToGroupWidget)
public:
    explicit MoveToGroupWidget(QWidget *parent = 0);
    virtual ~MoveToGroupWidget();
    void show(QString aEmail, QList<MRIMGroup*> aGrList, QString aNick = QString());
private slots:
    void EmitMoveToGroup();
signals:
    void MoveContactToGroup(QString aEmail, QString aGrId);
private:
    Ui::MoveToGroupWidget *m_ui;
    QString m_email;
};

#endif // MOVETOGROUPWIDGET_H
