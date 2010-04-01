/*
Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>,
Denis Daschenko <daschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "jJoinChat.h"
#include "utils.h"

jJoinChat::jJoinChat(jAccount *jabber_account, const QString &room, const QString &nick, const QString &password, bool showBookmark, QWidget *parent) : QDialog(parent), m_jabber_account(jabber_account), m_room(room), m_nick(nick), m_password(password)
{
	ui.setupUi(this);
	ui.bookmarkBox->setVisible(false);
	ui.autoJoinCheckBox->setVisible(false);
	ui.conferenceNameLineEdit->setEnabled(false);
	ui.saveButton->setVisible(false);
	ui.conferenceLineEdit->setCompleter(0);
	ui.conferenceLineEdit->setInsertPolicy(QComboBox::NoInsert);
	ui.exampleLabel->setText("<font color='#808080'>(room@conference.server.url)</font>");
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(ui.conferenceListWidget,SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(showConference(QListWidgetItem*, QListWidgetItem*)));
	connect(ui.conferenceListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(joinBookmark(QListWidgetItem*)));
	connect(ui.conferenceLineEdit, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRecent(int)));
	ui.joinButton->setIcon(m_jabber_account->getPluginSystem().getIcon("apply"));
	ui.searchButton->setIcon(m_jabber_account->getPluginSystem().getIcon("search"));
	ui.saveButton->setIcon(m_jabber_account->getPluginSystem().getIcon("save_all"));
	ui.addConferenceButton->setIcon(m_jabber_account->getPluginSystem().getIcon("add"));
	ui.removeConferenceButton->setIcon(m_jabber_account->getPluginSystem().getIcon("remove"));
	m_settings = "qutim/qutim."+m_jabber_account->getProfileName()+"/jabber."+m_jabber_account->getAccountName();
	ui.conferenceNameLineEdit->installEventFilter(this);
	ui.conferenceLineEdit->installEventFilter(this);
	ui.nickLineEdit->installEventFilter(this);
	ui.passwordLineEdit->installEventFilter(this);
	ui.conferenceListWidget->installEventFilter(this);
	QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, m_settings, "recent");
	ui.conferenceLineEdit->addItem("");
	int size = recent_settings.beginReadArray("conferences");
	for (int i = 0; i < size; ++i) {
		recent_settings.setArrayIndex(i);
		recent_conf << recent_settings.value("name").toString();
		recent_nick << recent_settings.value("nick").toString();
		recent_pass << recent_settings.value("password").toString();
		ui.conferenceLineEdit->addItem(recent_conf[i]+" ("+recent_nick[i]+")");
	}
	recent_settings.endArray();
	ui.conferenceLineEdit->setCurrentIndex(0);
	QSettings m_account_settings(QSettings::defaultFormat(), QSettings::UserScope, m_settings, "accountsettings");
	isLocal = m_account_settings.value("main/localbookmark",false).toBool();
	m_account_settings.beginGroup("confhistory");
	ui.historyCount->setValue(m_account_settings.value("messages", 20).toInt());
	int seconds = m_account_settings.value("seconds", 300).toInt();
	int hours = seconds/3600;
	seconds -= hours*3600;
	int minutes = seconds/60;
	seconds -= minutes*60;
	QTime time = QTime(hours,minutes,seconds);
	ui.historySeconds->setTime(time);
	QDateTime date;
	date.setDate(QDate::currentDate());
	date.setTime(QTime::currentTime().addSecs(-300));
	ui.historySince->setDateTime(date);
	QString history_type = m_account_settings.value("type", "messages").toString();
	m_account_settings.endGroup();
	ui.historySince->setEnabled(false);
	if (history_type == "messages")
	{
		ui.checkMessage->setChecked(true);
		ui.historySeconds->setEnabled(false);
	}
	else if (history_type == "seconds")
	{
		ui.checkSecond->setChecked(true);
		ui.historyCount->setEnabled(false);
	}
	connect(ui.checkMessage, SIGNAL(clicked()), this, SLOT(setHistoryMessage()));
	connect(ui.checkSecond, SIGNAL(clicked()), this, SLOT(setHistorySecond()));
	connect(ui.checkSince, SIGNAL(clicked()), this, SLOT(setHistorySince()));
	if (showBookmark)
		setConferences();
	SystemsCity::PluginSystem()->centerizeWidget(this);
}

jJoinChat::~jJoinChat()
{

}

void jJoinChat::setConferences()
{
	QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, m_settings, "recent");
	recent_settings.beginGroup("main");
	bool isSupportBookmarks = recent_settings.value("available", false).toBool();
	recent_settings.endGroup();
	if (isSupportBookmarks)
	{
		m_c_list = m_jabber_account->getRecentBookmarks();
		fillConferences();
	}
}

void jJoinChat::fillConferences()
{
	ui.bookmarkBox->setVisible(true);
	ui.autoJoinCheckBox->setVisible(true);
	ui.conferenceNameLineEdit->setEnabled(true);
	ui.saveButton->setVisible(true);
	int row = ui.conferenceListWidget->currentRow();
	ui.conferenceListWidget->clear();
	QString empty_name = tr("new chat");
	ui.conferenceListWidget->addItem(empty_name);
	foreach(ConferenceListItem item, m_c_list)
	{
		QString name = utils::fromStd(item.name);
		if(name.isEmpty())
			name = utils::fromStd(item.jid);
		ui.conferenceListWidget->addItem(name);
	}
	if (row == -1 || row > m_c_list.size()+1)
		showConference(0, 0);
	else
		ui.conferenceListWidget->setCurrentRow(row);
}

void jJoinChat::showConference(QListWidgetItem *current, QListWidgetItem *previous)
{
	int row = current==0?-1:ui.conferenceListWidget->row(current);
	if(row < 1 || row >= m_c_list.size()+1)
	{
		row = 0;
		ui.conferenceNameLineEdit->setText("");
		ui.conferenceLineEdit->setEditText(m_room);
		ui.passwordLineEdit->setText(m_password);
		if (m_nick.isEmpty())
			ui.nickLineEdit->setText(m_jabber_account->getProtocol()->getAccountName());
		else
			ui.nickLineEdit->setText(m_nick);
		ui.autoJoinCheckBox->setChecked(false);
		ui.conferenceListWidget->setCurrentRow(row);
		ui.conferenceNameLineEdit->setFocus();
	}
	else
	{
		m_room = "";
		m_password = "";
		ui.conferenceNameLineEdit->setText(utils::fromStd(m_c_list[row-1].name));
		/*QString str = utils::fromStd(m_c_list[row-1].jid);
		int num = ui.conferenceLineEdit->findText(str);
		if (num == -1)
		{
			num = 1;
			ui.conferenceLineEdit->insertItem(num, str);
		}
		ui.conferenceLineEdit->setCurrentIndex(num);*/
		ui.conferenceLineEdit->setEditText(utils::fromStd(m_c_list[row-1].jid));
		ui.nickLineEdit->setText(utils::fromStd(m_c_list[row-1].nick));
		ui.passwordLineEdit->setText(utils::fromStd(m_c_list[row-1].password));
		ui.autoJoinCheckBox->setChecked(m_c_list[row-1].autojoin);
	}
}

void jJoinChat::setConferenceRoom(const QString &conference)
{
	showConference(0, 0);
	ui.conferenceLineEdit->setEditText(conference);
	ui.nickLineEdit->setText(m_jabber_account->getProtocol()->getAccountName());
}

void jJoinChat::finishSearch()
{
	ui.searchButton->setEnabled(true);
}

void jJoinChat::joinBookmark(QListWidgetItem *item)
{
	showConference(item, 0);
	if (ui.conferenceListWidget->currentRow() != 0)
		on_joinButton_clicked();
}

void jJoinChat::changeRecent(int i)
{
	if (!i)
		return;
	ui.conferenceLineEdit->setEditText(recent_conf[i-1]);
	ui.nickLineEdit->setText(recent_nick[i-1]);
	ui.passwordLineEdit->setText(recent_pass[i-1]);
}

void jJoinChat::on_joinButton_clicked()
{
	QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, m_settings, "recent");
	bool exist;
	for (int i = 0; i < recent_conf.size(); i++)
		if (recent_conf[i] == ui.conferenceLineEdit->currentText().trimmed() && recent_nick[i] == ui.nickLineEdit->text())
			exist = true;
	if (!exist)
	{
		recent_conf.insert(0, ui.conferenceLineEdit->currentText().trimmed());
		recent_nick.insert(0, ui.nickLineEdit->text());
		recent_pass.insert(0, ui.passwordLineEdit->text());
		recent_settings.beginWriteArray("conferences");
		for (int i = 0; i < recent_conf.size() && i < 10; ++i) {
			recent_settings.setArrayIndex(i);
			recent_settings.setValue("name", recent_conf[i]);
			recent_settings.setValue("nick", recent_nick[i]);
			recent_settings.setValue("password", recent_pass[i]);
		}
		recent_settings.endArray();
	}
	QSettings m_account_settings(QSettings::defaultFormat(), QSettings::UserScope, m_settings, "accountsettings");
	MUCRoom::HistoryRequestType request_type;
	QString value;
	m_account_settings.beginGroup("confhistory");
	if (ui.checkMessage->isChecked())
	{
		request_type = MUCRoom::HistoryMaxStanzas;
		value = ui.historyCount->cleanText();
		m_account_settings.setValue("type", "messages");
		m_account_settings.setValue("messages", value);
	}
	else if (ui.checkSecond->isChecked())
	{
		request_type = MUCRoom::HistorySeconds;
		QTime time = ui.historySeconds->time();
		int seconds = time.hour()*3600 + time.minute()*60 + time.second();
		value = QString("%1").arg(seconds);
		m_account_settings.setValue("type", "seconds");
		m_account_settings.setValue("seconds", value);
	}
	else if (ui.checkSince->isChecked())
	{
		request_type = MUCRoom::HistorySince;
		value = ui.historySince->dateTime().toUTC().toString("yyyy-MM-ddTHH:mm:ssZ");
	}
	m_account_settings.endGroup();
	emit createConferenceRoom(ui.conferenceLineEdit->currentText().trimmed(), ui.nickLineEdit->text(), ui.passwordLineEdit->text(), request_type, value);
	close();
}

void jJoinChat::on_searchButton_clicked()
{
	jServiceBrowser *jSConf = new jServiceBrowser("conference", m_jabber_account, true);
	connect(jSConf, SIGNAL(joinConference(const QString&)), this, SLOT(setConferenceRoom(const QString&)));
	connect(jSConf, SIGNAL(registerTransport(const QString&)), m_jabber_account, SLOT(registerTransport(const QString&)));
	connect(jSConf, SIGNAL(executeCommand(const QString&, const QString&)), m_jabber_account, SLOT(executeCommand(const QString&, const QString&)));
	connect(jSConf, SIGNAL(showVCard(const QString&)), m_jabber_account, SLOT(showInformation(const QString&)));
	connect(jSConf, SIGNAL(addContact(const QString&, const QString&)), m_jabber_account, SLOT(showAddDialog(const QString&, const QString&)));
	connect(jSConf, SIGNAL(searchService(const QString&, const QString&)), m_jabber_account, SLOT(showSearch(const QString&, const QString&)));
	connect(jSConf, SIGNAL(finishSearch()), this, SLOT(finishSearch()));
	jSConf->show();
	ui.searchButton->setEnabled(false);
}

void jJoinChat::on_saveButton_clicked()
{
	int row = ui.conferenceListWidget->currentRow() - 1;
	if (ui.conferenceNameLineEdit->text().isEmpty())
	{
		if (ui.conferenceLineEdit->currentText().trimmed().isEmpty())
			ui.conferenceNameLineEdit->text() = tr("New conference");
		else
			ui.conferenceNameLineEdit->setText(ui.conferenceLineEdit->currentText().trimmed());
	}
	bool isNew = false;
	if (row < 0)
	{
		row = m_c_list.size();
		ConferenceListItem item;
		m_c_list.append(item);
		//QString b_name = ui.conferenceNameLineEdit->text();
		QListWidgetItem *w_item = new QListWidgetItem("", ui.conferenceListWidget);
		ui.conferenceListWidget->addItem(w_item);
		isNew = true;
	}
	m_c_list[row].name = utils::toStd(ui.conferenceNameLineEdit->text());
	m_c_list[row].jid = utils::toStd(ui.conferenceLineEdit->currentText().trimmed());
	m_c_list[row].nick = utils::toStd(ui.nickLineEdit->text());
	m_c_list[row].password = utils::toStd(ui.passwordLineEdit->text());
	m_c_list[row].autojoin = ui.autoJoinCheckBox->isChecked();
	if (isNew)
		ui.conferenceListWidget->setCurrentRow(m_c_list.size());
	ui.conferenceListWidget->currentItem()->setText(ui.conferenceNameLineEdit->text());
	if (isLocal)
	{
		m_jabber_account->setRecentBookmarks(BookmarkList(), m_c_list.toStdList(), true);
		m_jabber_account->setRecentBookmarks(m_jabber_account->getRecentUrlmarks().toStdList(), m_jabber_account->getRecentBookmarks(true).toStdList());
	}
	else
		m_jabber_account->storeBookmarks(m_c_list);
	//m_bookmark_storage->requestBookmarks();
}

void  jJoinChat::on_addConferenceButton_clicked()
{
	ConferenceListItem item;
	m_c_list.append(item);
	int row = m_c_list.size() - 1;
	m_c_list[row].name = utils::toStd(tr("New conference"));
	m_c_list[row].nick = utils::toStd(m_jabber_account->getProtocol()->getAccountName());
	m_c_list[row].autojoin = false;
	QListWidgetItem *w_item = new QListWidgetItem(tr("New conference"),ui.conferenceListWidget);
	ui.conferenceListWidget->addItem(w_item);
	ui.conferenceListWidget->setCurrentItem(w_item);
	ui.conferenceNameLineEdit->setFocus();
	ui.conferenceNameLineEdit->selectAll();
}

void  jJoinChat::on_removeConferenceButton_clicked()
{
	QListWidgetItem *item = ui.conferenceListWidget->currentItem();
	int row = ui.conferenceListWidget->row(item);
	if (row == 0)
		return;
	m_c_list.removeAt(row-1);
	delete item;
	if (isLocal)
	{
		m_jabber_account->setRecentBookmarks(BookmarkList(), m_c_list.toStdList(), true);
		m_jabber_account->setRecentBookmarks(m_jabber_account->getRecentUrlmarks().toStdList(), m_jabber_account->getRecentBookmarks(true).toStdList());
	}
	else
		m_jabber_account->storeBookmarks(m_c_list);
	//m_bookmark_storage->requestBookmarks();
}

bool jJoinChat::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			on_joinButton_clicked();
		return QObject::eventFilter(obj, event);
	} else {
		return QObject::eventFilter(obj, event);
	}
}

void jJoinChat::setHistoryMessage()
{
	ui.checkMessage->setChecked(true);
	ui.historyCount->setEnabled(true);
	ui.historySeconds->setEnabled(false);
	ui.historySince->setEnabled(false);
}

void jJoinChat::setHistorySecond()
{
	ui.checkSecond->setChecked(true);
	ui.historyCount->setEnabled(false);
	ui.historySeconds->setEnabled(true);
	ui.historySince->setEnabled(false);

}

void jJoinChat::setHistorySince()
{
	ui.checkSince->setChecked(true);
	ui.historyCount->setEnabled(false);
	ui.historySeconds->setEnabled(false);
	ui.historySince->setEnabled(true);

}
