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

#ifndef AcceptAuthDialog_H
#define AcceptAuthDialog_H

#include "ui_acceptautdialog.h"
#include <QtGui>
#include <gloox/client.h>
#include <gloox/jid.h>

using namespace gloox;

class AcceptAuthDialog : public QDialog 
{
    Q_OBJECT
public:
    AcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client, QWidget *parent=0);
public slots:
    void on_authorizeButton_clicked();
    void on_denyButton_clicked();
private:
    Ui::AcceptAuthDialog ui;
    JID m_jid;
    Client *m_jabber_client;
};

#endif
