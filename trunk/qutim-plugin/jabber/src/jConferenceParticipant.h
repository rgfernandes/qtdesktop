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

#ifndef JCONFERENCEPARTICIPANT_H
#define JCONFERENCEPARTICIPANT_H

#include <QDialog>
#include "jAccount.h"
#include "ui_jConferenceParticipant.h"
#include "jDataForm.h"
#include <gloox/dataform.h>
#include <gloox/mucroom.h>
#include <gloox/mucroomconfighandler.h>

using namespace gloox;

class jConferenceParticipant : public QWidget
{
	Q_OBJECT

	public:
		jConferenceParticipant(jAccount *jabber_account, const QString &room, QWidget *parent = 0);
		void setUserList(const MUCListItemList &items, MUCOperation operation);

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void on_applyButton_clicked();
		void on_okButton_clicked();
		void addRow();
		void delRow();

	signals:
		void storeRoomParticipant(const QString &room, const MUCListItemList &items, MUCOperation operation);
		void destroyDialog(const QString&);

	private:
		Ui::RoomParticipant ui;
		jAccount *m_jabber_account;
		jDataForm *data_form;
		QString m_room;
		MUCListItemList m_owner_list, m_admin_list, m_member_list, m_ban_list;
};

#endif // JCONFERENCEPARTICIPANT_H
