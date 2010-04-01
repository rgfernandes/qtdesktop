/*
Copyright (c) 2009 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JCONFERENCECONFIG_H
#define JCONFERENCECONFIG_H

#include <QDialog>
#include "jAccount.h"
#include "ui_jConferenceConfig.h"
#include "jDataForm.h"
#include <gloox/dataform.h>
#include <gloox/mucroom.h>
#include <gloox/mucroomconfighandler.h>

using namespace gloox;

class jConferenceConfig : public QWidget
{
	Q_OBJECT

	public:
		jConferenceConfig(jAccount *jabber_account, const QString &room, MUCRoom *room_ptr, QWidget *parent = 0);
		void setDataForm(const DataForm &form);

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void on_applyButton_clicked();
		void on_okButton_clicked();

	signals:
		void storeRoomConfig(const DataForm &form);
		void destroyDialog(const QString&);

	private:
		Ui::RoomConfig ui;
		jAccount *m_jabber_account;
		jDataForm *data_form;
		QString m_room;
		MUCRoom *m_room_ptr;
};

#endif // JCONFERENCECONFIG_H
