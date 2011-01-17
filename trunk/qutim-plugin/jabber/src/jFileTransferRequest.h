/*****************************************************************************
    jFileTransfer

    Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef JFILETRANSFERREQUEST_H
#define JFILETRANSFERREQUEST_H

#include <QtGui/QWidget>
#include <gloox/siprofileft.h>
#include "jFileTransfer.h"

class jFileTransfer;
using namespace gloox;

namespace Ui {
    class jFileTransferRequest;
}

class jFileTransferRequest : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(jFileTransferRequest)
public:
    explicit jFileTransferRequest(jFileTransfer *jft, jFileTransferWidget *ft_widget, SIProfileFT *ft, const JID &from, const std::string &sid, const std::string &name, long size,
				  const std::string &hash, const std::string &date, const std::string &mimetype,
				  const std::string &desc, int stypes, QWidget *parent = 0);
    virtual ~jFileTransferRequest();

private slots:
	void on_declineButton_clicked();
	void on_acceptButton_clicked();
protected:
    virtual void changeEvent(QEvent *e);

private:
    jFileTransfer *m_jft;
    SIProfileFT *m_ft;
    jFileTransferWidget *m_ft_widget;
    JID m_from;
    std::string m_sid;
    int m_stypes;
    bool m_decline_on_delete;
    Ui::jFileTransferRequest *m_ui;
};

#endif // JFILETRANSFERREQUEST_H
