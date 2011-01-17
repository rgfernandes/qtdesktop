/*
jSearch.h

Copyright (c) 2008 by Kostin Dmitrij <kostindima@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JSEARCH_H
#define JSEARCH_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QMovie>

#include <gloox/search.h>
#include <gloox/searchhandler.h>
#include <gloox/client.h>
#include <gloox/dataform.h>
#include <gloox/dataformitem.h>
#include <gloox/dataformreported.h>

#include "jAccount.h"
#include "jDataForm.h"
#include "jServiceDiscovery.h"
#include "jPluginSystem.h"

#include "ui_jSearch.h"

using namespace gloox;

class jSearch : public QWidget, SearchHandler
{
	Q_OBJECT

	public:
		jSearch(jAccount *account, QString jid = "", QWidget *parent = 0);
		~jSearch();

	private slots:
		void setSearchService(jDiscoItem *disco_item);
		void on_fetchButton_clicked();
		void on_clearButton_clicked();
		void on_searchButton_clicked();
		void treeDoubleClicked ( QTreeWidgetItem * item, int column );

	signals:
		// void handleError(const Error *error);
		void addContact(const QString &jid, const QString &nick);

	private:
		void handleSearchFields (const JID &directory, int fields, const std::string &instructions);
		void handleSearchFields (const JID &directory, const DataForm *form);
		void handleSearchResult (const JID &directory, const SearchResultList &resultList);
		void handleSearchResult (const JID &directory, const DataForm *form);
		void handleSearchError (const JID &directory, const Error *error);
		void getSearchService();
		void prepareFetch();
		void fetch();

		//void newLineEdit(QString name, QString label);
		jAccount *m_jabber_account;
		Search *m_search;

		//QLabel *information;
		//QGridLayout *gridLayout;
		jDataForm* d_form;
		QString dir;
		//QTreeWidget *resultsTree;
		QStringList headerList;

		DataForm *forma;
		int wfields;
		bool haveDataform;
		//QPushButton* pushSearch;
		/*QList<QLineEdit *> line_edits;
		QList<QLabel *> labels;
		QList<QHBoxLayout *> laouts;*/
		QLineEdit *firstLbl, *lastLbl, *nickLbl, *emailLbl;
		Ui::Search ui;
		QMovie *movie;

};

#endif // JSEARCH_H
