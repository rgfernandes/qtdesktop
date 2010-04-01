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


#ifndef LISTCHANNEL_H
#define LISTCHANNEL_H
//
#include <QWidget>
#include <QMovie>
#include <QDesktopWidget>
#include <QDebug>
#include "ui_listchannel.h"
//
class listChannel : public QWidget, public Ui::listChannelClass
{
Q_OBJECT
public:
	listChannel( QWidget * parent = 0, Qt::WFlags f = 0 );

private:
	int ccount;
	QMovie *imgmovie;

	
signals:
	void requestList();
	void joinChannel(QString);

private slots:
	void on_table_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void on_btnRequest_clicked();
	void addRow(QString, QString, QString);

};
#endif





