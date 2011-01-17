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

#include "jFileTransferRequest.h"
#include <QLabel>
#include <QFileDialog>
#include "utils.h"
#include "jPluginSystem.h"
#include "ui_jFileTransferRequest.h"

jFileTransferRequest::jFileTransferRequest(jFileTransfer *jft, jFileTransferWidget *ft_widget, SIProfileFT *ft, const JID &from, const std::string &sid, const std::string &name, long size,
										   const std::string &hash, const std::string &date, const std::string &mimetype,
										   const std::string &desc, int stypes, QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui::jFileTransferRequest)
{
	m_ui->setupUi(this);
	m_ui->fileLabel->setPixmap(jPluginSystem::instance().getIcon("filerequest").pixmap(128, 128));
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowIcon(jPluginSystem::instance().getIcon("save_all"));
	m_ui->fromLabel->setText(utils::fromStd(from.full()));
	m_ui->fileNameLabel->setText(utils::fromStd(name));
	m_ui->fileSizeLabel->setText(QString::number(size));
	m_decline_on_delete = true;
	m_ft = ft;
	m_from = from;
	m_sid = sid;
	m_stypes = stypes;
	m_ft_widget = ft_widget;
	m_jft = jft;
}

jFileTransferRequest::~jFileTransferRequest()
{
	if(m_decline_on_delete)
	{
		m_ft->declineFT(m_from, m_sid, SIManager::RequestRejected);
		m_jft->removeWidget(m_from, m_sid);
	}
	delete m_ui;
}

void jFileTransferRequest::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void jFileTransferRequest::on_declineButton_clicked()
{
	close();
}

void jFileTransferRequest::on_acceptButton_clicked()
{
	m_ft_widget->setFilePath(QFileDialog::getSaveFileName(this, tr("Save File"), m_ui->fileNameLabel->text()));
	m_ft_widget->show();
	m_decline_on_delete = false;
	if(m_stypes&SIProfileFT::FTTypeS5B)
		m_ft->acceptFT(m_from, m_sid, SIProfileFT::FTTypeS5B);
	else if(m_stypes&SIProfileFT::FTTypeIBB)
		m_ft->acceptFT(m_from, m_sid, SIProfileFT::FTTypeIBB);
	else
		m_ft->declineFT(m_from, m_sid, SIManager::NoValidStreams);
	close();
}
