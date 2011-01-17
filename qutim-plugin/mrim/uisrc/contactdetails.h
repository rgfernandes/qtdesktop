/*****************************************************************************
    ContactDetails

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

#ifndef CONTACTDETAILS_H
#define CONTACTDETAILS_H

#include <QWidget>
#include "ui_contactdetails.h"
#include "../coresrc/mrimdefs.h"

class MRIMClient;

class ContactDetails : public QWidget
{
	Q_OBJECT

public:
        ContactDetails(MRIMClient* aClient,QWidget *parent = 0);
	~ContactDetails();
	void SetInfo(MRIMSearchParams aInfo);
	void ResetInfo();
        void show(const MRIMSearchParams& aInfo);

private slots:
        void on_addToCLButton_clicked();
        void SetAvatarLabelText(QString aEmail);
private:
	Ui::ContactDetailsClass ui;
        MRIMClient* m_client;
        QString m_email;
};

#endif // CONTACTDETAILS_H
