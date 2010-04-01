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

#include "jConferenceParticipant.h"
#include "utils.h"

jConferenceParticipant::jConferenceParticipant(jAccount *jabber_account, const QString &room, QWidget *parent) : QWidget(parent)
{
	m_room = room;
	m_jabber_account = jabber_account;
	ui.setupUi(this);
	ui.buttonOwnerAdd->setIcon(m_jabber_account->getPluginSystem().getIcon("add"));
	ui.buttonAdminAdd->setIcon(m_jabber_account->getPluginSystem().getIcon("add"));
	ui.buttonMemberAdd->setIcon(m_jabber_account->getPluginSystem().getIcon("add"));
	ui.buttonBanAdd->setIcon(m_jabber_account->getPluginSystem().getIcon("add"));
	ui.buttonOwnerDel->setIcon(m_jabber_account->getPluginSystem().getIcon("remove"));
	ui.buttonAdminDel->setIcon(m_jabber_account->getPluginSystem().getIcon("remove"));
	ui.buttonMemberDel->setIcon(m_jabber_account->getPluginSystem().getIcon("remove"));
	ui.buttonBanDel->setIcon(m_jabber_account->getPluginSystem().getIcon("remove"));
	ui.tabOwner->setEnabled(false);
	ui.tabAdmin->setEnabled(false);
	ui.tabMember->setEnabled(false);
	ui.tabBan->setEnabled(false);
	connect(ui.buttonOwnerAdd, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(ui.buttonAdminAdd, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(ui.buttonMemberAdd, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(ui.buttonBanAdd, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(ui.buttonOwnerDel, SIGNAL(clicked()), this, SLOT(delRow()));
	connect(ui.buttonAdminDel, SIGNAL(clicked()), this, SLOT(delRow()));
	connect(ui.buttonMemberDel, SIGNAL(clicked()), this, SLOT(delRow()));
	connect(ui.buttonBanDel, SIGNAL(clicked()), this, SLOT(delRow()));
	ui.okButton->setIcon(m_jabber_account->getPluginSystem().getIcon("apply"));
	ui.applyButton->setIcon(m_jabber_account->getPluginSystem().getIcon("apply"));
	ui.cancelButton->setIcon(m_jabber_account->getPluginSystem().getIcon("cancel"));
	ui.applyButton->setEnabled(false);
}

void jConferenceParticipant::closeEvent(QCloseEvent *event)
{
	emit destroyDialog(m_room);
}

void jConferenceParticipant::setUserList(const MUCListItemList &items, MUCOperation operation)
{
	QTableWidget *table = 0;
	if (operation == RequestOwnerList)
	{
		table = ui.ownerTable;
		ui.tabOwner->setEnabled(true);
		m_owner_list = items;
	}
	else if (operation == RequestAdminList)
	{
		table = ui.adminTable;
		ui.tabAdmin->setEnabled(true);
		m_admin_list = items;
	}
	else if (operation == RequestMemberList)
	{
		table = ui.memberTable;
		ui.tabMember->setEnabled(true);
		m_member_list = items;
	}
	else if (operation == RequestBanList)
	{
		table = ui.banTable;
		ui.tabBan->setEnabled(true);
		m_ban_list = items;
	}
	if (table)
	{
		foreach (MUCListItem item, items)
		{
			int row = table->rowCount();
			table->insertRow(row);
			QTableWidgetItem *table_item = new QTableWidgetItem(utils::fromStd(item.jid().full()));
			table->setItem(row, 0, table_item);
			if (table == ui.banTable)
			{
				table_item = new QTableWidgetItem(utils::fromStd(item.reason()));
				table->setItem(row, 1, table_item);
			}
		}
	}
}

void jConferenceParticipant::addRow()
{
	ui.applyButton->setEnabled(true);
	QToolButton *button = qobject_cast<QToolButton*>(sender());
	QTableWidget *table = 0;
	if (button == ui.buttonOwnerAdd)
	{
		table = ui.ownerTable;
	}
	else if (button == ui.buttonAdminAdd)
	{
		table = ui.adminTable;
	}
	else if (button == ui.buttonMemberAdd)
	{
		table = ui.memberTable;
	}
	else if (button == ui.buttonBanAdd)
	{
		table = ui.banTable;
	}
	if (table)
	{
		int row = table->rowCount();
		table->insertRow(row);
		QTableWidgetItem *table_item = new QTableWidgetItem();
		table->setItem(row, 0, table_item);
		table->editItem(table_item);
		if (table == ui.banTable)
		{
			table_item = new QTableWidgetItem();
			table->setItem(row, 1, table_item);
		}
	}
}

void jConferenceParticipant::delRow()
{
	ui.applyButton->setEnabled(true);
	QToolButton *button = qobject_cast<QToolButton*>(sender());
	QTableWidget *table = 0;
	if (button == ui.buttonOwnerDel)
	{
		table = ui.ownerTable;
	}
	else if (button == ui.buttonAdminDel)
	{
		table = ui.adminTable;
	}
	else if (button == ui.buttonMemberDel)
	{
		table = ui.memberTable;
	}
	else if (button == ui.buttonBanDel)
	{
		table = ui.banTable;
	}
	if (table)
	{
		int row = table->currentRow();
		if (row > -1)
			table->removeRow(row);
	}
}

void jConferenceParticipant::on_applyButton_clicked()
{
	if (!ui.applyButton->isEnabled())
		return;
	ui.applyButton->setEnabled(false);
	QList<MUCListItem> owner_list;
	for (int row = 0; row < ui.ownerTable->rowCount(); row++)
	{
		if (ui.ownerTable->item(row, 0)->text().isEmpty())
			continue;
		MUCListItem item = MUCListItem(JID(utils::toStd(ui.ownerTable->item(row, 0)->text())));
		owner_list << item;
	}
	QList<MUCListItem> admin_list;
	for (int row = 0; row < ui.adminTable->rowCount(); row++)
	{
		if (ui.adminTable->item(row, 0)->text().isEmpty())
			continue;
		MUCListItem item = MUCListItem(JID(utils::toStd(ui.adminTable->item(row, 0)->text())));
		admin_list << item;
	}
	QList<MUCListItem> member_list;
	for (int row = 0; row < ui.memberTable->rowCount(); row++)
	{
		if (ui.memberTable->item(row, 0)->text().isEmpty())
			continue;
		MUCListItem item = MUCListItem(JID(utils::toStd(ui.memberTable->item(row, 0)->text())));
		member_list << item;
	}
	QList<MUCListItem> ban_list;
	for (int row = 0; row < ui.banTable->rowCount(); row++)
	{
		if (ui.banTable->item(row, 0)->text().isEmpty())
			continue;
		MUCListItem item = MUCListItem(JID(utils::toStd(ui.banTable->item(row, 0)->text())));
		ban_list << item;
	}
	QList<MUCListItem> participant_list;
	foreach (MUCListItem item, owner_list)
	{
		bool exist = false;
		foreach (MUCListItem prev_item, m_owner_list)
			if (item.jid() == prev_item.jid())
				exist = true;
		if (!exist)
		{
			MUCListItem new_item = MUCListItem(item.jid(), RoleInvalid, AffiliationOwner, item.jid().full());
			participant_list << new_item;
		}
	}
	foreach (MUCListItem item, admin_list)
	{
		bool exist = false;
		foreach (MUCListItem prev_item, m_admin_list)
			if (item.jid() == prev_item.jid())
				exist = true;
		if (!exist)
		{
			MUCListItem new_item = MUCListItem(item.jid(), RoleInvalid, AffiliationAdmin, item.jid().full());
			participant_list << new_item;
		}
	}
	foreach (MUCListItem item, member_list)
	{
		bool exist = false;
		foreach (MUCListItem prev_item, m_member_list)
			if (item.jid() == prev_item.jid())
				exist = true;
		if (!exist)
		{
			MUCListItem new_item = MUCListItem(item.jid(), RoleInvalid, AffiliationMember, item.jid().full());
			participant_list << new_item;
		}
	}
	foreach (MUCListItem item, ban_list)
	{
		bool exist = false;
		foreach (MUCListItem prev_item, m_ban_list)
			if (item.jid() == prev_item.jid())
				exist = true;
		if (!exist)
		{
			MUCListItem new_item = MUCListItem(item.jid(), RoleInvalid, AffiliationOutcast, item.jid().full());
			participant_list << new_item;
		}
	}
	foreach (MUCListItem prev_item, m_owner_list)
	{
		bool exist = false;
		foreach (MUCListItem item, owner_list)
			if (item.jid() == prev_item.jid())
				exist = true;
		if (!exist)
		{
			foreach (MUCListItem item, participant_list)
				if (item.jid() == prev_item.jid())
					exist = true;
			if (!exist)
			{
				MUCListItem new_item = MUCListItem(prev_item.jid(), RoleInvalid, AffiliationNone, prev_item.jid().full());
				participant_list << new_item;
			}
		}
	}
	foreach (MUCListItem prev_item, m_admin_list)
	{
		bool exist = false;
		foreach (MUCListItem item, admin_list)
			if (item.jid() == prev_item.jid())
				exist = true;
		if (!exist)
		{
			foreach (MUCListItem item, participant_list)
				if (item.jid() == prev_item.jid())
					exist = true;
			if (!exist)
			{
				MUCListItem new_item = MUCListItem(prev_item.jid(), RoleInvalid, AffiliationNone, prev_item.jid().full());
				participant_list << new_item;
			}
		}
	}
	foreach (MUCListItem prev_item, m_member_list)
	{
		bool exist = false;
		foreach (MUCListItem item, member_list)
			if (item.jid() == prev_item.jid())
				exist = true;
		if (!exist)
		{
			foreach (MUCListItem item, participant_list)
				if (item.jid() == prev_item.jid())
					exist = true;
			if (!exist)
			{
				MUCListItem new_item = MUCListItem(prev_item.jid(), RoleInvalid, AffiliationNone, prev_item.jid().full());
				participant_list << new_item;
			}
		}
	}
	foreach (MUCListItem prev_item, m_ban_list)
	{
		bool exist = false;
		foreach (MUCListItem item, ban_list)
			if (item.jid() == prev_item.jid())
				exist = true;
		if (!exist)
		{
			foreach (MUCListItem item, participant_list)
				if (item.jid() == prev_item.jid())
					exist = true;
			if (!exist)
			{
				MUCListItem new_item = MUCListItem(prev_item.jid(), RoleInvalid, AffiliationNone, prev_item.jid().full());
				participant_list << new_item;
			}
		}
	}
	emit storeRoomParticipant(m_room, participant_list.toStdList(), RequestOwnerList);
}

void jConferenceParticipant::on_okButton_clicked()
{
	on_applyButton_clicked();
	close();
}
