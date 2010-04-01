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

#ifndef JFILETRANSFERWIDGET_H
#define JFILETRANSFERWIDGET_H

#include <QtGui/QWidget>
#include <QThread>
#include <QFile>
#include <QTcpSocket>
#include <gloox/siprofileft.h>
#include <gloox/siprofilefthandler.h>
#include <gloox/bytestream.h>
#include <gloox/bytestreamdatahandler.h>
#include <gloox/socks5bytestream.h>
#include "jFileTransfer.h"

using namespace gloox;

namespace Ui {
    class jFileTransferWidget;
}

class jFileTransferWidget;
class jFileTransfer;

class ConnectThread : public QThread
{
    Q_OBJECT
public:
    ConnectThread(QObject *parent) : QThread(parent)
    { connect(this, SIGNAL(atConnection()), parent, SLOT(start())); }
    virtual void run()
    { emit atConnection(); }
signals:
    void atConnection();
};

class jFileTransferWidget : public QWidget, BytestreamDataHandler {
    Q_OBJECT
    Q_DISABLE_COPY(jFileTransferWidget)
public:
    explicit jFileTransferWidget(bool send, jFileTransfer *jft, SIProfileFT *ft, const JID &from, const std::string &sid, const std::string &name, long size,
				  const std::string &hash="", const std::string &date="", const std::string &mimetype="",
				  const std::string &desc="", int stypes=SIProfileFT::FTTypeAll, QWidget *parent = 0);
    virtual ~jFileTransferWidget();
    void setFilePath(const QString &file_path="");
    void setBytestream(Bytestream *bs);
    void handleBytestreamData( Bytestream* bs, const std::string& data );
    void handleBytestreamError( Bytestream* bs, const IQ& iq );
    void handleBytestreamOpen( Bytestream* bs );
    void handleBytestreamClose( Bytestream* bs );
	bool isSending() { return m_send; }
public slots:
    void start();
    void on_cancelButton_clicked();
    void bytesWritten(qint64 bytes=0);
    void sendFile();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::jFileTransferWidget *m_ui;
    jFileTransfer *m_jft;
    SIProfileFT *m_ft;
    Bytestream *m_bs;
    ConnectThread *m_thread;
    QFile *m_file;
    QTcpSocket *m_socket;
    JID m_from;
    std::string m_sid;
    bool m_send;
    bool m_finished;
};

#endif // JFILETRANSFERWIDGET_H
