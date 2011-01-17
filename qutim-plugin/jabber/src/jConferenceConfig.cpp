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

#include "jConferenceConfig.h"
#include "utils.h"

jConferenceConfig::jConferenceConfig(jAccount *jabber_account, const QString &room, MUCRoom *room_ptr, QWidget *parent) : QWidget(parent)
{
	m_room = room;
	m_room_ptr = room_ptr;
	m_jabber_account = jabber_account;
	ui.setupUi(this);
	ui.okButton->setIcon(m_jabber_account->getPluginSystem().getIcon("apply"));
	ui.applyButton->setIcon(m_jabber_account->getPluginSystem().getIcon("apply"));
	ui.cancelButton->setIcon(m_jabber_account->getPluginSystem().getIcon("cancel"));
	SystemsCity::PluginSystem()->centerizeWidget(this);
}

void jConferenceConfig::closeEvent(QCloseEvent *event)
{
	emit destroyDialog(m_room);
}

void jConferenceConfig::setDataForm(const DataForm &form)
{
	data_form = new jDataForm(new DataForm(form), false, this);
	QGridLayout *layout = new QGridLayout(this);
	ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	ui.scrollArea->setWidgetResizable(true);
	ui.scrollAreaWidgetContents->setLayout(layout);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	data_form->setSizePolicy(sizePolicy);
	layout->addWidget(data_form);
}

void jConferenceConfig::on_applyButton_clicked()
{
	m_room_ptr->setRoomConfig(data_form->getDataForm());
}

void jConferenceConfig::on_okButton_clicked()
{
	on_applyButton_clicked();
	close();
}
