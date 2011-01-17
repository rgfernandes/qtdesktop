/*****************************************************************************
    MRIMSearchWidget

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

#ifndef MRIMSEARCHWIDGET_H
#define MRIMSEARCHWIDGET_H

#include <QWidget>
#include "ui_mrimsearchwidget.h"
#include "../coresrc/mrimdefs.h"

class RegionListParser;
struct LiveRegion; 
class MRIMClient;

class MRIMSearchWidget : public QWidget
{
    Q_OBJECT
public:
        MRIMSearchWidget(MRIMClient* aClient, QWidget *parent = 0);
	~MRIMSearchWidget();
        bool ShowAvatars();
public slots:
        void SearchFinished(quint32 aFoundCnts);
signals:
	void StartSearch(MRIMSearchParams aParams);
private:
	Ui::MRIMSearchWidgetClass ui;
	RegionListParser* m_listParser;
	const QList<LiveRegion>* m_regionsList;
        MRIMClient* m_client;
private slots:
	void on_pushButton_clicked();
	void on_countryComboBox_currentIndexChanged(int);
	void on_emailEdit_textChanged(QString);
	void on_groupBox_toggled(bool);
};

#endif // MRIMSEARCHWIDGET_H
