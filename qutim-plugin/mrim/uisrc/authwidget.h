/*****************************************************************************
    authwidget

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

#ifndef AUTHWIDGET_H
#define AUTHWIDGET_H

#include <QWidget>
#include "ui_authwidget.h"

class MRIMClient;

class authwidget : public QWidget
{
	Q_OBJECT

public:
        authwidget(MRIMClient* aClient, QWidget *parent = 0);
	~authwidget();
        void SetupAuthRequest(QString aText, const QString& aContact);
private:
        void AcceptAuth();

	Ui::authwidgetClass ui;
        MRIMClient* m_client;
        QString m_contact;
private slots:
	void on_rejectButton_clicked();
	void on_authButton_clicked();
};

#endif // AUTHWIDGET_H
