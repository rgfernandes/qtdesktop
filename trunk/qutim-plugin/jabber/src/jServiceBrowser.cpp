/*
Copyright (c) 2009 by Denis Daschenko <daschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "jServiceBrowser.h"
#include "jProtocol.h"
#include "jAccount.h"
#include "utils.h"

jServiceBrowser::jServiceBrowser(const QString &target, jAccount *jabber_account, bool autoclose, QWidget *parent) : QWidget(parent)
{
	m_jabber_account = jabber_account;
	m_target = target;
	m_autoclose = autoclose;
	m_key = 0;
	ui.setupUi(this);
	if (m_target == "conference")
		this->setWindowTitle(QApplication::translate("SearchConference", "Search conference", 0, QApplication::UnicodeUTF8));
	else if (m_target == "gateway")
		this->setWindowTitle(QApplication::translate("SearchTransport", "Search transport", 0, QApplication::UnicodeUTF8));
	else
		this->setWindowTitle(QApplication::translate("SearchService", "Search service", 0, QApplication::UnicodeUTF8));
	ui.serviceServer->installEventFilter(this);
	ui.searchButton->setIcon(m_jabber_account->getPluginSystem().getIcon("search"));
	ui.closeButton->setIcon(m_jabber_account->getPluginSystem().getIcon("cancel"));
	ui.joinButton->setIcon(m_jabber_account->getPluginSystem().getIcon("conference"));
	ui.registerButton->setIcon(m_jabber_account->getPluginSystem().getIcon("servicereg"));
	ui.searchFormButton->setIcon(m_jabber_account->getPluginSystem().getIcon("finduser"));
	ui.executeButton->setIcon(m_jabber_account->getPluginSystem().getIcon("command"));
	ui.addRosterButton->setIcon(m_jabber_account->getPluginSystem().getIcon("add_user"));
	ui.showVCardButton->setIcon(m_jabber_account->getPluginSystem().getIcon("contactinfo"));
	ui.addProxyButton->setIcon(m_jabber_account->getPluginSystem().getIcon(""));
	ui.filterButton->setIcon(m_jabber_account->getPluginSystem().getIcon("filter"));
	ui.filterButton->setCheckable(true);
	ui.filterLine->setVisible(false);
	connect(ui.serviceTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(showControls(QTreeWidgetItem*, int)));
	//connect(ui.serviceTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(showControls(QTreeWidgetItem*, int)));
	connect(ui.serviceTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(getChildItems(QTreeWidgetItem*)));
	connect(ui.filterLine, SIGNAL(textEdited(const QString&)), this, SLOT(filterItem(const QString&)));
	connect(ui.filterButton, SIGNAL(clicked(bool)), this, SLOT(showFilterLine(bool)));
	QString server = utils::fromStd(JID(utils::toStd(m_jabber_account->getAccountName())).server());
	ui.serviceServer->addItem(server);
	on_searchButton_clicked();
	this->resize(700, 600);
	ui.serviceTree->setColumnWidth(0, ui.serviceTree->width()*9/10);
}

jServiceBrowser::~jServiceBrowser()
{
}

void jServiceBrowser::closeEvent(QCloseEvent*)
{
	emit finishSearch();
}

bool jServiceBrowser::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			on_searchButton_clicked();
		return QObject::eventFilter(obj, event);
	} else {
		return QObject::eventFilter(obj, event);
	}
}

void jServiceBrowser::setItemInfo(jDiscoItem *disco_item)
{
	QString key;
	if (disco_item->key().isEmpty())
		key = disco_item->jid()+":"+disco_item->node();
	else
		key = disco_item->key();
	QTreeWidgetItem *item = m_item_list.value(key);
	if (!item)
		return;
	if (disco_item->empty())
	{
		item->setDisabled(true);
		if (disco_item->name().isEmpty())
			item->setText(0, disco_item->jid());
		else
			item->setText(0, disco_item->name());
		item->setText(1, disco_item->jid());
		QString tooltip;
		tooltip = disco_item->error();
		item->setToolTip(0, tooltip);
	}
	else
	{
		item->setDisabled(false);
		if (!disco_item->name().isEmpty())
			item->setText(0, disco_item->name());
		item->setText(1, disco_item->jid());
		item->setIcon(0, Icon(setServiceIcon(disco_item), "png"));
		QString tooltip;
		tooltip = "<b>"+disco_item->name()+"</b> ("+disco_item->jid()+")<br/>";
		tooltip += tr("<br/><b>Identities:</b><br/>");
		foreach(jDiscoItem::jDiscoIdentity identity, disco_item->identities())
		{
			jDiscoItem *di = new jDiscoItem();
			di->setJID(disco_item->jid());
			di->addIdentity(identity);
			QString img = m_jabber_account->getPluginSystem().getIconFileName(setServiceIcon(disco_item));
			delete di;
			tooltip += "<img src='"+img+"'> "+identity.name+" ("+tr("category: ")+identity.category+", "+tr("type: ")+identity.type+")<br/>";
		}
		if (!disco_item->features().isEmpty())
		{
			tooltip += tr("<br/><b>Features:</b><br/>");
			foreach(QString feature, disco_item->features())
				tooltip += feature+"<br/>";
		}
		item->setToolTip(0, tooltip);
		if (disco_item->expand())
			item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	}
	item->setData(0, Qt::UserRole+1, reinterpret_cast<qptrdiff>(disco_item));
	if (ui.filterButton->isChecked())
	{
		QTreeWidgetItem *parent = item->parent();
		if (parent && parent->isHidden())
		{
			if (item->text(0).contains(ui.filterLine->text(), Qt::CaseInsensitive) || item->text(1).contains(ui.filterLine->text(), Qt::CaseInsensitive))
			{
				QList<QTreeWidgetItem*> items;
				items << item;
				setBranchVisible(items);
			}
			else
				item->setHidden(true);
		}
	}
}

void jServiceBrowser::setItemChild(const QList<jDiscoItem *>& disco_list, const QString &key)
{
	QTreeWidgetItem *parent_item = m_item_list.value(key);
	if (!parent_item || parent_item->childCount())
		return;
	if (disco_list.isEmpty())
		parent_item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
	foreach (jDiscoItem *disco_item, disco_list)
		setItemChild(parent_item, disco_item, key);
}

void jServiceBrowser::setItemChild(QTreeWidgetItem *parent_item, jDiscoItem *disco_item, const QString &key)
{
	QTreeWidgetItem *item = new QTreeWidgetItem(parent_item);
	disco_item->setKey(disco_item->jid()+":"+disco_item->node());
	m_item_list.insert(disco_item->key(), item);
	setItemInfo(disco_item);
	parent_item->setExpanded(true);
	item->setExpanded(false);
}

QString jServiceBrowser::setServiceIcon(jDiscoItem *disco_item)
{
	if (disco_item->identities().isEmpty())
		return "";
	QString service_icon;
	if (disco_item->hasIdentity("server"))
		service_icon = "server";
	else if (disco_item->hasIdentity("conference", "text"))
	{
		if (utils::fromStd(JID(utils::toStd(disco_item->jid())).username()).isEmpty())
			service_icon = "conferenceserver";
		else if (utils::fromStd(JID(utils::toStd(disco_item->jid())).resource()).isEmpty())
			service_icon = "conference";
		else
			service_icon = "conferenceuser";
	}
	else if (disco_item->hasIdentity("conference", "irc"))
		service_icon = "irc";
	else if (disco_item->hasIdentity("gateway", "icq"))
		service_icon = "icq_tr";
	else if (disco_item->hasIdentity("gateway", "aim"))
		service_icon = "aim_tr";
	else if (disco_item->hasIdentity("gateway", "mrim"))
		service_icon = "mrim_tr";
	else if (disco_item->hasIdentity("gateway", "msn"))
		service_icon = "msn_tr";
	else if (disco_item->hasIdentity("gateway", "xmpp"))
		service_icon = "jabber_tr";
	else if (disco_item->hasIdentity("gateway"))
		service_icon = "default_tr";
	else if (disco_item->hasIdentity("directory"))
		service_icon = "finduser";
	else if (disco_item->hasIdentity("automation"))
		service_icon = "command";
	else
		service_icon = "defaultservice";
	return service_icon;
}

void jServiceBrowser::on_searchButton_clicked()
{
	hideControls();
	ui.serviceTree->clear();
	m_item_list.clear();
	QTreeWidgetItem *root_item = new QTreeWidgetItem();
	root_item->setText(0, "");
	root_item->setText(1, ui.serviceServer->currentText());
	jDiscoItem *di = new jDiscoItem();
	di->setJID(ui.serviceServer->currentText());
	root_item->setData(0, Qt::UserRole+1, reinterpret_cast<qptrdiff>(di));
	ui.serviceTree->addTopLevelItem(root_item);
	getChildItems(root_item);
}

void jServiceBrowser::getChildItems(QTreeWidgetItem *item) //expanded
{
	jDiscoItem *di;
	di = reinterpret_cast<jDiscoItem*>(item->data(0, Qt::UserRole+1).value<qptrdiff>());
	if (!item->childCount() && (di->expand() || di->identities().isEmpty()))
	{
		if (di->key().isEmpty())
			di->setKey(QString(m_key++));
		m_item_list.insert(di->key(), item);
		jServiceDiscovery *handler = new jServiceDiscovery(m_target);
		connect(handler, SIGNAL(getDiscoItem(const QString&, const QString&, DiscoHandler*)), m_jabber_account, SLOT(getDiscoItem(const QString&, const QString&, DiscoHandler*)));
		connect(handler, SIGNAL(getDiscoInfo(const QString&, const QString&, DiscoHandler*)), m_jabber_account, SLOT(getDiscoInfo(const QString&, const QString&, DiscoHandler*)));
		connect(handler, SIGNAL(finishSelfSearch(jDiscoItem*)), this, SLOT(setItemInfo(jDiscoItem*)));
		//connect(handler, SIGNAL(finishSearch(jDiscoItem*, int)), this, SLOT(setItemChild(const QString&, jDiscoItem*, int)));
		connect(handler, SIGNAL(finishSearch(const QList<jDiscoItem*>&, const QString&)), this, SLOT(setItemChild(const QList<jDiscoItem*>&, const QString&)));
		handler->search(di);
	}
}

void jServiceBrowser::showControls(QTreeWidgetItem *item, int)
{
	hideControls();
	jDiscoItem *di;
	di = reinterpret_cast<jDiscoItem*>(item->data(0, Qt::UserRole+1).value<qptrdiff>());
	foreach (jDiscoItem::jDiscoAction action, di->actions())
	{
		switch (action)
		{
			case jDiscoItem::JOIN:
				ui.joinButton->setEnabled(true);
				break;
			case jDiscoItem::REGISTER:
				ui.registerButton->setEnabled(true);
				break;
			case jDiscoItem::SEARCH:
				ui.searchFormButton->setEnabled(true);
				break;
			case jDiscoItem::EXEC:
				ui.executeButton->setEnabled(true);
				break;
			case jDiscoItem::ADD:
				ui.addRosterButton->setEnabled(true);
				break;
			case jDiscoItem::VCARD:
				ui.showVCardButton->setEnabled(true);
				break;
			case jDiscoItem::PROXY:
				ui.addProxyButton->setEnabled(true);
				break;
		}
	}
}

void jServiceBrowser::hideControls()
{
	ui.joinButton->setEnabled(false);
	ui.registerButton->setEnabled(false);
	ui.searchFormButton->setEnabled(false);
	ui.executeButton->setEnabled(false);
	ui.addRosterButton->setEnabled(false);
	ui.showVCardButton->setEnabled(false);
	ui.addProxyButton->setEnabled(false);
}

void jServiceBrowser::on_joinButton_clicked()
{
	QTreeWidgetItem *item = ui.serviceTree->currentItem();
	emit joinConference(item->text(1));
	if (m_autoclose)
		close();
}

void jServiceBrowser::on_registerButton_clicked()
{
	QTreeWidgetItem *item = ui.serviceTree->currentItem();
	emit registerTransport(item->text(1));
}

void jServiceBrowser::on_searchFormButton_clicked()
{
	QTreeWidgetItem *item = ui.serviceTree->currentItem();
	jDiscoItem *di;
	di = reinterpret_cast<jDiscoItem*>(item->data(0, Qt::UserRole+1).value<qptrdiff>());
	/*QStringList searches;
	foreach (jDiscoItem::jDiscoIdentity identity, di->identities())
		if (identity.category == "directory")
			searches << identity.type*/
	emit searchService("", item->text(1));
}

void jServiceBrowser::on_executeButton_clicked()
{
	QTreeWidgetItem *item = ui.serviceTree->currentItem();
	jDiscoItem *di;
	di = reinterpret_cast<jDiscoItem*>(item->data(0, Qt::UserRole+1).value<qptrdiff>());
	emit executeCommand(item->text(1), di->node());
}

void jServiceBrowser::on_addRosterButton_clicked()
{
	QTreeWidgetItem *item = ui.serviceTree->currentItem();
	emit addContact(item->text(1), item->text(0));
}

void jServiceBrowser::on_showVCardButton_clicked()
{
	QTreeWidgetItem *item = ui.serviceTree->currentItem();
	jDiscoItem *di;
	di = reinterpret_cast<jDiscoItem*>(item->data(0, Qt::UserRole+1).value<qptrdiff>());
	emit showVCard(item->text(1));
}

void jServiceBrowser::on_addProxyButton_clicked()
{
	QTreeWidgetItem *item = ui.serviceTree->currentItem();
	emit addProxy(JID(utils::toStd(item->text(1))));
}

void jServiceBrowser::on_closeButton_clicked()
{
	close();
}

void jServiceBrowser::filterItem(const QString &mask)
{
	setItemVisible(ui.serviceTree->invisibleRootItem(), true);
	QList<QTreeWidgetItem*> visibleItems;
	visibleItems = findItems(ui.serviceTree->invisibleRootItem(), ui.filterLine->text());
	setBranchVisible(visibleItems);
}

void jServiceBrowser::setBranchVisible(QList<QTreeWidgetItem*> items)
{
	int count = items.count();
	int num = 0;
	while (num < count)
	{
		QTreeWidgetItem *item = items[num]->parent();
		if (item && !items.contains(item))
		{
			items.append(item);
			count++;
		}
		num++;
	}
	foreach(QTreeWidgetItem* item, items)
		item->setHidden(false);
}

QList<QTreeWidgetItem*> jServiceBrowser::findItems(QTreeWidgetItem *item, const QString &text)
{
	QList<QTreeWidgetItem*> list;
	int count = item->childCount();
	for (int pos = 0; pos < count; pos++)
	{
		if (text == "" || item->child(pos)->text(0).contains(text, Qt::CaseInsensitive) || item->child(pos)->text(1).contains(text, Qt::CaseInsensitive))
		{
			list << item->child(pos);
			list << findItems(item->child(pos), "");
		}
		else if (item->child(pos)->childCount())
			list << findItems(item->child(pos), text);
	}
	return list;
}

void jServiceBrowser::setItemVisible(QTreeWidgetItem *item, bool visibility)
{
item->setHidden(visibility);
int count = item->childCount();
for (int pos = 0; pos < count; pos++)
setItemVisible(item->child(pos), visibility);
}

void jServiceBrowser::showFilterLine(bool show)
{
	ui.filterLine->setText("");
	filterItem("");
	ui.filterLine->setVisible(show);
}
