/*
    fileRequestWindow

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#include "filerequestwindow.h"
#include <qutim/iconmanagerinterface.h>

fileRequestWindow::fileRequestWindow(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	setFixedSize(size());
        qutim_sdk_0_2::SystemsCity::PluginSystem()->centerizeWidget(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	gettedFileSize = 0;
	setWindowIcon(qutim_sdk_0_2::Icon("save_all"));
	ui.fileLabel->setPixmap(qutim_sdk_0_2::Icon("filerequest").pixmap(128));
	ui.acceptButton->setIcon(qutim_sdk_0_2::Icon("apply"));
	ui.declineButton->setIcon(qutim_sdk_0_2::Icon("cancel"));
}

fileRequestWindow::~fileRequestWindow()
{

}

void fileRequestWindow::setSengingData(const QString &name, const QString &fileName, 
		quint32 ip, quint32 fileSize, quint16 port)
{
	peerip = ip;
	peerport = port;
	getFileName = fileName;
	gettedFileSize = fileSize;
	ui.fromLabel->setText(name);
	ui.fileNameLabel->setText(fileName);
	
	QHostAddress addr(ip);
	
	ui.ipLabel->setText(addr.toString());
	ui.fileSizeLabel->setText(getFileSize(fileSize));
	
}

QString fileRequestWindow::getFileSize(quint32 size) const
{
	quint16 bytes = size % 1024;
	quint16 kBytes = size % (1024 * 1024) / 1024;
	quint16 mBytes = size % (1024 * 1024 * 1024) / (1024 * 1024);
	quint16 gBytes = size / (1024 * 1024 * 1024);
	
	QString fileSize;
	
	if ( gBytes )
		fileSize.append(QString::number(gBytes)+",");
	if ( gBytes || mBytes )
		fileSize.append(QString::number(mBytes)+",");
	if ( gBytes || mBytes || kBytes )
		fileSize.append(QString::number(kBytes)+",");
	if ( gBytes || mBytes || kBytes || bytes )
		fileSize.append(QString::number(bytes));
	
	return fileSize;
}

void fileRequestWindow::on_declineButton_clicked()
{
	emit cancelSending(cookie, uin);
	close();
}

void fileRequestWindow::on_acceptButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                    QDir::homePath() + "/" + getFileName,
	                            tr("All files (*)"));
	if ( !fileName.isEmpty() )
	{
                  //TODO make nice dialog for whis
//                QFileInfo fi = QFileInfo(fileName);
//                if (fi.exists()) {
//                    qutim_sdk_0_2::TreeModelItem item;
//                    qutim_sdk_0_2::SystemsCity::PluginSystem()->customNotification(item,tr("File %1 is exist").arg(fileName));
//                    return;
//                }
		emit fileAccepted(cookie, uin, fileName, peerip, peerport, gettedFileSize);
		close();
	}
}

