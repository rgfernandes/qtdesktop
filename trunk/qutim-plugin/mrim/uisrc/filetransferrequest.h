/*****************************************************************************
    FileTransferRequest

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

#ifndef FILETRANSFERREQUEST_H
#define FILETRANSFERREQUEST_H

#include <QtGui/QWidget>
#include "../coresrc/mrimdefs.h"

namespace Ui {
    class FileTransferRequestWidget;
}

class MRIMClient;

class FileTransferRequestWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(FileTransferRequestWidget)
public:
    explicit FileTransferRequestWidget(MRIMClient* aClient, const FileTransferRequest& aReq, QWidget *parent = 0);
    virtual ~FileTransferRequestWidget();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::FileTransferRequestWidget *m_ui;
    MRIMClient* m_client;
    FileTransferRequest m_req;
private slots:
    void on_declineButton_clicked();
    void on_acceptButton_clicked();
};

#endif // FILETRANSFERREQUEST_H
