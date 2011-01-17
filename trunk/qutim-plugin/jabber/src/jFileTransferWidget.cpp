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

#include "jFileTransferWidget.h"
#include "utils.h"
#include "jPluginSystem.h"
#include "jConnection.h"
#include "ui_jFileTransferWidget.h"

jFileTransferWidget::jFileTransferWidget(bool send, jFileTransfer *jft, SIProfileFT *ft, const JID &from, const std::string &sid, const std::string &name, long size,
										 const std::string &hash, const std::string &date, const std::string &mimetype,
										 const std::string &desc, int stypes, QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui::jFileTransferWidget)
{
	m_ui->setupUi(this);
	setWindowIcon(jPluginSystem::instance().getIcon("save_all"));
	setWindowTitle(tr("File transfer: %1").arg(utils::fromStd(from.full())));
	m_ui->fileNameLabel->setText(utils::fromStd(name));
	m_ui->fileSizeLabel->setText(QString::number(size));
	m_ui->doneLabel->setText("0");
	m_ui->progressBar->setMaximum(size);
	m_thread = new ConnectThread(this);
	m_file = 0;
	m_ft = ft;
	m_bs = 0;
	m_from = from;
	m_sid = sid;
	m_jft = jft;
	m_ui->statusLabel->setText(tr("Waiting..."));
	m_send = send;
	m_socket = 0;
	m_finished = false;
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);

}

jFileTransferWidget::~jFileTransferWidget()
{
	m_jft->removeWidget(m_from, m_sid, false, m_send);
	if(m_bs)
	{
		m_ft->dispose(m_bs);
	}
	delete m_ui;
}

void jFileTransferWidget::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void jFileTransferWidget::setFilePath(const QString &file_path)
{
	if(!file_path.isEmpty())
		m_ui->fileNameLabel->setText(file_path);
	m_file = new QFile(m_ui->fileNameLabel->text());
}

void jFileTransferWidget::setBytestream(Bytestream *bs)
{
	m_bs = bs;
	m_bs->registerBytestreamDataHandler(this);
	if(!m_send)
	{
		m_thread->start();
	}
}

void jFileTransferWidget::start()
{
	if(m_send)
	{
	}
	else
	{
		if(m_bs->connect())
		{
		}
		else
		{
			m_ft->dispose(m_bs);
			m_bs = 0;
		}
	}
}

void jFileTransferWidget::handleBytestreamData( Bytestream* bs, const std::string& data )
{
	m_file->write(data.c_str(), data.size());
	qDebug() << "handleBytestreamData" << data.size();
	m_ui->progressBar->setValue(m_ui->progressBar->value()+data.size());
	m_ui->doneLabel->setText(QString::number(m_ui->progressBar->value()));
}

void jFileTransferWidget::handleBytestreamError( Bytestream* bs, const IQ& iq )
{
	qDebug() << "handleBytestreamError";
}

void jFileTransferWidget::handleBytestreamOpen( Bytestream* bs )
{
	if(m_send)
	{
		m_ui->statusLabel->setText(tr("Sending..."));
		m_file->open(QIODevice::ReadOnly);
		switch(m_bs->type())
		{
		case Bytestream::S5B:{
				SOCKS5Bytestream *s5_bs = dynamic_cast<SOCKS5Bytestream *>(m_bs);
				m_socket = dynamic_cast<jConnection *>(s5_bs->connectionImpl())->getSocket();
				connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
				break;}
		case Bytestream::IBB:
			break;
		}
		sendFile();
	}
	else
	{
		m_ui->statusLabel->setText(tr("Getting..."));
		m_file->open(QIODevice::WriteOnly);
	}
	qDebug() << "handleBytestreamOpen";
}

void jFileTransferWidget::handleBytestreamClose( Bytestream* bs )
{
	m_ui->statusLabel->setText(tr("Done..."));
	m_file->close();
	qDebug() << "handleBytestreamClose";
	m_finished = true;
	m_bs = 0;
	m_ui->cancelButton->setText(tr("Close"));
}

void jFileTransferWidget::on_cancelButton_clicked()
{
	close();
}

void jFileTransferWidget::bytesWritten(qint64 bytes)
{
	sendFile();
}

void jFileTransferWidget::sendFile()
{
	if(!m_file || m_finished)
		return;
	QByteArray data = m_file->read(m_socket?0x2000:0x0C00);
	m_ui->progressBar->setValue(m_ui->progressBar->value()+data.size());
	m_ui->doneLabel->setText(QString::number(m_ui->progressBar->value()));
	bool result;
	if(m_socket)
		result = m_socket->write(data)>0;
	else
		result = m_bs->send(std::string(data.constData(), data.size()));
	if(!m_socket && result)
		QTimer::singleShot(10, this, SLOT(sendFile()));
	if(!result || m_file->atEnd())
		m_bs->close();
}
