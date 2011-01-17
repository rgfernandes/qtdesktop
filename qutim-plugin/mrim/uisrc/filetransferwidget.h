/*****************************************************************************
    FileTransferWidget

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

#ifndef FILETRANSFERWIDGET_H
#define FILETRANSFERWIDGET_H

#include <QTcpSocket>
#include <QtGui/QWidget>
#include <QFile>
#include <QTcpServer>
#include <QHash>
#include "../coresrc/mrimdefs.h"

namespace Ui {
    class FileTransferWidget;
}

class MRIMClient;

class FileTransferWidget : public QWidget
 {
    Q_OBJECT
    Q_DISABLE_COPY(FileTransferWidget)

   enum TransferMode
    {
        TM_RECIEVE_CLIENT,
        TM_RECIEVE_SERVER,
        TM_SEND_CLIENT,
        TM_SEND_SERVER
    };

   enum FileTranferStatus
    {
        FT_IDLE = 0,
        FT_CONNECTING,
        FT_CONNECTED,
        FT_WAIT_FOR_CLIENT,
        FT_WAIT_FOR_HELLO,
        FT_WAIT_FOR_TRANSFER,
        FT_TRANSFER,
        FT_TRANSFER_FILE_COMPLETED,
        FT_TRANSFER_COMPLETED,
        FT_TRANSFER_CANCELLED,
        FT_DISCONNECTED
    };

public:
    explicit FileTransferWidget(MRIMClient* aClient, FileTransferRequest aReq, QString aLocation = QString(), QWidget *parent = 0);
    virtual ~FileTransferWidget();

protected:
    virtual void changeEvent(QEvent *e);
    void StartTransfer();
    void SendCmd(const QString& aCmd);
    void GetNextFile();
    void SetRemainTime();
private slots:
    void on_openButton_clicked();
    void on_cancelButton_clicked();
    void ConnectedToPeer();
    void ReadyRead();
    void Disconnected();
    void UpdateProgress();
    void ClientConnected();
    void SendFile(QString aFileName);
    void FileBytesWritten(qint64 aBytesCount);
    void SendFileDataChunk();
    void SocketError(QAbstractSocket::SocketError);
private:
    Ui::FileTransferWidget *m_ui;
    FileTransferRequest m_req;
    quint32 m_currentFileIndex;
    FileTranferStatus m_currentStatus;
    QTcpSocket* m_socket;
    QTcpServer* m_server;
    QHashIterator<QString,quint32>* m_IPsHashIter;
    QHashIterator<QString,quint32>* m_filesHashIter;
    QFile m_currentFile;
    qint64 m_currentFileSize;
    qint64 m_speedBytes;
    qint64 m_bytesSent;
    qint32 m_currentFileChunkSize;
    qint32 m_sentFilesCount;
    MRIMClient* m_client;
    QString m_location;
    TransferMode m_transferMode;
};

#endif // FILETRANSFERWIDGET_H
