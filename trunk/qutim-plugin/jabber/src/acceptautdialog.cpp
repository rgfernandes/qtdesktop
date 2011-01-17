/*
    Сopyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "acceptautdialog.h"
#include "utils.h"
#include <gloox/rostermanager.h>

AcceptAuthDialog::AcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client, QWidget *parent) :
	QDialog(parent),
	m_jid(jid)
{
    ui.setupUi(this);
    ui.authorizeText->setText(text);
    setFixedSize(size());
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_jabber_client = jabber_client;
    setWindowTitle(utils::fromStd(jid.full()));
}

void AcceptAuthDialog::on_authorizeButton_clicked()
{
    m_jabber_client->rosterManager()->ackSubscriptionRequest(m_jid,true);
    close();
}

void AcceptAuthDialog::on_denyButton_clicked()
{
    m_jabber_client->rosterManager()->ackSubscriptionRequest(m_jid,false);
    close();
}
