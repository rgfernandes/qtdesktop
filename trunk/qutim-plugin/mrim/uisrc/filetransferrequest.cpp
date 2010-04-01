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

#include "filetransferrequest.h"
#include <QLabel>
#include <QFileDialog>
#include "../coresrc/MRIMPluginSystem.h"
#include "../coresrc/MRIMUtils.h"
#include "ui_filetransferrequest.h"
#include "filetransferwidget.h"

FileTransferRequestWidget::FileTransferRequestWidget(MRIMClient* aClient, const FileTransferRequest& aReq, QWidget *parent) :
    QWidget(parent), m_ui(new Ui::FileTransferRequestWidget), m_client(aClient), m_req(aReq)
{
    m_ui->setupUi(this);
    move(MRIMCommonUtils::DesktopCenter(size()));
    setWindowTitle(tr("File transfer request from %1").arg(m_req.From));
    m_ui->fileLabel->setPixmap(MRIMPluginSystem::ImplPointer()->PluginSystem()->getIcon("filerequest").pixmap(128, 128));
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowIcon(MRIMPluginSystem::ImplPointer()->PluginSystem()->getIcon("save_all"));
    m_ui->fromLabel->setText(m_req.From);
    m_ui->filesTreeWidget->setColumnWidth(0,100);
    m_ui->filesTreeWidget->setColumnWidth(1,40);

    qint64 totalSize = 0;
    for (qint32 i = 0; i<m_req.FilesDict.count();i++)
    {       
        QTreeWidgetItem* fileTreeItem = new QTreeWidgetItem(m_ui->filesTreeWidget);
        fileTreeItem->setText(0,m_req.FilesDict.keys().at(i));
        qint64 fileSize = m_req.FilesDict.values().at(i);
        fileTreeItem->setText(1,MRIMCommonUtils::GetFileSize(fileSize));
        totalSize += fileSize;
    }
    m_ui->fileSizeLabel->setText(MRIMCommonUtils::GetFileSize(totalSize));
}

FileTransferRequestWidget::~FileTransferRequestWidget()
{
    delete m_ui;
}

void FileTransferRequestWidget::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FileTransferRequestWidget::on_acceptButton_clicked()
{
    QString location = QFileDialog::getExistingDirectory(this,tr("Choose location to save file(s)"),QDesktopServices::storageLocation(QDesktopServices::HomeLocation),QFileDialog::ShowDirsOnly);
    if ( !location.isEmpty() )
    {
        qDebug()<<"Will recieve files to: "<<location;
        FileTransferWidget* trWidget = new FileTransferWidget(m_client,m_req,location);
        trWidget->show();
        close();
    }
}

void FileTransferRequestWidget::on_declineButton_clicked()
{
    m_client->Protocol()->DeclineFileTransfer(m_req.UniqueId);
    close();
}
