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

#include "jSaveBookmark.h"
#include "utils.h"

jSaveBookmark::jSaveBookmark(jAccount *jabber_account, const QString &conference, const QString &nick, QString &password) : m_jabber_account(jabber_account)
{
	ui.setupUi(this);
	ui.saveButton->setIcon(Icon("apply"));
	ui.cancelButton->setIcon(Icon("cancel"));
	ui.lineConference->setText(conference);
	ui.lineName->setText(conference);
	ui.lineNick->setText(nick);
	ui.linePassword->setText(password);
}

void jSaveBookmark::on_saveButton_clicked()
{
	QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_jabber_account->getProfileName()+"/jabber."+m_jabber_account->getAccountName(), "recent");
	bool isSupportBookmarks = recent_settings.value("main/available", false).toBool();
	if (isSupportBookmarks)
	{
		QList<ConferenceListItem> c_list = m_jabber_account->getRecentBookmarks();
		if (ui.lineName->text().isEmpty())
			ui.lineName->setText(ui.lineConference->text());
		bool exist = false;
		int row;
		for (int i = 0; i < c_list.count(); i++)
			if (utils::fromStd(c_list[i].jid) == ui.lineConference->text() && utils::fromStd(c_list[i].nick) == ui.lineNick->text())
			{
				exist = true;
				row = i;
				break;
			}
		ConferenceListItem item;
		item.name = utils::toStd(ui.lineName->text());
		item.jid = utils::toStd(ui.lineConference->text());
		item.nick = utils::toStd(ui.lineNick->text());
		item.password = utils::toStd(ui.linePassword->text());
		item.autojoin = ui.checkAutojoin->isChecked();
		if (exist)
		{
			c_list.replace(row, item);
		}
		else
		{
			c_list << item;
		}
		QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_jabber_account->getProfileName()+"/jabber."+m_jabber_account->getAccountName(), "accountsettings");
		bool isLocal = account_settings.value("main/localbookmark",false).toBool();
		if (isLocal)
		{
			m_jabber_account->setRecentBookmarks(BookmarkList(), c_list.toStdList(), true);
			m_jabber_account->setRecentBookmarks(m_jabber_account->getRecentUrlmarks().toStdList(), m_jabber_account->getRecentBookmarks(true).toStdList());
		}
		else
			m_jabber_account->storeBookmarks(c_list);
	}
	close();
}
