/*****************************************************************************
    SearchResultsWidget

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

#ifndef SEARCHRESULTSWIDGET_H
#define SEARCHRESULTSWIDGET_H

#include <QWidget>
#include "ui_searchresultswidget.h"
#include "../coresrc/mrimdefs.h"

class MRIMClient;

class ContactWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    ContactWidgetItem(QString aEmail, bool aShowAvatar, QTreeWidget* aParent);
    ~ContactWidgetItem();
private slots:
    void HandleSmallAvatarFetched(QString aEmail);
    void SetAvatar();
private:
    QString m_email;    
};

class SearchResultsWidget : public QWidget
{
	Q_OBJECT

public:
        SearchResultsWidget(MRIMClient* aClient,QWidget *parent = 0);
	~SearchResultsWidget();
        void show(QList<MRIMSearchParams*> aFoundList, bool aShowAvatars);
public slots:	
	void Reset();
        void AddContacts(QList<MRIMSearchParams*> aFoundContacts, bool aShowAvatars);
private:
	Ui::SearchResultsWidgetClass ui;
        MRIMClient* m_client;
private slots:
void on_contactsTreeWidget_itemClicked(QTreeWidgetItem* item, int column);
        void on_addCntButton_clicked();
};

#endif // SEARCHRESULTSWIDGET_H
