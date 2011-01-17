/*****************************************************************************
    AddContactWidget

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

#ifndef ADDCONTACTWIDGET_H
#define ADDCONTACTWIDGET_H

//#include <QWidget>
#include <QDialog>
#include "ui_addcontactwidget.h"
#include "../coresrc/MRIMContactList.h"

class MRIMClient;

class AddContactWidget : public QDialog
{
	Q_OBJECT

public:
        AddContactWidget(MRIMClient* aClient, QWidget *parent = 0);
	~AddContactWidget();
        void FillGroups();
	quint32 GetSelectedGroupId();
	QString GetContactEmail();
	inline QString GetNickname() { return ui.nickEdit->text(); }
	void SetEmail(QString aEmail, bool aReadOnly = false);
	void SetNick(QString aNick, bool aReadOnly = false);
private:
	Ui::AddContactWidgetClass ui;
	quint32 m_selctedGrId;
	QString m_contactEmail;
        MRIMClient* m_client;
private slots:
	void on_addContactButton_clicked();
	void on_pushButton_clicked();
};

#endif // ADDCONTACTWIDGET_H
