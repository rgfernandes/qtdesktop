/*
    Сopyright (c) 2009 by Denis Daschenko <daschenko@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JSERVICEBROWSER_H
#define JSERVICEBROWSER_H

#include "ui_jServiceBrowser.h"
#include <gloox/jid.h>
#include "jDiscoItem.h"
//#include "jAdhoc.h"

//class jAdhoc;
class jAccount;

using namespace gloox;

class jServiceBrowser : public QWidget
{
	Q_OBJECT

	public:
		jServiceBrowser(const QString &target, jAccount *jabber_account, bool autoclose = false, QWidget *parent = 0);
		~jServiceBrowser();

	protected:
		bool eventFilter(QObject *obj, QEvent *event);

	private slots:
		void setItemInfo(jDiscoItem *disco_item);
		void setItemChild(const QList<jDiscoItem *>& disco_list, const QString &key);
		void getChildItems(QTreeWidgetItem *item);
		void showControls(QTreeWidgetItem*, int);
		void filterItem(const QString &mask);
		void closeEvent(QCloseEvent*);
		void showFilterLine(bool show);
		void on_closeButton_clicked();
		void on_searchButton_clicked();
		void on_joinButton_clicked();
		void on_registerButton_clicked();
		void on_searchFormButton_clicked();
		void on_executeButton_clicked();
		void on_showVCardButton_clicked();
		void on_addProxyButton_clicked();
		void on_addRosterButton_clicked();

	signals:
		void finishSearch();
		void joinConference(const QString &conference);
		void registerTransport(const QString &transport);
		void executeCommand(const QString &command, const QString &node);
		void showVCard(const QString &jid);
		void addProxy(const JID &proxy);
		void addContact(const QString &jid, const QString &nick);
		void searchService(const QString &type, const QString &jid);

	private:
		void setItemChild(QTreeWidgetItem *parent_item, jDiscoItem *disco_item, const QString &key);
		void hideControls();
		void setItemVisible(QTreeWidgetItem *item, bool visibility);
		QList<QTreeWidgetItem*> findItems(QTreeWidgetItem *item, const QString &text);
		void setBranchVisible(QList<QTreeWidgetItem*> items);
		QString setServiceIcon(jDiscoItem *disco_item);
		Ui::ServiceBrowser ui;
		jAccount *m_jabber_account;
		QString m_target;
		QHash<QString, QTreeWidgetItem*> m_item_list;
		bool m_autoclose;
		int m_key;

};

#endif // JSERVICEBROWSER_H
