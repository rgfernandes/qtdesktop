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

#include "ui_filetransferwidget.h"
#include "filetransferwidget.h"
#include "../coresrc/MRIMPluginSystem.h"
#include "../coresrc/MRIMUtils.h"

FileTransferWidget::FileTransferWidget(MRIMClient* aClient, FileTransferRequest aReq, QString aLocation, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::FileTransferWidget), m_req(aReq), m_speedBytes(0), m_client(aClient), m_location(aLocation)
{
    m_ui->setupUi(this);
    move(MRIMCommonUtils::DesktopCenter(size()));
    setWindowIcon(MRIMPluginSystem::PluginSystem()->getIcon("save_all"));
    setWindowTitle(tr("File transfer with: %1").arg(m_req.From));
    m_ui->doneLabel->setText("0");
    m_ui->statusLabel->setText(tr("Waiting..."));
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);

    if (m_location.length() > 0 && m_req.From.length() > 0)
        m_transferMode = TM_RECIEVE_CLIENT;
    else
    {
        m_transferMode = TM_SEND_SERVER;
    }    
    StartTransfer();
}

void FileTransferWidget::StartTransfer()
{
    m_currentStatus = FT_IDLE;
    m_speedBytes = 0;
    m_IPsHashIter = new QHashIterator<QString,quint32>(m_req.IPsDict);
    m_IPsHashIter->toFront();
    m_filesHashIter = new QHashIterator<QString,quint32>(m_req.FilesDict);
    m_filesHashIter->toFront();

    switch (m_transferMode)
    {
        case TM_RECIEVE_CLIENT:
        {
            m_socket = new QTcpSocket;
            connect(m_socket,SIGNAL(connected()),this,SLOT(ConnectedToPeer()));
            connect(m_socket,SIGNAL(readyRead()),this,SLOT(ReadyRead()));
            connect(m_socket,SIGNAL(disconnected()),this,SLOT(Disconnected()));
            connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(SocketError(QAbstractSocket::SocketError)));
            QHash<QString, quint32>::const_iterator currIp = m_IPsHashIter->next();
            m_currentStatus = FT_CONNECTING;
            qDebug()<<"MRIM: FT: Connecting to "<<currIp.key()<<":"<<currIp.value();
            m_socket->connectToHost(currIp.key(),currIp.value());
        }
        break;
        case TM_SEND_SERVER:
        {
            m_sentFilesCount = 0;
            m_server = new QTcpServer();
            connect(m_server,SIGNAL(newConnection()),this,SLOT(ClientConnected()));
            qDebug()<<"MRIM: FT: Starting server @ 127.0.0.1:"<<m_req.IPsDict.values().at(0);
            m_currentStatus = FT_WAIT_FOR_CLIENT;
            m_server->listen(QHostAddress(QHostAddress::LocalHost),m_req.IPsDict.values().at(0));
        }
        break;
    }

}

FileTransferWidget::~FileTransferWidget()
{
    delete m_ui;
}

void FileTransferWidget::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FileTransferWidget::ConnectedToPeer()
{
    m_currentStatus = FT_WAIT_FOR_HELLO;
    SendCmd("MRA_FT_HELLO "+m_client->GetAccountInfo().account_name);
}

void FileTransferWidget::ReadyRead()
{
    switch (m_transferMode)
    {
    case TM_RECIEVE_CLIENT:
    {
        if (m_currentStatus == FT_WAIT_FOR_HELLO)
        {
            QString cmd(m_socket->readAll());
            qDebug()<<"File transfer cmd recieved: "<<cmd;
            if (cmd.contains("MRA_FT_HELLO") && cmd.contains(m_req.From))
            {
                GetNextFile();
            }
        }
        else
        {
            m_currentFileSize += m_socket->bytesAvailable();
            m_speedBytes += m_socket->bytesAvailable();
            m_ui->doneLabel->setText(MRIMCommonUtils::GetFileSize(m_currentFileSize));
            m_ui->progressBar->setValue(m_currentFileSize);
            m_currentFile.write(m_socket->readAll());

            if (m_currentFileSize >= m_filesHashIter->value())
            {
                //done with current file
                m_currentFile.close();
                m_currentStatus = FT_TRANSFER_FILE_COMPLETED;
                GetNextFile();
            }
        }
    }
    break;
    case TM_SEND_SERVER:
    {
        if (m_currentStatus == FT_WAIT_FOR_HELLO)
        {
            QString cmd(m_socket->readAll());
            qDebug()<<"File transfer cmd recieved: "<<cmd;
            if (cmd.contains("MRA_FT_HELLO") && cmd.contains(m_req.To))
            {
                SendCmd("MRA_FT_HELLO "+m_client->GetAccountInfo().account_name);
                m_currentStatus = FT_WAIT_FOR_TRANSFER;
            }
        }
        else if (m_currentStatus == FT_WAIT_FOR_TRANSFER && m_sentFilesCount < m_req.FilesInfo.count())
        {
            QString cmdStr(m_socket->readAll());
            qDebug()<<"File transfer cmd recieved: "<<cmdStr;
            QStringList cmd = cmdStr.split(' ');
            if (cmd.contains("MRA_FT_GET_FILE"))
            {
                m_currentStatus = FT_TRANSFER;
                SendFile(cmd[1]);
            }
        }
    }
    break;
    }
}

void FileTransferWidget::Disconnected()
{
    qDebug()<<"MRIM: FT: Disconnected";
    if (m_currentStatus == FT_CONNECTING && m_IPsHashIter->hasNext())
    {
        QHash<QString, quint32>::const_iterator currIp = m_IPsHashIter->next();
        m_currentStatus = FT_CONNECTING;
        qDebug()<<"MRIM: FT: Connecting to "<<currIp.key()<<":"<<currIp.value();
        m_socket->connectToHost(currIp.key(),currIp.value());
    }
    else if (m_currentStatus != FT_TRANSFER_COMPLETED && m_currentStatus != FT_TRANSFER_CANCELLED)
    {
        m_currentStatus = FT_DISCONNECTED;
    }
}

void FileTransferWidget::SendCmd(const QString& aCmd)
{
    QString codepage = "CP1251";
    QTextCodec* codec = QTextCodec::codecForName(codepage.toLocal8Bit());
    if (codec != NULL)
    {
        m_socket->write(codec->fromUnicode(aCmd));
    }
    else
    {
        m_socket->write(aCmd.toLatin1());
    }
}

void FileTransferWidget::UpdateProgress()
{
    qint64 progress = 0, totalSize = 0;
    if (m_transferMode == TM_RECIEVE_CLIENT)
    {
        progress = m_currentFileSize;
        totalSize = m_filesHashIter->value();
    }
    else if (m_transferMode == TM_SEND_SERVER)
    {
        progress = m_currentFile.pos();
        totalSize = m_currentFile.size();
    }

    m_ui->doneLabel->setText(MRIMCommonUtils::GetFileSize(progress));
    m_ui->progressBar->setValue(progress);
    m_ui->speedLabel->setText(MRIMCommonUtils::GetFileSize(m_speedBytes)+tr("/sec"));
    SetRemainTime();
    m_speedBytes = 0;

    if (progress >= totalSize)
    {
        m_ui->statusLabel->setText(tr("Done!"));
        m_ui->speedLabel->setText("");
        return;
    }

    if ( m_socket->state() == QAbstractSocket::ConnectedState )
        QTimer::singleShot(1000, this, SLOT(UpdateProgress()));
}

void FileTransferWidget::GetNextFile()
{
    if (m_filesHashIter->hasNext())
    {
        m_speedBytes = 0;
        m_currentFileSize = 0;
        QHash<QString, quint32>::const_iterator currFile = m_filesHashIter->next();
        m_ui->progressBar->setMaximum(currFile.value());
        m_ui->progressBar->setValue(0);
        m_ui->fileSizeLabel->setText(MRIMCommonUtils::GetFileSize(currFile.value()));
        m_ui->doneLabel->setText(MRIMCommonUtils::GetFileSize(0));
        m_ui->statusLabel->setText(tr("Getting file..."));
        m_ui->fileNameLabel->setText(currFile.key());
        m_ui->speedLabel->clear();
        QString reqCmd("MRA_FT_GET_FILE "+currFile.key());        
        m_currentFile.setFileName(m_location+currFile.key());
        m_currentFile.open(QIODevice::WriteOnly);
        SendCmd(reqCmd);
        m_currentStatus = FT_WAIT_FOR_TRANSFER;
        UpdateProgress();
    }
    else
    {
        m_client->Protocol()->FileTransferCompleted(m_req.UniqueId);
        m_currentStatus = FT_TRANSFER_COMPLETED;
        m_socket->disconnectFromHost();
        m_ui->cancelButton->setText(tr("Close"));
        if (m_ui->closeAfterTransfer->checkState() == Qt::Checked)
        {
            close();
        }
    }
}

void FileTransferWidget::SetRemainTime()
{
    if(m_speedBytes)
    {
        qint64 progress = 0, totalSize = 0;
        if (m_transferMode == TM_RECIEVE_CLIENT)
        {
            progress = m_currentFileSize;
            totalSize = m_filesHashIter->value();
        }
        else if (m_transferMode == TM_SEND_SERVER)
        {
            progress = m_currentFile.pos();
            totalSize = m_currentFile.size();
        }

        int secsLeft = (totalSize - progress) / m_speedBytes;
        QTime tmpTime(0,0,0);
        m_ui->remainedLabel->setText(tmpTime.addSecs(secsLeft).toString());
    }
}

void FileTransferWidget::on_cancelButton_clicked()
{
    if (m_currentStatus == FT_TRANSFER_COMPLETED)
    {
        close();
    }
    else
    {
        m_client->Protocol()->DeclineFileTransfer(m_req.UniqueId);
    }
}

void FileTransferWidget::on_openButton_clicked()
{
    QDesktopServices::openUrl(QUrl(m_location));
}

void FileTransferWidget::ClientConnected()
{
    m_socket = m_server->nextPendingConnection();
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(ReadyRead()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(Disconnected()));
    connect(m_socket,SIGNAL(bytesWritten(qint64)),this,SLOT(FileBytesWritten(qint64)));
    m_currentStatus = FT_WAIT_FOR_HELLO;
    m_server->close();
}

void FileTransferWidget::SendFile(QString aFileName)
{
    if (!m_req.FilesDict.contains(aFileName)) return;

    QFileInfo info = m_req.FilesInfo.at(m_req.FilesDict.keys().indexOf(aFileName,0));
    if (info.exists())
    {
        m_speedBytes = 0;
        if (m_currentFile.isOpen()) m_currentFile.close();
        m_currentFile.setFileName(info.absoluteFilePath());
        m_currentFile.open(QIODevice::ReadOnly);
        m_currentFileChunkSize = m_currentFile.size() < 1360 ? m_currentFile.size() : 1360;

        m_ui->progressBar->setMaximum(m_currentFile.size());
        m_ui->progressBar->setValue(0);
        m_ui->fileSizeLabel->setText(MRIMCommonUtils::GetFileSize(m_currentFile.size()));
        m_ui->doneLabel->setText(MRIMCommonUtils::GetFileSize(0));
        m_ui->speedLabel->clear();
        m_ui->statusLabel->setText(tr("Sending file..."));
        m_ui->fileNameLabel->setText(m_currentFile.fileName());

        UpdateProgress();
        SendFileDataChunk();
    }
}

void FileTransferWidget::FileBytesWritten(qint64 aBytesCount)
{
    if (m_transferMode == TM_SEND_SERVER && m_currentStatus == FT_TRANSFER)
    {
        m_speedBytes += aBytesCount;
        SendFileDataChunk();
    }
}

void FileTransferWidget::SendFileDataChunk()
{
    qint64 bytesLeft = m_currentFile.size() - m_currentFile.pos();

    if (bytesLeft < 1)
    {
        m_currentFile.close();
        m_sentFilesCount++;
        if (m_sentFilesCount >= m_req.FilesInfo.count())
        {
            m_currentStatus = FT_TRANSFER_COMPLETED;
            m_socket->disconnectFromHost();
        }
        else
        {
            m_currentStatus = FT_WAIT_FOR_TRANSFER;
        }
        return;
    }

    m_socket->write(m_currentFile.read(m_currentFileChunkSize));
}

void FileTransferWidget::SocketError(QAbstractSocket::SocketError)
{
    qDebug()<<"MRIM: FT: Socket error";
    Disconnected();
}
