/*
    listChannel

		Copyright (c) 2008-2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#include "listchannel.h"
//
listChannel::listChannel( QWidget * parent, Qt::WFlags f) 
	: QWidget(parent, f)
{
	setupUi(this);

	QDesktopWidget &desktop = *QApplication::desktop();
	move(desktop.width()/2 - width()/2, desktop.height()/2 - height()/2);

	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);

	imgLoad->hide();
	imgmovie = new QMovie(":/icons/irc-loading.gif");
	imgLoad->setMovie(imgmovie);
}

void listChannel::on_btnRequest_clicked()
{
	labelStatus->setText(tr("Sending channels list request..."));
	emit requestList();
}

void listChannel::addRow(QString channel, QString users, QString topic) {
//	qDebug()<<"IRC"<<"chanlist"<<channel<<ccount;
	if (channel=="%START%") {
		imgLoad->show(); imgmovie->start();
		ccount=0;
		table->clear();
		QString headers = "Channel,Users,Topic";
		table->setHeaderLabels(headers.split(","));
		labelStatus->setText(tr("Fetching channels list... %1").arg(QString::number(ccount)));
		btnRequest->setEnabled(false);
	}
	else if (channel=="%END%") {
		table->sortByColumn(0, Qt::AscendingOrder);
		btnRequest->setEnabled(true);
		labelStatus->setText(tr("Channels list loaded. (%1)").arg(ccount));
		imgLoad->hide(); imgmovie->stop();
	}
	else {
		QRegExp rxfilter(editFilter->text());
		if (rxfilter.indexIn(channel)!=-1 || rxfilter.indexIn(topic)!=-1) {
			ccount++;
			labelStatus->setText(tr("Fetching channels list... (%1)").arg(ccount));
			QTreeWidgetItem *item = new QTreeWidgetItem();
			item->setText(0, channel);
			item->setText(1, users);
			item->setText(2, topic);
			table->addTopLevelItem(item);
		}
	}
}

void listChannel::on_table_itemDoubleClicked(QTreeWidgetItem* item, int /*column*/)
{
	emit joinChannel(item->text(0));
}
