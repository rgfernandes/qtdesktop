/*
jSearch.cpp

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

#include "jSearch.h"
#include "utils.h"
#include "jProtocol.h"
#include <gloox/error.h>

#include <QMessageBox>

//TODO:   Test search service with legacy forms

jSearch::jSearch(jAccount *account, QString jid, QWidget *parent) : QWidget(parent), m_jabber_account(account)
{
	ui.setupUi(this);
	dir = jid;
	m_search = 0;
	d_form = 0;
	if (!dir.isEmpty())
	{
		ui.serverContainer->setVisible(false);
		ui.searchServer->setEditText(dir);
		prepareFetch();
		fetch();
	}
	setWindowTitle(tr("Search"));
	this->setObjectName("this");
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	//ui.fetchButton->setIcon();
	//ui.clearButton->setIcon();
	ui.searchButton->setIcon(Icon("search"));
	ui.closeButton->setIcon(Icon("cancel"));
	haveDataform = false;
	ui.resultsTree->setIndentation(0);
	ui.resultsTree->setItemsExpandable(false);
	ui.resultsTree->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.resultsTree->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.resultsTree->setAlternatingRowColors(true);
	ui.resultsTree->setHeaderHidden(true);
	ui.resultsTree->setColumnCount(0);
	connect(ui.resultsTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int)),SLOT(treeDoubleClicked(QTreeWidgetItem *, int)));

	//information = new QLabel("Receiving form data...", this);
	//QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	//information->setSizePolicy(policy);
	//information->setObjectName("information");

	//gridLayout = new QGridLayout(this);
	//gridLayout->addWidget(information,0,0);

	/*	QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QHBoxLayout* tmplaout = new QHBoxLayout(this);
	pushSearch = new QPushButton("Search", this);
	QPushButton* pushCancel = new QPushButton("Cancel", this);
	tmplaout->addItem(horizontalSpacer);
	tmplaout->addWidget(pushSearch);
	tmplaout->addWidget(pushCancel);
	gridLayout->addLayout(tmplaout, 31, 0, Qt::AlignBottom);
	connect(pushCancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(pushSearch, SIGNAL(clicked()), this, SLOT(searchClicked()));
	pushSearch->setVisible(false);
	this->resize(700, 600);

	*/
	/*resultsTree = new QTreeWidget(this);

	gridLayout->addWidget(resultsTree, 30, 0, Qt::AlignBottom);
	*/
}

jSearch::~jSearch()
{
	delete(m_search);
	destroy(true, true);
}

void jSearch::handleSearchFields (const JID &directory, int fields, const std::string &instructions)
{
	/*ui.informationLabel->setText(utils::fromStd(instructions));
	ui.resultsTree->setHeaderHidden(false);
	QTreeWidgetItem *header_item = resultsTree->headerItem();

	QHBoxLayout *tmplay = new QHBoxLayout(this);
	QLabel *tmlbl = new QLabel(this);
	firstLbl = new QLineEdit(this);
	tmlbl->setText(tr("First"));
	tmplay->addWidget(tmlbl, 0);
	tmplay->addWidget(firstLbl, 1);
	gridLayout->addLayout(tmplay, 1, 0);

	tmplay = new QHBoxLayout(this);
	tmlbl = new QLabel(this);
	lastLbl = new QLineEdit(this);
	tmlbl->setText(tr("Last"));
	tmplay->addWidget(tmlbl, 0);
	tmplay->addWidget(lastLbl, 1);
	gridLayout->addLayout(tmplay, 2, 0);

	tmplay = new QHBoxLayout(this);
	tmlbl = new QLabel(this);
	nickLbl = new QLineEdit(this);
	tmlbl->setText(tr("Nick"));
	tmplay->addWidget(tmlbl, 0);
	tmplay->addWidget(nickLbl, 1);
	gridLayout->addLayout(tmplay, 3, 0);

	tmplay = new QHBoxLayout(this);
	tmlbl = new QLabel(this);
	emailLbl = new QLineEdit(this);
	tmlbl->setText(tr("E-Mail"));
	tmplay->addWidget(tmlbl, 0);
	tmplay->addWidget(emailLbl, 1);
	gridLayout->addLayout(tmplay, 4, 0);

	header_item->setText(0, tr("First"));
	header_item->setText(1, tr("Last"));
	header_item->setText(2, tr("Nick"));
	header_item->setText(3, tr("E-Mail"));
*/
}

void jSearch::handleSearchFields (const JID &directory, const DataForm *form)
{
	ui.fetchButton->setEnabled(true);
	ui.informationLabel->setText(utils::fromStd(form->instructions().back()));
	haveDataform = true;
	d_form = new jDataForm(form, this);
	ui.formLayout->insertWidget(1, d_form);
	ui.fetchButton->setEnabled(true);
	ui.searchButton->setEnabled(true);
}

void jSearch::handleSearchResult (const JID &directory, const SearchResultList &resultList)
{
	/*
	resultsTree->clear();
	SearchResultList srcres = SearchResultList(resultList);
	SearchResultList::iterator it;
	for ( it=srcres.begin() ; it != srcres.end(); it++ ) {
	QTreeWidgetItem *itm = new QTreeWidgetItem(resultsTree);
	itm->setText(headerList.indexOf("First"), utils::fromStd((*it)->first()));
	itm->setText(headerList.indexOf("Last"), utils::fromStd((*it)->last()));
	itm->setText(headerList.indexOf("Nick"), utils::fromStd((*it)->nick()));
	itm->setText(headerList.indexOf("E-Mail"), utils::fromStd((*it)->email()));
	}

	for(int i=0; i < resultsTree->columnCount();i++){
	resultsTree->resizeColumnToContents(i);
	}
	pushSearch->setEnabled(true);
*/
}

void jSearch::handleSearchResult (const JID &directory, const DataForm *form)
{
	ui.searchButton->setEnabled(true);
	ui.resultsTree->clear();
	ui.resultsTree->setHeaderHidden(false);
	haveDataform = true;
	//if(ui.resultsTree->columnCount()==0){
	delete ui.resultsTree->headerItem(); // Generating header
	QTreeWidgetItem *header_item = new QTreeWidgetItem();
	foreach(DataFormField *field, form->reported()->fields())
	{
		header_item->setText(headerList.count(), utils::fromStd(field->label()));
		headerList.append(utils::fromStd(field->name()));
	}
	ui.resultsTree->setHeaderItem(header_item);
	//}
	DataForm::ItemList itemlst = form->items(); //Generating table
	DataForm::ItemList::iterator itemit;
	for ( itemit=itemlst.begin() ; itemit != itemlst.end(); itemit++ )
	{
		DataFormFieldContainer::FieldList fldlst = (*itemit)->fields();
		DataFormFieldContainer::FieldList::iterator fldit;
		QTreeWidgetItem *itm = new QTreeWidgetItem(ui.resultsTree);
		for ( fldit = fldlst.begin() ; fldit != fldlst.end(); fldit++ )
		{
			itm->setText(headerList.indexOf( utils::fromStd((*fldit)->name())), utils::fromStd((*fldit)->value()));
		}
	}
	for(int i = 0; i < ui.resultsTree->columnCount(); i++)
	{
		ui.resultsTree->resizeColumnToContents(i);
	}
}

void jSearch::handleSearchError (const JID &directory, const Error *error)
{
	/*TODO: can't find error, for ex, for ICQ-transport, with XML like
	js say, what this is legacy format, and gloox doesn't support this
		*/
	/*
<iq from='jit.jabbus.org' to='dstr@jabbus.org/qutIM' xml:lang='en' id='uid110' type='error'>
<query xmlns='jabber:iq:search'/>
<error code='405'>Not Allowed</error>
		*/
	if(error->type() != StanzaErrorTypeUndefined)
	{
		QMessageBox::critical(this, tr("Error"), utils::fromStd(error->text()), QMessageBox::Ok);
		//emit handleError(error);
		close();
	}
}

void jSearch::getSearchService()
{
	jServiceDiscovery *handler = new jServiceDiscovery("usersearch");
	connect(handler, SIGNAL(getDiscoItem(const QString&, const QString&, DiscoHandler*)), m_jabber_account, SLOT(getDiscoItem(const QString&, const QString&, DiscoHandler*)));
	connect(handler, SIGNAL(getDiscoInfo(const QString&, const QString&, DiscoHandler*)), m_jabber_account, SLOT(getDiscoInfo(const QString&, const QString&, DiscoHandler*)));
	connect(handler, SIGNAL(finishSelfSearch(jDiscoItem*)), this, SLOT(setSearchService(jDiscoItem*)));
	jDiscoItem *di = new jDiscoItem();
	di->setJID(ui.searchServer->currentText());
	handler->search(di);
}

void jSearch::prepareFetch()
{
	movie = new QMovie(ui.informationLabel);
	movie->setFileName(SystemsCity::IconManager()->getIconPath("loading"));
	movie->start();
	ui.informationLabel->setText("");
	ui.informationLabel->setMovie(movie);
}


void jSearch::fetch()
{
	m_search = new Search(m_jabber_account->getProtocol()->getClient());
	m_search->fetchSearchFields(JID(utils::toStd(dir)), this);
	ui.fetchButton->setEnabled(false);
}

void jSearch::setSearchService(jDiscoItem *disco_item)
{
	if (!dir.isEmpty())
	{
		delete disco_item;
		return;
	}
	if (disco_item->hasIdentity("directory", "user"))
	{
		dir = disco_item->jid();
		fetch();
	}
	else if (!disco_item->error().isEmpty())
	{
		if ( disco_item->jid() == ui.searchServer->currentText())
			ui.informationLabel->setText(disco_item->error());
	}
	delete disco_item;
}

void jSearch::treeDoubleClicked ( QTreeWidgetItem * item, int column )
{
	if(item)
	{
		if(haveDataform)
		{
			QTreeWidgetItem *header_item = ui.resultsTree->headerItem();
			QString jid, nick;
			for(int i = 0; i < ui.resultsTree->columnCount(); i++)
			{
				if(header_item->text(i)==tr("Jabber ID") || header_item->text(i)==tr("JID") ) jid = item->text(i);
				if(header_item->text(i)==tr("Nickname")) nick = item->text(i);
			}
			if(!jid.isEmpty())
				emit addContact(jid, nick);
		}
		else {	//legacy form
			emit addContact(item->text(3), item->text(2));
		}
	}
}

void jSearch::on_fetchButton_clicked()
{
	if (m_search)
		delete m_search;
	if (d_form)
		delete d_form;
	dir = "";
	prepareFetch();
	getSearchService();
}

void jSearch::on_clearButton_clicked()
{
	ui.resultsTree->clear();
}

void jSearch::on_searchButton_clicked()
{
	ui.searchButton->setEnabled(false);
	if(haveDataform)
	{
		m_search->search(JID(utils::toStd(dir)), d_form->getDataForm(), this);
	}
	else //legacy form
	{
		m_search->search(JID(utils::toStd(dir)), wfields, SearchFieldStruct(utils::toStd(firstLbl->text()), utils::toStd(lastLbl->text()), utils::toStd(nickLbl->text()), utils::toStd(emailLbl->text())), this);
	}
}
