/*
    jSlotSignal.cpp

    Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>
						2008-2009 by Denis Daschenko <daschenko@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "jSlotSignal.h"
#include "jClientIdentification.h"
#include "acceptautdialog.h"
#include "conference/jConference.h"
#include "utils.h"


jSlotSignal::jSlotSignal(jAccount *parent) : m_jabber_account(parent)
{
	m_xml_console=0;
	//m_bookmark_settings=0;
	isConnected = false;
}

jSlotSignal::~jSlotSignal()
{
}

void jSlotSignal::setContactItemStatus(const TreeModelItem &item, const QString &status, int mass)
{
	m_jabber_account->getPluginSystem().setContactItemStatus(item, m_jabber_account->getPluginSystem().getStatusIcon(status, "jabber"), status, mass);
}

void jSlotSignal::addItemToContactList(const TreeModelItem &item, const QString &item_name)
{
//	qDebug() << item.m_item_name << item.m_item_type << "added SlotSignal";
	m_jabber_account->getPluginSystem().addItemToContactList(item, item_name);
}

void jSlotSignal::removeItemFromContactList(TreeModelItem Item)
{
	m_jabber_account->getPluginSystem().removeItemFromContactList(Item);
}

void jSlotSignal::setClientCustomIcon(const TreeModelItem & item, const QString &icon_name, int position)
{
	QIcon icon = m_jabber_account->getPluginSystem().getIcon(icon_name);
	if(icon.actualSize(QSize(16,16)).width()<0)
		icon = QIcon(icon_name);
	m_jabber_account->getPluginSystem().setContactItemIcon(item, icon, position);
}

void jSlotSignal::setClientCustomText(const TreeModelItem &item, const QString &text, int position)
{
	QList<QVariant> list;
	if(!text.isEmpty())
	{
		QString tmp_text = text;
		tmp_text.replace("\n"," | ");
		list.append(tmp_text);
	}
	m_jabber_account->getPluginSystem().setContactItemRow(item, list, position);
}

void jSlotSignal::addMessageFrom(const TreeModelItem &item, const QDateTime &date, const QString &text)
{
	m_jabber_account->getPluginSystem().addMessageFromContact(item,text,date);
}

void jSlotSignal::addServiceMessage(const TreeModelItem &item, const QString &text)
{
	m_jabber_account->getPluginSystem().addServiceMessage(item,text);
}

void jSlotSignal::messageDelievered(const TreeModelItem &item, int position)
{
	m_jabber_account->getPluginSystem().messageDelievered(item, position);
}

void jSlotSignal::sendTypingNotification(const TreeModelItem &item, bool composing)
{
	m_jabber_account->getPluginSystem().contactTyping(item, composing);
}

void jSlotSignal::addConferenceItem(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname)
{
	m_jabber_account->getPluginSystem().addConferenceItem(protocol_name,conference_name,account_name,nickname);
	m_jabber_account->getPluginSystem().setConferenceItemIcon(protocol_name, conference_name, account_name, nickname,QIcon(":/icons/clients/unknown"),12);
}

void jSlotSignal::removeConferenceItem(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname)
{
	m_jabber_account->getPluginSystem().removeConferenceItem(protocol_name,conference_name,account_name,nickname);
}

void jSlotSignal::renameConferenceItem(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname,const QString &new_nickname)
{
	m_jabber_account->getPluginSystem().renameConferenceItem(protocol_name,conference_name,account_name,nickname,new_nickname);
}

void jSlotSignal::setConferenceItemStatus(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname,const QString &status, int mass)
{
	QIcon icon;
	switch(mass)
	{
	case 1:
		icon=jPluginSystem::instance().getStatusIcon(Presence::Chat);
		break;
	case 2:
		icon=jPluginSystem::instance().getStatusIcon(Presence::Away);
		break;
	case 3:
		icon=jPluginSystem::instance().getStatusIcon(Presence::DND);;
		break;
	case 4:
		icon=jPluginSystem::instance().getStatusIcon(Presence::XA);
		break;
	default:
		icon=jPluginSystem::instance().getStatusIcon(Presence::Available);
		break;
	}
	m_jabber_account->getPluginSystem().setConferenceItemStatus(protocol_name,conference_name,account_name,nickname,icon,status,mass);
}

void jSlotSignal::setConferenceItemRole(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname,const QString &role, int mass)
{
	QIcon icon;
	switch(mass)
	{
	case 1:
		icon=QIcon(":/icons/dnd.png");
		break;
	case 2:
		icon=QIcon(":/icons/online.png");
		break;
	case 3:
		icon=QIcon(":/icons/chatty.png");
		break;
	}
	m_jabber_account->getPluginSystem().setConferenceItemRole(protocol_name,conference_name,account_name,nickname,icon,role,mass);
}

void jSlotSignal::addMessageToConference(const QString &conference_name, const QString &account_name,
		const QString &from, const QString &message, const QDateTime &date, bool history)
{
	m_jabber_account->getPluginSystem().addMessageToConference("Jabber", conference_name,
			account_name,
			from,
			message, date, history);
}

void jSlotSignal::conferenceClientVersion(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname, const QString &name)
{
	m_jabber_account->getPluginSystem().setConferenceItemIcon(protocol_name, conference_name, account_name, nickname,jClientIdentification::instance().clientIcon(name.isEmpty()?"unknown":name),12);
}
void jSlotSignal::clientVersion(const TreeModelItem &contact, const QString &name)
{
	m_jabber_account->getPluginSystem().setContactItemIcon(contact,jClientIdentification::instance().clientIcon(name.isEmpty()?"unknown":name),12);
}

void jSlotSignal::systemNotification(const QString &account_name, const QString &message)
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = account_name;
	contact.m_item_name = account_name;
	contact.m_item_type = 2;
	m_jabber_account->getPluginSystem().systemNotifiacation(contact, message);
}

void jSlotSignal::s_onFetchVCard(const QString &jid, const VCard *vcard, const QString &avatarUrl)
{
	m_jabber_account->setVCardInfo(vcard, jid, avatarUrl);
}

void jSlotSignal::changeOwnConferenceNickName(const QString &protocol_name,
		const QString &conference_name, const QString &account_name, const QString &nickname)
{
	m_jabber_account->getPluginSystem().changeOwnConferenceNickName(protocol_name, conference_name, account_name, nickname);
}

void jSlotSignal::createConference(const QString &protocol_name, const QString &conference_name, const QString &account_name)
{
	m_jabber_account->getPluginSystem().createConference(protocol_name, conference_name, account_name);
}

void jSlotSignal::setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QString &icon_name, int position)
{

	QIcon icon = m_jabber_account->getPluginSystem().getIcon(icon_name);
	if(icon.actualSize(QSize(16,16)).width()<0)
		icon = QIcon(icon_name);
	m_jabber_account->getPluginSystem().setConferenceItemIcon(protocol_name, conference_name, account_name, nickname,icon,position);
}
void jSlotSignal::setConferenceTopic(const QString &protocol_name, const QString &conference_name,
		const QString &account_name, const QString &topic)
{
	m_jabber_account->getPluginSystem().setConferenceTopic(protocol_name, conference_name, account_name, topic);
}

void jSlotSignal::addSystemMessageToConference(const QString &protocol_name, const QString &conference_name,
		const QString &account_name, const QString &message, const QDateTime &date, bool history)
{
	m_jabber_account->getPluginSystem().addSystemMessageToConference(protocol_name, conference_name, account_name, message, date, history);
}

void jSlotSignal::createAcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client)
{
	AcceptAuthDialog *widget = new AcceptAuthDialog(text,jid,jabber_client);
	widget->show();
}

void jSlotSignal::customNotification(const TreeModelItem &item, const QString &message)
{
	m_jabber_account->getPluginSystem().customNotifiacation(item, message);
}

void jSlotSignal::moveItemInContactList(const TreeModelItem &OldItem, const TreeModelItem &NewItem)
{
	m_jabber_account->getPluginSystem().moveItemInContactList(OldItem, NewItem);
}

void jSlotSignal::setContactItemName(const TreeModelItem &Item, const QString &name)
{
	m_jabber_account->getPluginSystem().setContactItemName(Item, name);
}
void jSlotSignal::bookmarksHandled()
{
	//m_jabber_account->getJabberRoster()->setConferences();
	if (isConnected)
	{
		m_jabber_account->getConferenceManagementObject()->setConferences();
	}
	else
	{
		isConnected = true;
		ConferenceList c_list = m_jabber_account->getRecentBookmarks().toStdList();
		ConferenceList::const_iterator ci = c_list.begin();
		for(;ci!=c_list.end();ci++)
		{
			if((*ci).autojoin)
			{
				JID jid((*ci).jid);
				QString server = utils::fromStd(jid.server());
				QString room = utils::fromStd(jid.username());
				QString conference = tr("%1@%2").arg(room).arg(server);
				QString nick = utils::fromStd((*ci).nick);
				QString password = utils::fromStd((*ci).password);
				m_jabber_account->getConferenceManagementObject()->s_createConferenceRoom(conference, nick, password);
			}
		}
	}
}

void jSlotSignal::tagHandled(const QString &xml, bool in)
{
	if(m_xml_console)
	{
		m_xml_console->appendTag(xml,in);
	}
}

void jSlotSignal::createXmlConsole()
{
	if(!m_xml_console)
	{
		m_xml_console = new XmlConsole("Xml Console on " + m_jabber_account->getAccountName());
		m_xml_console->show();
		connect(m_xml_console, SIGNAL(destroyed()), this, SLOT(destroyXmlConsole()));
		connect(m_xml_console, SIGNAL(send(QString)), this, SLOT(sendXml(QString)));
	}
}

void jSlotSignal::destroyXmlConsole()
{
	m_xml_console = 0;
}

void jSlotSignal::sendXml(QString XML)
{
    m_jabber_account->getProtocol()->getClient()->connectionImpl()->send(utils::toStd(XML));

}

void jSlotSignal::setPrivacyLists(const QStringList &privacy, const QString &active)
{
	QAction *activeAction = 0;
	QMenu *menu = m_jabber_account->getPrivacyMenu();
	menu->setEnabled(true);
	foreach (QAction *action, menu->actions())
		delete action;
	QActionGroup *actionsGroup = new QActionGroup(this);
	QStringList default_lists;
	default_lists << "visible" << "invisible list" << "visible list" << "invisible";
	foreach (QString list, default_lists)
	{
		QString label;
		if (list == "invisible")
			label = tr("Invisible for all");
		else if (list == "visible")
			label = tr("Visible for all");
		else if (list == "visible list")
			label = tr("Visible only for visible list");
		else if (list == "invisible list")
			label = tr("Invisible only for invisible list");
		QAction *action = new QAction(label, this);
		action->setCheckable(true);
		if (list == active)
			action->setChecked(true);
		actionsGroup->addAction(action);
		menu->addAction(action);
		connect(action, SIGNAL(toggled(bool)), m_jabber_account, SLOT(setPrivacyStatus(bool)));
	}
	m_jabber_account->setPrivacyExist(true);
}
