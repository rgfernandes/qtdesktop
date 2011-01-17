/*
    jAccount.h

    Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>
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

#include "jAccount.h"
#include "utils.h"
#include "conference/jConference.h"
#include "jProtocol.h"
#include "jPluginSystem.h"
#include "jRoster.h"
#include "jSlotSignal.h"
#include <gloox/sha.h>
#include <QMessageBox>

jAccount::jAccount(const QString &account_name, const QString &profile_name, jEventHandler *event_handler, QObject *parent) : QObject(parent), m_jabber_plugin_system(jPluginSystem::instance()), m_jabber_event_handler(event_handler) 
{
	m_status_icon = m_jabber_plugin_system.getStatusIcon("offline", "jabber");
	m_account_name = account_name;
	m_profile_name = profile_name;
	m_privacy_exist = false;
	m_autoaway_status = Presence::Unavailable;

	QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name/*+"/jabber."+m_account_name*/, "jabbericons");
	m_path_to_avatars = account_settings.fileName();
	m_path_to_avatars.truncate(m_path_to_avatars.length()-4);

	m_jabber_slotsignal = new jSlotSignal(this);
	m_jabber_roster = new jRoster(account_name, profile_name, this);
	m_jabber_protocol = new jProtocol(m_account_name, m_profile_name, m_jabber_roster, this);
	m_conference_management_object = m_jabber_protocol->getConferenceManagmentObject();
	connect(m_conference_management_object, SIGNAL(addToRoster(QString,QString)), this, SLOT(showAddDialog(QString,QString)));

	createStatusMenu();

	m_status = Presence::Unavailable;
	connect(m_jabber_roster, SIGNAL(addItemToContactList(const TreeModelItem &, QString)), m_jabber_slotsignal, SLOT(addItemToContactList(const TreeModelItem &, QString)));
	connect(m_jabber_roster, SIGNAL(removeItemFromContactList(TreeModelItem)), m_jabber_slotsignal, SLOT(removeItemFromContactList(TreeModelItem)));
	connect(m_jabber_roster, SIGNAL(setContactItemStatus(const TreeModelItem &, QString, int)), m_jabber_slotsignal, SLOT(setContactItemStatus(const TreeModelItem &, QString, int)));
	connect(m_jabber_roster, SIGNAL(modifyPrivacyList(QString,PrivacyItem,bool)), this, SLOT(modifyPrivacyList(QString,PrivacyItem,bool)));
	connect(m_jabber_protocol, SIGNAL(setRealStatus(int)), this, SLOT(setRealStatus(int)));
	connect(m_jabber_protocol, SIGNAL(setClientCustomIcon(const TreeModelItem &, const QString &, int)), m_jabber_slotsignal, SLOT(setClientCustomIcon(const TreeModelItem &, const QString &, int)));
	connect(m_jabber_protocol, SIGNAL(setClientCustomText(const TreeModelItem &, const QString &, int)), m_jabber_slotsignal, SLOT(setClientCustomText(const TreeModelItem &, const QString &, int)));
	connect(m_jabber_protocol, SIGNAL(addMessageFrom(const TreeModelItem &, const QDateTime &, const QString &)), m_jabber_slotsignal, SLOT(addMessageFrom(const TreeModelItem &, const QDateTime &, const QString &)));
	connect(m_jabber_roster, SIGNAL(addServiceMessage(const TreeModelItem &, const QString &)), m_jabber_slotsignal, SLOT(addServiceMessage(const TreeModelItem &, const QString &)));
	connect(m_jabber_protocol, SIGNAL(messageDelievered(const TreeModelItem &, int)), m_jabber_slotsignal, SLOT(messageDelievered(const TreeModelItem &, int)));
	connect(m_jabber_protocol, SIGNAL(sendTypingNotification(const TreeModelItem &, bool)), m_jabber_slotsignal, SLOT(sendTypingNotification(const TreeModelItem &, bool)));
	connect(m_conference_management_object, SIGNAL(addConferenceItem(const QString &,const QString &,const QString &,const QString &)), m_jabber_slotsignal, SLOT(addConferenceItem(const QString &,const QString &,const QString &,const QString &)));
	connect(m_conference_management_object, SIGNAL(removeConferenceItem(const QString &,const QString &,const QString &,const QString &)), m_jabber_slotsignal, SLOT(removeConferenceItem(const QString &,const QString &,const QString &,const QString &)));
	connect(m_conference_management_object, SIGNAL(renameConferenceItem(const QString &,const QString &,const QString &,const QString &,const QString &)), m_jabber_slotsignal, SLOT(renameConferenceItem(const QString &,const QString &,const QString &,const QString &,const QString &)));
	connect(m_conference_management_object, SIGNAL(setConferenceItemStatus(const QString &,const QString &,const QString &,const QString &,const QString &, int)), m_jabber_slotsignal, SLOT(setConferenceItemStatus(const QString &,const QString &,const QString &,const QString &,const QString &, int)));
	connect(m_conference_management_object, SIGNAL(setConferenceItemRole(const QString &,const QString &,const QString &,const QString &,const QString &, int)), m_jabber_slotsignal, SLOT(setConferenceItemRole(const QString &,const QString &,const QString &,const QString &,const QString &, int)));
	connect(m_conference_management_object, SIGNAL(setConferenceItemIcon(const QString &,const QString &,const QString &,const QString &,const QString &, int)), m_jabber_slotsignal, SLOT(setConferenceItemIcon(const QString &,const QString &,const QString &,const QString &,const QString &, int)));
	connect(m_conference_management_object, SIGNAL(addMessageToConference(const QString &, const QString &,const QString &, const QString &, const QDateTime &, bool)), m_jabber_slotsignal, SLOT(addMessageToConference(const QString &, const QString &,const QString &, const QString &, const QDateTime &, bool)));
	connect(m_conference_management_object, SIGNAL(changeOwnConferenceNickName(const QString &,const QString &, const QString &, const QString &)), m_jabber_slotsignal, SLOT(changeOwnConferenceNickName(const QString &,const QString &, const QString &, const QString &)));
	connect(m_conference_management_object, SIGNAL(createConference(const QString &, const QString &, const QString &)), m_jabber_slotsignal, SLOT(createConference(const QString &, const QString &, const QString &)));
	connect(m_jabber_roster, SIGNAL(clientVersion(const TreeModelItem &, const QString &)), m_jabber_slotsignal, SLOT(clientVersion(const TreeModelItem &, const QString &)));
	connect(m_jabber_protocol, SIGNAL(conferenceClientVersion(const QString &,const QString &,const QString &,const QString &, const QString &)), m_jabber_slotsignal, SLOT(conferenceClientVersion(const QString &,const QString &,const QString &,const QString &, const QString &)));
	connect(m_conference_management_object, SIGNAL(conferenceClientVersion(const QString &,const QString &,const QString &,const QString &, const QString &)), m_jabber_slotsignal, SLOT(conferenceClientVersion(const QString &,const QString &,const QString &,const QString &, const QString &)));
	connect(m_jabber_protocol, SIGNAL(systemNotification(const QString &, const QString &)), m_jabber_slotsignal, SLOT(systemNotification(const QString &, const QString &)));
	connect(m_jabber_protocol, SIGNAL(onFetchVCard(const QString&, const VCard*, const QString&)), m_jabber_slotsignal, SLOT(s_onFetchVCard(const QString&, const VCard*, const QString&)));
	//connect(m_jabber_protocol, SIGNAL(receiveSDisco(const JID&, const Disco::Items&, QString&)), m_jabber_slotsignal, SLOT(s_receiveSDisco(const JID&, const Disco::Items&, QString&)));
	//connect(m_jabber_protocol, SIGNAL(receiveSDiscoInfo(const JID&, const Disco::Info&, QString&)), m_jabber_slotsignal, SLOT(s_receiveSDiscoInfo(const JID&, const Disco::Info&, QString&)));
	connect(m_jabber_roster, SIGNAL(setItemIcon(const TreeModelItem &, const QString &, int)), m_jabber_slotsignal, SLOT(setClientCustomIcon(const TreeModelItem &, const QString &, int)));
	connect(m_jabber_roster, SIGNAL(setItemCustomText(const TreeModelItem &, const QString &, int)), m_jabber_slotsignal, SLOT(setClientCustomText(const TreeModelItem &, const QString &, int)));
	connect(m_conference_management_object, SIGNAL(setConferenceTopic(const QString &, const QString &, const QString &, const QString &)), m_jabber_slotsignal, SLOT(setConferenceTopic(const QString &, const QString &, const QString &, const QString &)));
	connect(m_conference_management_object, SIGNAL(addSystemMessageToConference(const QString &, const QString &, const QString &, const QString &, const QDateTime &, bool)), m_jabber_slotsignal, SLOT(addSystemMessageToConference(const QString &, const QString &, const QString &, const QString &, const QDateTime &, bool)));
	connect(m_jabber_protocol, SIGNAL(createAcceptAuthDialog(const QString &, const JID &, Client *)), m_jabber_slotsignal, SLOT(createAcceptAuthDialog(const QString &, const JID &, Client *)));
	connect(m_jabber_roster, SIGNAL(s_customNotification(const TreeModelItem &, const QString &)), m_jabber_slotsignal, SLOT(customNotification(const TreeModelItem &, const QString &)));
	connect(m_jabber_roster, SIGNAL(moveItemInContactList(TreeModelItem, TreeModelItem)), m_jabber_slotsignal, SLOT(moveItemInContactList(TreeModelItem, TreeModelItem)));
	connect(m_jabber_roster, SIGNAL(setContactItemName(TreeModelItem, QString)), m_jabber_slotsignal, SLOT(setContactItemName(TreeModelItem, QString)));
	connect(m_jabber_protocol, SIGNAL(bookmarksHandled()), m_jabber_slotsignal, SLOT(bookmarksHandled()));
	connect(m_jabber_protocol, SIGNAL(tagHandled(const QString &, bool)), m_jabber_slotsignal, SLOT(tagHandled(const QString &, bool)));
	connect(m_jabber_protocol, SIGNAL(conferenceInvite(const JID&, const JID&, const QString&, const QString&)), m_conference_management_object, SLOT(s_conferenceInvite(const JID&, const JID&, const QString&, const QString&)));
	connect(m_jabber_protocol, SIGNAL(setPrivacyLists(const QStringList&, const QString &)), m_jabber_slotsignal, SLOT(setPrivacyLists(const QStringList&, const QString &)));
//	connect(m_jabber_roster, SIGNAL(), m_jabber_slotsignal, SLOT());

	m_jabber_roster->constr();
	m_jabber_roster->loadSettings();
	m_jabber_roster->loadRosterFromSettings();

	m_adhoc = new Adhoc(m_jabber_protocol->getClient());
	setStatusDepends(false);
}

jAccount::~jAccount()
{
	delete m_jabber_protocol;
	delete m_jabber_slotsignal;
	delete m_account_button;
	delete m_additional_menu;
	delete m_status_menu;
}

void jAccount::createStatusMenu()
{
	m_join_groupchat = new QAction(jPluginSystem::instance().getIcon("chat") , tr("Join groupchat"), this);
	connect(m_join_groupchat, SIGNAL(triggered()), m_conference_management_object, SLOT(joinGroupchat()));
	m_additional_menu = new QMenu();
	m_additional_menu->setTitle(tr("Additional"));
	m_open_xml_console = new QAction(Icon("command"), tr("Open XML console"), this);
	m_add_contact = new QAction(Icon("add_user"), tr("Add new contact"), this);
	m_find_contact = new QAction(Icon("search"), tr("Find users"), this);
	//m_transport_browser = new QAction(tr("Transports"), this);
	m_service_browser = new QAction(Icon("defaultservice"), tr("Service browser"), this);
	m_change_vcard = new QAction(Icon("changedetails"), tr("View/change personal vCard"), this);

	m_privacy_list = new QMenu(tr("Privacy status"));
	m_privacy_list->setIcon(Icon("privacy"));
	m_privacy_list->setEnabled(false);

	QAction *mood_action = new QAction(Icon("mood"),tr("Set mood"),this);
	mood_action->setEnabled(false);
	QAction *activity_action = new QAction(Icon("activity"),tr("Set activity"),this);
	activity_action->setEnabled(false);
	m_jabber_protocol->setActions(/*bookmark_action, */mood_action, activity_action);

	//connect(m_transport_browser, SIGNAL(triggered()),
	//		this, SLOT(showTransportsBrowser()));
	//m_additional_menu->addAction(m_transport_browser);

	connect(m_service_browser, SIGNAL(triggered()),
			this, SLOT(showServicesBrowser()));
	m_additional_menu->addAction(m_service_browser);

	//m_add_contact->setIcon(QIcon(":/icons/servdisco"));
	connect(m_add_contact, SIGNAL(triggered()), this, SLOT(showAddDialog()));
	m_additional_menu->addAction(m_add_contact);

	connect(m_find_contact, SIGNAL(triggered()), this, SLOT(showSearch()));
	m_additional_menu->addAction(m_find_contact);

	m_additional_menu->addSeparator();

	connect(m_change_vcard, SIGNAL(triggered()), this, SLOT(showInformation()));
	m_additional_menu->addAction(m_change_vcard);

	connect(m_open_xml_console, SIGNAL(triggered()),
			m_jabber_slotsignal, SLOT(createXmlConsole()));
	m_additional_menu->addAction(m_open_xml_console);

	onlineAction = new QAction(m_jabber_plugin_system.getStatusIcon("online", "jabber"), tr("Online"), this);
	connect(onlineAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("online", onlineAction);

	offlineAction = new QAction(m_jabber_plugin_system.getStatusIcon("offline", "jabber"), tr("Offline"), this);
	connect(offlineAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("offline", offlineAction);

	ffcAction = new QAction(m_jabber_plugin_system.getStatusIcon("ffc", "jabber"), tr("Free for chat"), this);
	connect(ffcAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("ffc", ffcAction);

	awayAction = new QAction(m_jabber_plugin_system.getStatusIcon("away", "jabber"), tr("Away"), this);
	connect(awayAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("away", awayAction);

	xaAction = new QAction(m_jabber_plugin_system.getStatusIcon("na", "jabber"), tr("NA"), this);
	connect(xaAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("na", xaAction);

	dndAction = new QAction(m_jabber_plugin_system.getStatusIcon("dnd", "jabber"), tr("DND"), this);
	connect(dndAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("dnd", dndAction);

	QHashIterator<QString, QAction *> iterator(statusMenuActions);
	while (iterator.hasNext())
	{
		iterator.next();
		iterator.value()->setCheckable(true);
	}

	m_status_menu = new QMenu();
	m_status_menu->setTitle(m_account_name);
	m_status_menu->setIcon(m_jabber_plugin_system.getStatusIcon("offline", "jabber"));

	m_status_menu->addAction(onlineAction);
	m_status_menu->addAction(ffcAction);
	m_status_menu->addAction(awayAction);
	m_status_menu->addAction(xaAction);
	m_status_menu->addAction(dndAction);
	m_status_menu->addMenu(m_privacy_list);
	m_status_menu->addSeparator();
	m_status_menu->addAction(mood_action);
	m_status_menu->addAction(activity_action);
	m_status_menu->addSeparator();
	m_status_menu->addMenu(m_additional_menu);
	m_status_menu->addAction(m_join_groupchat);
	m_status_menu->addSeparator();
	m_status_menu->addAction(offlineAction);

	offlineAction->setChecked(true);
}

void jAccount::createAccountButton(QHBoxLayout *account_button_layout)
{
	m_account_button = new QToolButton();
	m_account_button->setMinimumSize(QSize(22, 22));
	m_account_button->setMaximumSize(QSize(22, 22));
	m_account_button->setAutoRaise(true);
#if defined(Q_OS_MAC)
	account_button_layout->addWidget(m_account_button, 0, Qt::AlignLeft);
#else
	account_button_layout->addWidget(m_account_button, 0, Qt::AlignRight);
#endif
	m_account_button->setToolTip(m_account_name);
	m_account_button->setIcon(m_jabber_plugin_system.getStatusIcon("offline", "jabber"));
	m_account_button->setPopupMode(QToolButton::InstantPopup);
	m_account_button->setMenu(m_status_menu);
}

QMenu *jAccount::getAccountMenu()
{
	return m_status_menu;
}

void jAccount::setStatusP(const Presence::PresenceType &presence, bool hide)
{
	QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
	QString password = account_settings.value("main/password","").toString();

	if (m_account_name.count("@") != 1)
	{
		QMessageBox::warning(0, "Connect", tr("You must use a valid jid. Please, recreate your jabber account."));
		return;
	}
	if (password.isEmpty())
	{
		QMessageBox::warning(0, "Connect", tr("You must enter a password in settings."));
		return;
	}

	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	QString message = utils::fromStd( m_jabber_protocol->getClient()->presence().status() );
	QString sname;
	sname = getStatusName(presence);

	if (!hide && presence != Presence::Unavailable)
	{
		bool dshow = settings.value("autoreply/" + sname + "dshow", false).toBool();
		if (!dshow || m_status == presence)
		{
			if (m_jabber_plugin_system.setStatusMessage(message, dshow))
			{
				settings.setValue("autoreply/" + sname + "dshow", dshow);
				if(dshow)
					settings.setValue("autoreply/" + sname + "msg", message);
			}
			else
				return;
		}
		else
			message = settings.value("autoreply/" + sname + "msg", "").toString();
	}
	m_gmail_hack_status = presence;
	m_jabber_protocol->setStatus(presence, message);
}

void jAccount::setStatus()
{
	QAction *act = qobject_cast<QAction *>(sender());
	Presence::PresenceType status;
	if (act == onlineAction)
	{
		status = Presence::Available;
	}
	else if (act == offlineAction)
	{
		status = Presence::Unavailable;
	}
	else if (act == ffcAction)
	{
		status = Presence::Chat;
	}
	else if (act == awayAction)
	{
		status = Presence::Away;
	}
	else if (act == xaAction)
	{
		status = Presence::XA;
	}
	else if (act == dndAction)
	{
		status = Presence::DND;
	}
	act->setChecked(false);
	setStatusP(status);
}

void jAccount::setRealStatus(int status)
{
	QHashIterator<QString, QAction *> iterator(statusMenuActions);
	while (iterator.hasNext())
	{
		iterator.next();
		iterator.value()->setChecked(false);
	}

	QString name = "connecting";
	if (status != -1)
	{
		Presence::PresenceType presence = static_cast<gloox::Presence::PresenceType>(status);
		if(presence != Presence::Unavailable)
			getEventHandler()->accountStatusChanged(m_account_name, getStatusName(presence), utils::fromStd(m_jabber_protocol->getClient()->presence().status()));
		name = getStatusName(presence);
		statusMenuActions.value(name)->setChecked(true);
		if (presence == Presence::Unavailable)
			setStatusDepends(false);
		else if (m_status == Presence::Unavailable)
			setStatusDepends(true);
		m_status = presence;
		QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "recent");
		recent_settings.setValue("main/prevstatus", name);
	}
	m_status_icon = m_jabber_plugin_system.getStatusIcon(name, "jabber");
	m_status_menu->setIcon(m_status_icon);
	m_account_button->setIcon(m_status_icon);
	m_jabber_plugin_system.updateStatusIcons();
	m_conference_management_object->sendPresenceToAll();
}

QString jAccount::getStatusIconName(Presence::PresenceType presence)
{
	return m_jabber_plugin_system.getStatusIconFileName(jAccount::getStatusName(presence), "jabber");
}

QString jAccount::getStatusName(Presence::PresenceType presence)
{
	QString name;
	switch(presence)
	{
		case Presence::Available:
			name = "online";
			break;
		case Presence::Chat:
			name = "ffc";
			break;
		case Presence::Away:
			name = "away";
			break;
		case Presence::DND:
			name = "dnd";
			break;
		case Presence::XA:
			name = "na";
			break;
		case Presence::Probe:
		case Presence::Error:
		case Presence::Invalid:
		case Presence::Unavailable:
			name = "offline";
			break;
	}
	return name;
}

Presence::PresenceType jAccount::getPresence(const QString &status)
{
	Presence::PresenceType precence;

	if (status == "online")
		precence = Presence::Available;
	else if (status == "ffc")
		precence = Presence::Chat;
	else if (status == "away")
		precence = Presence::Away;
	else if (status == "dnd")
		precence = Presence::DND;
	else if (status == "na")
		precence = Presence::XA;
	else
		precence = Presence::Unavailable;

	return precence;
}

int jAccount::getStatusMass(Presence::PresenceType presence)
{
	int mass;
	switch(presence)
	{
		case Presence::Available:
			mass = 10;
			break;
		case Presence::Chat:
			mass = 20;
			break;
		case Presence::Away:
			mass = 30;
			break;
		case Presence::DND:
			mass = 50;
			break;
		case Presence::XA:
			mass = 40;
			break;
		case Presence::Probe:
		case Presence::Error:
		case Presence::Invalid:
		case Presence::Unavailable:
			mass = 1000;
			break;
		default:
			mass = 0;
	}
	return mass;
}

void jAccount::showVCardWidget(const QString &jid_base) {
	QString jid = jid_base;
	jid.remove(QRegExp("/.*"));
	if (m_conference_management_object->JIDIsRoom(jid))
		jid = jid_base;
	if (m_vcard_list.contains(jid))
		return;
	bool mode;
	if (m_account_name == jid) mode = true;
	else mode = false;
	jVCard *vcard = new jVCard(jid, mode);
	m_vcard_list.insert(jid, vcard);
	m_vcard_list.value(jid)->setWindowIcon(m_jabber_plugin_system.getIcon("vcard"));
	m_vcard_list.value(jid)->setWindowTitle(jid);
	connect(m_vcard_list.value(jid), SIGNAL(requestVCard(const QString&)), this, SLOT(s_requestVCard(const QString&)));
	connect(m_vcard_list.value(jid), SIGNAL(saveVCard(VCard*)), this, SLOT(s_saveVCard(VCard*)));
	connect(m_vcard_list.value(jid), SIGNAL(closeVCard(const QString&)), this, SLOT(s_closeVCard(const QString&)));
	m_vcard_list.value(jid)->fillData();
	m_vcard_list.value(jid)->show();
}

void jAccount::s_requestVCard(const QString &jid)
{
	m_jabber_protocol->fetchVCard(jid);
}

void jAccount::setVCardInfo(const VCard *vcard, const QString &jid_base, const QString &avatarUrl)
{
	QString jid = jid_base;
	jid.remove(QRegExp("/.*"));
	if (m_conference_management_object->JIDIsRoom(jid))
		jid = jid_base;
	if(m_vcard_list.contains(jid))
		m_vcard_list.value(jid)->setVCard(vcard, avatarUrl);
	else
		qDebug() << "unknown vCard" << jid;
}

void jAccount::s_saveVCard(VCard *vcard)
{
	QString hex = "";
	const VCard::Photo &photo = vcard->photo();
	if(!photo.binval.empty())
	{
		QByteArray data(photo.binval.c_str(),photo.binval.length());
		SHA sha;
		sha.feed(photo.binval);
		sha.finalize();
		hex = utils::fromStd(sha.hex());
	}
	m_jabber_protocol->updateAvatarPresence(hex);
	m_jabber_protocol->storeVCard(vcard);
}

void jAccount::s_closeVCard(const QString &jid)
{
	m_vcard_list.remove(jid);
}

void jAccount::getDiscoItem(const QString &server, const QString &node, DiscoHandler *handler)
{
	m_jabber_protocol->getDiscoItems(server, node, handler);
}

void jAccount::getDiscoInfo(const QString &server, const QString &node, DiscoHandler *handler)
{
	m_jabber_protocol->getDiscoInfo(server, node, handler);
}

void jAccount::showAddDialog(const QString &jid, const QString &nick)
{
	//jSearchUser *searchDlg = new jSearchUser(this);
	//searchDlg->show();
	QStringList groups;
	groups = m_jabber_roster->getGroups();
	groups.removeAll(tr("Services"));
	groups.removeAll(tr("Conferences"));
	jAddContact *jadduser = new jAddContact(groups, jid, nick);
	jadduser->setWindowIcon(Icon("add_user"));
	jadduser->setWindowTitle(tr("Add new contact on") + " " + getAccountName());
	connect(jadduser, SIGNAL(showInformation(const QString&)), this, SLOT(showInformation(const QString&)));
	connect(jadduser, SIGNAL(addContact(const QString&, const QString&, const QString&, bool)), this, SLOT(addContact(const QString&, const QString&, const QString&, bool)));
	jadduser->show();
}

void jAccount::addContact(const QString &jid, const QString &name, const QString &group, bool authorize)
{
	m_jabber_protocol->addContact(jid, name, group, authorize);
}

void  jAccount::s_requestDiscoItems(const QString &server, const QString &node)
{
}

/*void jAccount::showTransportsBrowser()
{
	jServiceBrowser *service_browser = new jServiceBrowser("gateway", this);
	connect(service_browser, SIGNAL(registerTransport(const QString&)), this, SLOT(registerTransport(const QString&)));
	connect(service_browser, SIGNAL(executeCommand(const QString&, const QString&)), this, SLOT(executeCommand(const QString&, const QString&)));
	connect(service_browser, SIGNAL(showVCard(const QString&)), this, SLOT(showInformation(const QString&)));
	connect(service_browser, SIGNAL(addContact(const QString&, const QString&)), this, SLOT(showAddDialog(const QString&, const QString&)));
	connect(service_browser, SIGNAL(searchService(const QString&, const QString&)), this, SLOT(showSearch(const QString&, const QString&)));
	service_browser->show();

}*/

void jAccount::showServicesBrowser()
{
	jServiceBrowser *service_browser = new jServiceBrowser("", this);
	connect(service_browser, SIGNAL(joinConference(const QString&)), this, SLOT(joinConference(const QString&)));
	connect(service_browser, SIGNAL(registerTransport(const QString&)), this, SLOT(registerTransport(const QString&)));
	connect(service_browser, SIGNAL(executeCommand(const QString&, const QString&)), this, SLOT(executeCommand(const QString&, const QString&)));
	connect(service_browser, SIGNAL(showVCard(const QString&)), this, SLOT(showInformation(const QString&)));
	connect(service_browser, SIGNAL(addProxy(const JID&)), getProtocol()->getFileTransfer(), SLOT(searchSocks5Proxy(const JID&)));
	connect(service_browser, SIGNAL(addContact(const QString&, const QString&)), this, SLOT(showAddDialog(const QString&, const QString&)));
	connect(service_browser, SIGNAL(searchService(const QString&, const QString&)), this, SLOT(showSearch(const QString&, const QString&)));
	service_browser->show();
}

void jAccount::joinConference(const QString &conference)
{
	m_conference_management_object->joinGroupchat(conference, "", "", true);
}

void jAccount::registerTransport(const QString &transport)
{
	getProtocol()->showTransportReg(transport);
}

void jAccount::showSearch(const QString &type, const QString &jid)
{
/*	if (type == "user")
	{
		jSearchUser *searchDlg = new jSearchUser(this, jid);
		searchDlg->show();
	}
	else*/
	jSearch *search = new jSearch(this, jid);
	connect(search, SIGNAL(addContact(const QString&, const QString&)), this, SLOT(showAddDialog(const QString&, const QString&)));
	search->show();
}

void jAccount::executeCommand(const QString &command, const QString &node)
{
    jAdhoc *adhoc = new jAdhoc(command, m_adhoc);
    adhoc->execute(node);
}

void jAccount::adhoc(const QString &jid)
{
    jAdhoc *adhoc = new jAdhoc(jid, m_adhoc);
    adhoc->getCommands();
}

void jAccount::setPrivacyStatus(bool checked)
{
	if (!checked)
		return;
	QString label = ((QAction*) sender())->text();
	QString list;
	if (label == tr("Invisible for all"))
			list = "invisible";
		else if (label == tr("Visible for all"))
			list = "visible";
		else if (label == tr("Visible only for visible list"))
			list = "visible list";
		else if (label == tr("Invisible only for invisible list"))
			list = "invisible list";
		else
			list = label;
	getProtocol()->setPrivacyStatus(list);
}

void jAccount::modifyPrivacyList(const QString &list, const PrivacyItem &jid, bool add)
{
	QList<PrivacyItem> privacy = m_jabber_protocol->getPrivacyList(list);
	if (add)
		privacy.insert(0, jid);
	else
		privacy.removeOne(jid);
	m_jabber_protocol->storePrivacyList(list, privacy.toStdList());
}

void jAccount::generalSettingsChanged()
{

}

void jAccount::setAutoAway()
{
	if (m_status == Presence::Available || m_status == Presence::Chat)
	{
		m_autoaway_status = m_status;
		setStatusP(Presence::Away, true);
	}
}

void jAccount::setStatusAfterAutoAway()
{
	if (m_autoaway_status != Presence::Unavailable)
	{
		setStatusP(m_autoaway_status, true);
		m_autoaway_status = Presence::Unavailable;
	}
}

void jAccount::setStatusDepends(bool state)
{
	m_join_groupchat->setEnabled(state);
	m_add_contact->setEnabled(state);
	m_change_vcard->setEnabled(state);
	m_service_browser->setEnabled(state);
	if (!state)
		m_privacy_list->setEnabled(state);
}

void jAccount::setRecentBookmarks(const BookmarkList &b_list, const ConferenceList &c_list, bool isLocal)
{
	QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, isLocal ? "accountsettings" : "recent");
	recent_settings.beginGroup("main");
	recent_settings.setValue("available", true);
	recent_settings.endGroup();
	recent_settings.beginWriteArray("bookmarks");
	QList<ConferenceListItem> m_c_list = QList<ConferenceListItem>::fromStdList(c_list);
	for (int i = 0; i < m_c_list.size(); ++i)
	{
		recent_settings.setArrayIndex(i);
		recent_settings.setValue("name", utils::fromStd(m_c_list[i].name));
		recent_settings.setValue("jid", utils::fromStd(m_c_list[i].jid));
		recent_settings.setValue("nick", utils::fromStd(m_c_list[i].nick));
		recent_settings.setValue("password", utils::fromStd(m_c_list[i].password));
		recent_settings.setValue("autojoin", m_c_list[i].autojoin);
	}
	recent_settings.endArray();
	if (isLocal)
		return;
	recent_settings.beginWriteArray("urlmarks");
	QList<BookmarkListItem> m_b_list = QList<BookmarkListItem>::fromStdList(b_list);
	for (int i = 0; i < m_b_list.size(); ++i)
	{
		recent_settings.setArrayIndex(i);
		recent_settings.setValue("name", utils::fromStd(m_b_list[i].name));
		recent_settings.setValue("url", utils::fromStd(m_b_list[i].url));
	}
	recent_settings.endArray();
}


void jAccount::clearRecentBookmarks()
{
	QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "recent");
	recent_settings.beginGroup("main");
	recent_settings.setValue("available", false);
	recent_settings.endGroup();
	recent_settings.remove("bookmarks");
	recent_settings.remove("urlmarks");
}

QList<ConferenceListItem> jAccount::getRecentBookmarks(bool isLocal)
{
	QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, isLocal ? "accountsettings" : "recent");
	QList<ConferenceListItem> c_list;
	int size = recent_settings.beginReadArray("bookmarks");
	for (int i = 0; i < size; ++i)
	{
		recent_settings.setArrayIndex(i);
		ConferenceListItem item;
		item.name = utils::toStd(recent_settings.value("name").toString());
		item.jid = utils::toStd(recent_settings.value("jid").toString());
		item.nick = utils::toStd(recent_settings.value("nick").toString());
		item.password = utils::toStd(recent_settings.value("password").toString());
		item.autojoin = recent_settings.value("autojoin").toBool();
		c_list << item;
	}
	recent_settings.endArray();
	return c_list;
}

QList<BookmarkListItem> jAccount::getRecentUrlmarks()
{
	QSettings recent_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "recent");
	QList<BookmarkListItem> b_list;
	int size = recent_settings.beginReadArray("urlmarks");
	for (int i = 0; i < size; ++i)
	{
		recent_settings.setArrayIndex(i);
		BookmarkListItem item;
		item.name = utils::toStd(recent_settings.value("name").toString());
		item.url = utils::toStd(recent_settings.value("url").toString());
		b_list << item;
	}
	recent_settings.endArray();
	return b_list;
}

void jAccount::storeBookmarks(const QList<ConferenceListItem> &c_list)
{
	BookmarkList b_list = getRecentUrlmarks().toStdList();
	getProtocol()->getBookmarkStorage()->storeBookmarks(b_list, c_list.toStdList());
	setRecentBookmarks(b_list, c_list.toStdList());
}
