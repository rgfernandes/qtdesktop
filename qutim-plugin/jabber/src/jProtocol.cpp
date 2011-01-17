/*
jProtocol.cpp

Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>
						   Nigmatullin Ruslan <euroelessar@gmail.com>
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

#include "jProtocol.h"
#include "jRoster.h"
#include <gloox/glooxversion.h>

jProtocol::jProtocol(const QString &account_name, const QString &profile_name, jRoster *jabber_roster, jAccount *parent) : m_jabber_roster(jabber_roster), m_jabber_account(parent)
{
	m_state_online = false;
	m_account_name = account_name;
	m_profile_name = profile_name;
	qDebug() << m_account_name;
	/*CustomStatusDialog *dialog = new CustomStatusDialog(m_account_name,m_profile_name);
	dialog->setStatuses("");
	dialog->show();
	dialog->exec();
	*/
	JID jid(utils::toStd(m_account_name));
	jClient = new Client(jid,"");
	m_connection = new jConnection(jClient, m_profile_name, m_account_name);
	jClient->setConnectionImpl(m_connection);
	jClient->setXmlLang( utils::toStd( tr("en", "xml:lang") ) );
	connect(m_connection, SIGNAL(reconnecting()), this, SLOT(reconnecting()));
	if(!jPluginSystem::instance().getSOCKS5Server())
	{
		QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
		settings.beginGroup("filetransfer");
		int socks5port = settings.value("socks5port", 8010).toInt();
		SOCKS5BytestreamServer *socks5server = new SOCKS5BytestreamServer(jClient->logInstance(), socks5port);
#if (GLOOXVERSION >= 0x010100)
		socks5server->setServerImpl(new jConnectionServer(socks5server));
#endif
		jPluginSystem::instance().setSOCKS5Server(socks5server, socks5port);
		settings.endGroup();
	}

//	jClient->logInstance().registerLogHandler(LogLevelWarning,LogAreaClassDns,this);
	loadSettings();

	quint8 qutimver_major, qutimver_minor, qutimver_secminor;
	quint16 qutimver_svn;
	m_jabber_account->getPluginSystem().getQutimVersion(qutimver_major, qutimver_minor, qutimver_secminor, qutimver_svn);
	QString qutimver = QApplication::applicationVersion();
	if(qutimver_svn > 0)
	{
		qutimver += " r";
		qutimver += QString::number(qutimver_svn);
	}

	m_conference_management_object = new jConference(this, jClient, parent);
//	qDebug() << m_conference_management_object;
	Capabilities *caps = new Capabilities( jClient->disco() );
	caps->setNode( "http://qutim.org" );
	jClient->addPresenceExtension( caps );
//	((Capabilities*)jClient->presenceExtensions().value(ExtCaps))->setNode("http://qutim.org");
	jClient->disco()->addFeature("http://jabber.org/protocol/bytestreams");
	jClient->disco()->addFeature("http://jabber.org/protocol/si");
//	jClient->disco()->addFeature("http://jabber.org/protocol/si/profile/file-transfer");
	jClient->disco()->addFeature("http://jabber.org/protocol/disco#info");
//	jClient->disco()->addFeature("http://jabber.org/protocol/commands");
	jClient->disco()->addFeature("http://jabber.org/protocol/rosterx");
	jClient->disco()->addFeature("http://jabber.org/protocol/muc");
	jClient->disco()->addFeature("jabber:x:data");
	jClient->disco()->addFeature("http://jabber.org/protocol/chatstates");
	jClient->disco()->addFeature("http://jabber.org/protocol/activity+notify");
	jClient->disco()->addFeature("http://jabber.org/protocol/mood+notify");
	jClient->disco()->addFeature("http://jabber.org/protocol/tune+notify");
	jClient->disco()->addFeature("http://jabber.org/protocol/physloc+notify");
	jClient->disco()->addFeature("http://jabber.org/protocol/geoloc+notify");
	jClient->disco()->addFeature("http://www.xmpp.org/extensions/xep-0084.html#ns-metadata+notify");
	jClient->disco()->addFeature("urn:xmpp:receipts");
//	jClient->disco()->addFeature("http://jabber.org/protocol/xhtml-im");
//	jClient->disco()->addFeature("urn:xmpp:tmp:sxe");
//	jClient->disco()->addFeature("http://www.w3.org/2000/svg");
	QString os, timezone; 
	int timezone_offset;
	jPluginSystem::i()->getSystemInfo(os, timezone, timezone_offset); 
	jClient->disco()->setVersion("qutIM", utils::toStd(qutimver), utils::toStd(os));
	// jClient->disco()->setVersion("qutIM", utils::toStd(qutimver), utils::toStd(SystemInfo::instance()->os()));
	jClient->disco()->setIdentity("client", "pc","qutIM");
//	jClient->disco()->addIdentity("client", "pc","qutIM/"+utils::toStd(qutimver)+"/"+utils::toStd(os));
//	m_conference_management_object = new jConference(this, jClient,m_plugin_system);
//	jClient->setCompressionImpl(new jCompression(jClient));
//	CompressionBase* cmp = new CompressionZlib( this );
//	qDebug() << mp->init();
//	jClient->setCompressionImpl(cmp);
  DataForm *form = new DataForm(TypeResult);
  form->addField(DataFormField::TypeHidden, "FORM_TYPE", "urn:xmpp:dataforms:softwareinfo");
  form->addField(DataFormField::TypeNone, "os", utils::toStd(os));
  form->addField(DataFormField::TypeNone, "software", "qutIM");
  form->addField(DataFormField::TypeNone, "software_version", utils::toStd(qutimver));
  jClient->disco()->setForm(form);

	jClient->registerConnectionListener(this);
	jClient->registerMessageHandler(this);
	jClient->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, this);//LogLevelDebug,LogAreaXmlIncoming|LogAreaXmlOutgoing,this);
	jClient->registerPresenceHandler(this);
	jClient->registerSubscriptionHandler( this );
	jClient->registerIqHandler(this,ExtVersion);
	jClient->registerIqHandler(this,ExtUser+2);
	m_invitationHandler = new jInvitationHandler(jClient);
	jClient->registerMUCInvitationHandler(m_invitationHandler);
	connect(m_invitationHandler, SIGNAL(conferenceInvite(const JID&, const JID&, const QString&, const QString&)), this, SLOT(receiveInvite(const JID&, const JID&, const QString&, const QString&)));
	m_presence = Presence::Available;
	jClient->setPresence(m_presence, m_priority.value(m_presence));
	jClient->rosterManager()->registerRosterListener(this,false);
	jClient->disco()->registerDiscoHandler(this);
	m_vcardManager = new VCardManager(jClient);
	m_bookmark_storage = new BookmarkStorage(jClient);
	m_bookmark_storage->registerBookmarkHandler(this);
	//m_bookmark_menu = 0;
	m_pubsub_manager = new PubSub::Manager(jClient);
	m_privacy_manager = new PrivacyManager(jClient);
	m_privacy_manager->registerPrivacyListHandler(this);
	m_last_activity = new LastActivity(jClient);
	m_last_activity->registerLastActivityHandler(this);

	jClient->registerStanzaExtension(new VCardUpdate());
	jClient->registerStanzaExtension(new VersionExtension());
	jClient->registerStanzaExtension(new XPresenceExtension());
	jClient->registerStanzaExtension(new XStatusExtension());
	jClient->registerStanzaExtension(new Receipt(Receipt::Request));
	jClient->registerStanzaExtension(new PubSub::Event((Tag *)0));
	jClient->registerStanzaExtension(new ChatState(ChatStateActive));
	jClient->registerStanzaExtension(new DelayedDelivery(new Tag("")));
	jClient->registerStanzaExtension(new GMailExtension((Tag *)0));
	jClient->addPresenceExtension(new VCardUpdate(utils::toStd(m_avatar_hash)));
	m_last_gmail_date = 0;
	m_pep_support = false;
	m_ping_timer.setInterval(60000);
	connect(&m_ping_timer, SIGNAL(timeout()),this,SLOT(onTimer()));
	m_server_ping=0;
	m_jabber_roster->setRosterManager(jClient->rosterManager());
	m_file_transfer = new jFileTransfer(jClient);
	m_proxy_discovery = new jServiceDiscovery("proxy");
	connect(m_proxy_discovery, SIGNAL(finishSelfSearch(jDiscoItem*)), m_file_transfer, SLOT(searchSocks5Proxy(jDiscoItem*)));
	connect(m_proxy_discovery, SIGNAL(getDiscoInfo(QString,QString,DiscoHandler*)), this, SLOT(getDiscoInfo(QString,QString,DiscoHandler*)));
	connect(m_proxy_discovery, SIGNAL(getDiscoItem(QString,QString,DiscoHandler*)), this, SLOT(getDiscoItems(QString,QString,DiscoHandler*)));
}

void jProtocol::loadSettings()
{
	QSettings profile_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	m_show_mainres_notify = profile_settings.value("roster/showmainresnotify", true).toBool();
	m_auto_retrieve_vcards = profile_settings.value("main/getavatars",true).toBool();
	m_gmail_notify_type = profile_settings.value("xeps/gmailtype",0).toInt();
	QString default_resource = profile_settings.value("main/defaultresource", "qutIM").toString();

	QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
	account_settings.beginGroup("main");
	bool local_bookmark = account_settings.value("localbookmark",false).toBool();
	if (local_bookmark != m_local_bookmark)
	{
		m_local_bookmark = local_bookmark;
		requestBookmarks();
	}
	m_resource = account_settings.value("resource", default_resource).toString();
	m_priority.clear();
	m_priority.insert(Presence::Invalid, account_settings.value("priority", 30).toInt());
	if (account_settings.value("autopriority",true).toBool())
	{
		profile_settings.beginGroup("priority");
		m_priority.insert(Presence::Available, profile_settings.value("online", 30).toInt());
		m_priority.insert(Presence::Chat, profile_settings.value("online", 30).toInt());
		m_priority.insert(Presence::Away, profile_settings.value("away", 20).toInt());
		m_priority.insert(Presence::XA, profile_settings.value("na", 10).toInt());
		m_priority.insert(Presence::DND, profile_settings.value("dnd", 5).toInt());
		profile_settings.endGroup();
	}
	m_avatar_hash = account_settings.value("avatarhash","").toString();
	QString password = account_settings.value("password","").toString();
	m_my_nick = account_settings.value("nickname",m_account_name).toString();
	account_settings.endGroup();

	jClient->setResource(utils::toStd(m_resource));
	jClient->setPassword(utils::toStd(password));
	m_presence = jClient->presence().presence();
	jClient->setPresence(m_presence, m_priority.value(m_presence), jClient->presence().status());
}

jProtocol::~jProtocol()
{
	if (m_connection->state()!=StateDisconnected)
		jClient->disconnect();

	m_last_activity->removeLastActivityHandler();

	delete m_jabber_roster;
	delete m_conference_management_object;
	delete m_pubsub_manager;
	delete m_last_activity;
	delete jClient;
}

QString jProtocol::getBare(const QString &full)
{
	return full.section('/',0,0);
}

QString jProtocol::getResource(const QString &full)
{
	return full.section('/',1);
}

void jProtocol::setStatus(const Presence::PresenceType &presence, QString message)
{
	if (message.isEmpty())
		jClient->presence().resetStatus();
	int priority;
	m_presence = presence;
	if (m_priority.contains(m_presence))
		priority = m_priority.value(m_presence);
	else
		priority = m_priority.value(Presence::Invalid);
	jClient->setPresence(m_presence, priority, utils::toStd(message));
	if (m_presence != Presence::Unavailable)
	{
		if (m_connection->state()==StateDisconnected)
    {
//      jClient->setCompressionImpl(0);
//      jClient->setEncryptionImpl(0);
			m_connection->loadSettings();
			start();
			emit setRealStatus(-1);
		}
		else
			emit setRealStatus(m_presence);
	}
	else
	{
		m_mood_action->setEnabled(false);
		m_activity_action->setEnabled(false);
		emit setRealStatus(m_presence);
		m_connection->stopReconnect();
		m_conference_management_object->disconnectAll();
		jClient->disconnect();
	}
}

void jProtocol::run()
{
	jClient->connect(false);
}

void jProtocol::onConnect()
{
	m_state_online = true;
	TreeModelItem item;
	item.m_protocol_name = "Jabber";
	item.m_account_name = m_account_name;
	item.m_item_type = 2;
	m_jabber_account->getPluginSystem().setAccountIsOnline(item, true);
	emit setRealStatus(m_jabber_account->getGmailHackPresence());
	m_jids_auto_vcard.append(m_account_name);
	fetchVCard(m_account_name);
	m_conference_management_object->connectAll();
	requestBookmarks();
	JID server(jClient->jid().server());
	jClient->disco()->getDiscoInfo(server,"",this,100);
	m_server_disco_item = new jDiscoItem();
	m_server_disco_item->setJID(utils::fromStd(server.bare()));
	m_proxy_discovery->search(m_server_disco_item);
	m_privacy_default_created = false;
	utils::fromStd(m_privacy_manager->requestListNames());
	StreamHost host;
	host.host = utils::toStd(m_connection->getSocket()->localAddress().toString());
	host.jid = jClient->jid();
	host.port = jPluginSystem::instance().getSOCKS5ServerPort();
	m_file_transfer->replaceStreamHost(host);
	m_file_transfer->prependStreamHost(host);

	m_last_activity->resetIdleTimer();
	m_jabber_account->getEventHandler()->accountConnected(m_account_name);
}

void jProtocol::handleBookmarks(const BookmarkList &b_list, const ConferenceList &c_list)
{
	m_jabber_account->setRecentBookmarks(b_list, c_list);
	emit bookmarksHandled();
}

void jProtocol::onDisconnect(ConnectionError e)
{
	m_state_online = false;
	m_ping_timer.stop();
	TreeModelItem item;
	item.m_protocol_name = "Jabber";
	item.m_account_name = m_account_name;
	item.m_item_type = 2;
	if(e != ConnUserDisconnected && e != ConnNotConnected)
	{
		QString error_tr;
		switch(e)
		{
			case ConnStreamError:
				error_tr = tr("A stream error occured. The stream has been closed.");
				break;
			case ConnStreamVersionError:
				error_tr = tr("The incoming stream's version is not supported");
				break;
			case ConnStreamClosed:
				error_tr = tr("The stream has been closed (by the server).");
				break;
			case ConnProxyAuthRequired:
				error_tr = tr("The HTTP/SOCKS5 proxy requires authentication.");
				break;
			case ConnProxyAuthFailed:
				error_tr = tr("HTTP/SOCKS5 proxy authentication failed.");
				break;
			case ConnProxyNoSupportedAuth:
				error_tr = tr("The HTTP/SOCKS5 proxy requires an unsupported auth mechanism.");
				break;
			case ConnIoError:
				error_tr = tr("An I/O error occured.");
				break;
			case ConnParseError:
				error_tr = tr("An XML parse error occurred.");
				break;
			case ConnConnectionRefused:
				error_tr = tr("The connection was refused by the server (on the socket level).");
				break;
			case ConnDnsError:
				error_tr = tr("Resolving the server's hostname failed.");
				break;
			case ConnOutOfMemory:
				error_tr = tr("Out of memory. Uhoh.");
				break;
			case ConnNoSupportedAuth:
				error_tr = tr("The auth mechanisms the server offers are not supported or the server offered no auth mechanisms at all.");
				break;
			case ConnTlsFailed:
				error_tr = tr("The server's certificate could not be verified or the TLS handshake did not complete successfully.");
				break;
			case ConnTlsNotAvailable:
				error_tr = tr("The server didn't offer TLS while it was set to be required or TLS was not compiled in.");
				break;
			case ConnCompressionFailed:
				error_tr = tr("Negotiating/initializing compression failed.");
				break;
			case ConnAuthenticationFailed:
				error_tr = tr("Authentication failed. Username/password wrong or account does not exist. Use ClientBase::authError() to find the reason.");
				break;
			case ConnUserDisconnected:
				error_tr = tr("The user (or higher-level protocol) requested a disconnect.");
				break;
			case ConnNotConnected:
				error_tr = tr("There is no active connection.");
				break;
			default:
				error_tr = tr("Unknown error. It is amazing that you see it... O_o");
				break;
		}
		emit systemNotification(m_account_name,error_tr);
	}
	m_jabber_account->getPluginSystem().setAccountIsOnline(item, false);
	m_jabber_roster->setOffline();
	emit setRealStatus(Presence::Unavailable);
	m_jabber_account->getEventHandler()->accountDisconnected(m_account_name);
//	m_gloox_timer->stop();
	m_conference_management_object->disconnectAll();
}

bool jProtocol::onTLSConnect(const CertInfo& info)
{
	return true;
}

void jProtocol::onResourceBindError(ResourceBindError error)
{
}

void jProtocol::onSessionCreateError(SessionCreateError error)
{
}

void jProtocol::handleItemSubscribed(const JID& jid)
{
}

void jProtocol::handleItemAdded(const JID& jid)
{
	RosterItem *item = jClient->rosterManager()->getRosterItem(jid);
	QString name = utils::fromStd(item->name());
	QString group;
	StringList g = item->groups();
	StringList::const_iterator it_g = g.begin();
	for(; it_g != g.end(); ++it_g)
		group = utils::fromStd(*it_g);
	if (group.isEmpty())
		group = "General";
	if ( !utils::fromStd(jid.bare()).contains("@") )
		group = tr("Services");
	m_jabber_roster->addContact(utils::fromStd(jid.bare()),name,group);
}

void jProtocol::handleItemUnsubscribed(const JID& jid)
{
}

void jProtocol::handleItemRemoved(const JID& jid)
{
	m_jabber_roster->removeContact(utils::fromStd(jid.bare()));
}

void jProtocol::handleItemUpdated(const JID& jid)
{
	RosterItem *item = jClient->rosterManager()->getRosterItem(jid);
	QString bare = utils::fromStd(jid.bare());
	jBuddy *buddy = m_jabber_roster->getBuddy(bare);
	if(!item || !buddy)
		return;
	QString name = utils::fromStd(item->name());
	QString group;
	StringList g = item->groups();
	StringList::const_iterator it_g = g.begin();
	for(; it_g != g.end(); ++it_g)
		group = utils::fromStd(*it_g);
	qDebug() << bare << buddy->getGroup() << group;
	if (group.isEmpty())
		group = "General";
	if ( !utils::fromStd(jid.bare()).contains("@") )
		group = tr("Services");
	if(buddy->getName()!=name)
		m_jabber_roster->renameContact(bare,name);
	if(buddy->getGroup()!=group)
		m_jabber_roster->moveContact(bare,group);
}

void jProtocol::handleRoster(const Roster& roster)
{
	m_jabber_roster->startLoadRoster();
	Roster::const_iterator it = roster.begin();
	for(; it != roster.end(); ++it)
	{
		QString group;
		StringList g = (*it).second->groups();
		StringList::const_iterator it_g = g.begin();
		for(; it_g != g.end(); ++it_g)
			group = utils::fromStd(*it_g);
		if (group.isEmpty())
			group = "General";
		if ( !utils::fromStd((*it).second->jid()).contains("@") )
			group = tr("Services");
		if (!m_jabber_roster->groupExist(group))
			m_jabber_roster->addGroup(group);
		jBuddy *buddy = m_jabber_roster->addContact(getBare(utils::fromStd((*it).second->jid())), utils::fromStd((*it).second->name()), group);
		m_last_activity->query(JID((*it).second->jid()));
		buddy->setSubscription((*it).second->subscription());
	}
	m_jabber_roster->stopLoadRoster();
}

void jProtocol::handleRosterError(Stanza *stanza)
{
}

void jProtocol::handlePresence (const Presence &presence)
{
	QString jid = utils::fromStd(presence.from().bare());
	QString resource = utils::fromStd(presence.from().resource());
	Presence::PresenceType presence_type = presence.presence();
	if(presence_type == Presence::Invalid)
		presence_type = Presence::Available;
	jBuddy *buddy = m_jabber_roster->getBuddy(jid);

	if(jid == m_account_name)
	{
		if (presence_type != Presence::Unavailable)
		{
			bool exist = m_jabber_roster->myConnectExist(resource);
			m_jabber_roster->addMyConnect(resource, presence.priority(), presence_type);
			m_jabber_roster->changeItemStatus(jid+"/"+resource, presence_type);
			if(!exist)
			{
				const Capabilities *caps = presence.capabilities();
				jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
				if(caps)
				{
					info->m_caps_ver = utils::fromStd(caps->ver());
					info->m_caps_node = utils::fromStd(caps->node());
				}
				else
				{
					info->m_caps_ver = "";
					info->m_caps_node = "";
				}
				if(resource!=m_resource)
				{
					jClientIdentification::instance().setClient(info,presence.from(),jClient,this);
				}
				else
				{
					info->m_client_name = utils::fromStd(jClient->disco()->name());
					info->m_client_version = utils::fromStd(jClient->disco()->version());
					info->m_client_os = utils::fromStd(jClient->disco()->os());
				}
				m_jabber_roster->setClient(jid, resource, info->m_client_name,true);
			}
		}
		else
			m_jabber_roster->delMyConnect(resource);
	}
	else
	{
		if(!m_jabber_roster->contactExist(jid))
			return;

		if (presence_type == Presence::Unavailable)
		{
			m_jabber_roster->delResource(jid, resource);
			m_last_activity->query(JID(utils::toStd(jid)));
		}
		else
		{
			m_jabber_roster->addResource(jid, resource, presence.priority(), presence_type);
			const Capabilities *caps = presence.capabilities();
			jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
			if(caps)
			{
				info->m_caps_ver = utils::fromStd(caps->ver());
				info->m_caps_node = utils::fromStd(caps->node());
			}
			else
			{
				info->m_caps_ver = "";
				info->m_caps_node = "";
			}
			if(info->m_client_name=="")
			{
				jClientIdentification::instance().setClient(info,presence.from(),jClient,this);
				m_jabber_roster->setClient(jid, resource, info->m_client_name);
			}

			if (m_jabber_roster->getMaxPriorityResource(jid) == resource)
				m_jabber_roster->changeItemStatus(jid, presence_type);

			m_jabber_roster->changeItemStatus(jid+"/"+resource, presence_type);
			buddy->m_last_activity_time.setTime_t(0);
			buddy->m_last_activity_message.clear();
		}
		m_jabber_roster->setStatusMessage(jid, resource, utils::fromStd(presence.status()));
	}
	if (buddy)
	{
		if(presence_type != Presence::Unavailable)
		{
	  const VCardUpdate *vcard_update = presence.findExtension<VCardUpdate>(ExtVCardUpdate);
			if(vcard_update)
			{
				QString hash = utils::fromStd(vcard_update->hash());
				if(buddy->getAvatarHash()!=hash)
				{
					if(hash.isEmpty() || QFile(m_jabber_account->getPathToAvatars()+"/"+hash).exists())
					{
						m_jabber_roster->setAvatar(jid,hash);
					}
					else
					{
						if(m_auto_retrieve_vcards)
						{
							m_jids_auto_vcard.append(jid);
							fetchVCard(jid);
						}
					}
				}
			}

	  const XStatusExtension *xstatus = presence.findExtension<XStatusExtension>(SExtXStatus);
			if(xstatus)
			{
				jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
				if(info)
					if (xstatus->status_id()>=0)
						info->m_x_status = QString("icq_xstatus%1").arg(xstatus->status_id());
					else
						info->m_x_status = "";
			}
			else
			{
				jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
				if (info)
					info->m_x_status = "";
			}

	  const XPresenceExtension *xpresence = presence.findExtension<XPresenceExtension>(SExtXPresence);
			if(xpresence)
			{
				jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
				if(info)
					if (xpresence->value() != -1)
						info->m_x_presence = jPluginSystem::instance().getXPresenceIconName(xpresence->value());
					else
						info->m_x_presence = "";
			}
			else
			{
				jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
				if (info)
					info->m_x_presence = "";
			}
		}
		if(buddy->getMaxPriorityResource()==resource)
		{
			m_jabber_roster->updateXIcon(jid, "status");
			m_jabber_roster->updateXIcon(jid, "presence");
		}
	}
}

void jProtocol::handleSubscription (const Subscription &s10n)
{
	QString jid = utils::fromStd(s10n.from().bare());
	jBuddy *buddy = m_jabber_roster->getBuddy(jid);
	if(!buddy)
	{
	const Nickname *nickname = s10n.findExtension<Nickname>(ExtNickname);
		buddy = m_jabber_roster->addContact(jid,nickname?utils::fromStd(nickname->nick()):"","");
	}
	RosterItem *item = jClient->rosterManager()->getRosterItem(s10n.from().bareJID());
	if(item)
	    buddy->setSubscription(item->subscription());
	QString text;
	switch( s10n.subtype() )
	{
		case Subscription::Subscribe:
			emit createAcceptAuthDialog(utils::fromStd(s10n.status()),s10n.from(),jClient);
			text = tr("Authorization request");
			break;
		case Subscription::Subscribed:
			text = tr("You were authorized");
			break;
		case Subscription::Unsubscribe:
			text = tr("Contacts's authorization was removed");
			break;
		case Subscription::Unsubscribed:
			text = tr("Your authorization was removed");
			break;
		default:
			return;
	}
	if(!s10n.status().empty())
	    text+="\n"+utils::fromStd(s10n.status());
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = m_account_name;
	contact.m_item_type = 0;
	contact.m_parent_name = buddy->getGroup();
	contact.m_item_name = jid;
	emit addMessageFrom(contact, QDateTime::currentDateTime(), text);
}

void jProtocol::handleRosterPresence(const RosterItem&/*item*/, const std::string&/*resource*/, Presence::PresenceType /*presence*/, const std::string &/*msg*/)
{
}

void jProtocol::handleSelfPresence(const RosterItem&/*item*/, const std::string& resource, Presence::PresenceType presence, const std::string &msg)
{
	if (utils::fromStd(resource) == m_resource)
	{
		emit setRealStatus(presence);
	}
}

void jProtocol::handleNonrosterPresence(const gloox::Presence&)
{
}

void jProtocol::handleRosterError(const gloox::IQ&)
{
}

bool jProtocol::handleSubscriptionRequest(const JID& jid, const std::string &msg)
{
	return false;
}

bool jProtocol::handleUnsubscriptionRequest(const JID& jid, const std::string &msg)
{
	return false;
}

void jProtocol::handleNonrosterPresence(Stanza* stanza)
{
}

void jProtocol::handleMessage(const Message &stanza, MessageSession * /*session*/)
{
	addMessageFrom(stanza.from(), stanza);
}

void jProtocol::addMessageFrom(const JID &from, const Message &stanza, bool conference)
{
	QString jid = utils::fromStd(from.bare());
	QString resource = utils::fromStd(from.resource());
	QString full = jid + (resource.isEmpty() ? "" : ("/"+resource));
	jBuddy *o_contact = m_jabber_roster->getBuddy(jid);
	if(!o_contact)
	{
	const Nickname *nickname = stanza.findExtension<Nickname>(ExtNickname);
		m_jabber_roster->addContact(jid,nickname?utils::fromStd(nickname->nick()):"","");
		o_contact = m_jabber_roster->getBuddy(jid);
	}

  const PubSub::Event *event = stanza.findExtension<PubSub::Event>(ExtPubSubEvent);
	if (event)
	{
		jPluginSystem& jps = jPluginSystem::instance();
		QList<QVariant> l;
		QString name;
		PubSub::Event::ItemOperationList items = event->items();
		foreach(const PubSub::Event::ItemOperation *item, items)
		{
		const Tag *tag = item->payload;
		if(tag == 0)
			continue;
		if (tag->hasChild("mood", "xmlns", "http://jabber.org/protocol/mood"))
		{
		    name = "mood";
		    tag = tag->findChild(utils::toStd(name));
		    MoodsExtenstion mood(tag);

		    l.append(mood.moodName());
		    l.append(mood.moodText());

			if(jid == m_account_name)
			{
			m_current_mood = mood.moodName();
			m_current_mood_text = mood.moodText();
			QIcon icon;
			if(!m_current_mood.isEmpty())
				icon = Icon(m_current_mood, "moods");
			m_mood_action->setIcon(icon);
		    }
		}
		else if (tag->hasChild("activity", "xmlns", "http://jabber.org/protocol/activity"))
		{
		    name = "activity";
		    tag = tag->findChild(utils::toStd(name));
		    ActivityExtension activity(tag);

		    l.append(activity.general());
		    l.append(activity.specific());
		    l.append(activity.text());

		    if(jid==m_account_name)
		    {
			m_current_activity.first = activity.general();
			m_current_activity.second = activity.specific();
			QIcon icon;
			if(!activity.general().isEmpty())
				icon = Icon(activity.specific().isEmpty()?"category":activity.specific(),"activity/"+activity.general());
			m_activity_action->setIcon(icon);
		    }
		}
		else if(tag->hasChild("tune", "xmlns", "http://jabber.org/protocol/tune"))
		{
		    name = "tune";
		    tag = tag->findChild(utils::toStd(name));
		    TuneExtension tune(tag);

		    l.append(tune.getArtist());
		    l.append(tune.getTitle());
		    l.append(tune.getSource());
		    l.append(tune.getTrack());
		    l.append(tune.getLength());
		    l.append(tune.getRating());
		    l.append(tune.getUri());
		}
	    }
	    if (!name.isEmpty())
	    {
		    o_contact->setPubsubInfo(name, l);
		    m_jabber_roster->updateIcon(jid, name);
	    }
	}

  const Receipt *receipt = stanza.findExtension<Receipt>(ExtReceipt);
	if(receipt && o_contact->resourceExist(resource))
	{
		Receipt::ReceiptType receipt_type = receipt->rcpt();//(ReceiptType)(receipt->rcpt()-1);
		if(receipt_type==Receipt::Request)
		{
			Message msg(Message::Chat, from);
			Receipt *receipt = new Receipt(Receipt::Received);
			msg.addExtension(receipt);
			msg.setID(stanza.id());
			jClient->send(msg);
		}
		else if(receipt_type==Receipt::Received)
		{
			QPair<QString,int> pair = m_receipts_hash.value(utils::fromStd(stanza.id()));
			m_receipts_hash.remove(utils::fromStd(stanza.id()));
			TreeModelItem contact;
			contact.m_protocol_name = "Jabber";
			contact.m_account_name = m_account_name;
			contact.m_item_name = pair.first;
			contact.m_item_type = 0;
			emit messageDelievered(contact, pair.second);
		}
	}

  const ChatState *chat_state = stanza.findExtension<ChatState>(ExtChatState);
	if(chat_state)
	{
		ChatStateType state = chat_state->state();
		TreeModelItem contact;
		contact.m_protocol_name = "Jabber";
		contact.m_account_name = m_account_name;
		contact.m_parent_name = o_contact->getGroup();
		contact.m_item_name = full;
		contact.m_item_type = 0;

		bool composing = state == ChatStateComposing;

		if(composing)
			m_jabber_roster->chatWindowAboutToBeOpened(utils::fromStd(from.full()));

		if (jid==m_account_name)
			contact.m_parent_name = "My connections";
		else
		{
			if (m_jabber_roster->chatWindowIsOpened(full))
				emit sendTypingNotification(contact, composing);
			if (m_show_mainres_notify && o_contact->getMaxPriorityResource() == resource)
				contact.m_item_name = jid;
		}
		emit sendTypingNotification(contact, composing);
	}

	if(!stanza.body().empty())
	{
		TreeModelItem contact;
		contact.m_protocol_name = "Jabber";
		contact.m_account_name = m_account_name;
		contact.m_parent_name = o_contact->getGroup();
		contact.m_item_type = conference ? 33 : 0;
		contact.m_item_name = full;

		if(contact.m_item_name==m_account_name)
			contact.m_parent_name = "My connections";

		QDateTime date;
		const DelayedDelivery * delayed_delievery = stanza.when();
		if(delayed_delievery)
			date = utils::fromStamp(delayed_delievery->stamp());
		else
			date = QDateTime::currentDateTime();

		m_jabber_roster->chatWindowAboutToBeOpened(full);
		if (!m_jabber_roster->chatWindowIsOpened(full) && o_contact->getMaxPriorityResource() == resource && m_show_mainres_notify)
			contact.m_item_name = jid;

		emit addMessageFrom(contact, date, utils::fromStd(stanza.body()));
	}
}

void jProtocol::sendMessageTo(const QString &contact_name, const QString& message, int message_icon_position)
{
	Message msg(Message::Chat, JID(utils::toStd(contact_name)), utils::toStd(message));
	QString bare = getBare(contact_name);
	QString resource = getResource(contact_name);
	jBuddy *buddy = m_jabber_roster->getBuddy(bare);
	if(buddy && buddy->getSubscription()!=S10nBoth)
	{
		Nickname *nick = new Nickname(utils::toStd(m_my_nick));
		msg.addExtension(nick);
	}
	std::string id = jClient->getID();
	msg.setID(id);
	QPair<QString,int> pair(contact_name,message_icon_position);
	m_receipts_hash.insert(utils::fromStd(id),pair);
	Receipt *receipt = new Receipt(Receipt::Request);
	msg.addExtension(receipt);
	jClient->send(msg);
	m_last_activity->resetIdleTimer();
}

void jProtocol::sendTypingNotification(const QString &item_name, int notification_type)
{
	ChatStateType state;
	if(notification_type==2)
		state = ChatStateComposing;
	else if(notification_type==0)
		state = ChatStateActive;
	else
		return;

	QString full = item_name;
	if(!m_conference_management_object->JIDIsRoom(getBare(item_name)))
	{
		jBuddy *buddy = m_jabber_roster->getBuddy(getBare(item_name));
		if(!buddy)
			return;

		QString resource = getResource(item_name);
		if (resource.isEmpty())
		{
			resource = buddy->getMaxPriorityResource();
			if(!resource.isEmpty())
				full += "/"+resource;
		}
		if(!buddy->resourceExist(resource))
			return;
	}

	Message msg(Message::Chat, utils::toStd(full));
	ChatState *chat_state = new ChatState(state);
	msg.addExtension(chat_state);
	jClient->send(msg);
}

bool jProtocol::handleIq (const IQ &iq)
{
  const VersionExtension *version = iq.findExtension<VersionExtension>(SExtVersion);
	if(version)
	{
		if(version->name().isNull())
			return true;
		if(m_conference_management_object->JIDIsRoom(utils::fromStd(iq.from().bare())))
		{
			QString conference = utils::fromStd(iq.from().bare());
//			QString full = utils::fromStd(iq.from().full());
			QString nick = utils::fromStd(iq.from().resource());
//			qDebug() << utils::fromStd(iq.from().full()) << version->name();
//			emit conferenceClientVersion("Jabber",utils::fromStd(iq.from().bare()),m_account_name,utils::fromStd(iq.from().resource()), version->name());
			m_conference_management_object->setClient(conference,nick,version);
		}
		else
		{
			if(version->name().isEmpty())
				return true;
			QString bare = utils::fromStd(iq.from().bare());
			//			QString full = utils::fromStd(iq.from().full());
			QString resource = utils::fromStd(iq.from().resource());
			if(!m_jabber_roster->contactExist(bare))
				return true;
			jBuddy *o_contact = m_jabber_roster->getBuddy(bare);
			jBuddy::ResourceInfo *info = o_contact->getResourceInfo(resource);
			jClientIdentification::instance().newInfo(version, info);

			m_jabber_roster->setClient(bare, resource, version->name(),bare==m_account_name);
		}
		return true;
	}
  const GMailExtension *gmail = iq.findExtension<GMailExtension>(SExtGmail);
	if(gmail)
	{
	    if(m_gmail_notify_type<0)
		return true;
		if(gmail->isNotification())
		{
			IQ iq(IQ::Get, JID(utils::toStd(m_account_name)), jClient->getID());
			iq.addExtension(new GMailExtension(m_last_gmail_date));
			jClient->send(iq);
		}
		else
		{
			if(gmail->getMailThreads().size()==0)
				return true;
			m_last_gmail_date = gmail->getResultTime();
			QString message;
			int num = 0;
			foreach(GMailExtension::MailThread mail, gmail->getMailThreads())
			{
			    num++;
				if(mail.senders.size()==1)
					message.append((tr("Sender: %2 <%1>")+"\n").arg(mail.senders.at(0).address).arg(mail.senders.at(0).name));
				else
				{
					message.append(tr("Senders: "));
					foreach(GMailExtension::Sender sender, mail.senders)
					{
						message.append((tr("%2 <%1>")+"\n").arg(sender.address).arg(sender.name));
					}
				}
				message.append(tr("Subject: %1").arg(mail.subject)+"\n");
				message.append(tr("URL: %1").arg(mail.url)+"\n\n");
			}
			if(!num)
			    return true;
			if(!message.isEmpty())
				message.append("------\n");
			message.append(tr("Unreaded messages: %1").arg(gmail->getTotalMatched()));
			TreeModelItem item;
			item.m_protocol_name = "Jabber";
			item.m_account_name = item.m_item_name = m_account_name;

			if(m_gmail_notify_type==0)
			{
			    item.m_item_type = 0;
			    emit addMessageFrom(item, QDateTime::currentDateTime(), message);
			}
			else if(m_gmail_notify_type==1)
			{
			    item.m_item_type = 0;
			    jPluginSystem::instance().customNotifiacation(item,message);
			}
		}
		return true;
	}
	return false;
}

void jProtocol::handleIqID (const IQ &/*iq*/, int /*context*/)
{

}

void jProtocol::getDiscoItems(const QString &jid, const QString &node, DiscoHandler *handler)
{
	/*int context;
	if (tid == "request-conference")
		context = 1000;
	else if (tid == "request-conference-item")
		context = 1001;*/
	DiscoHandler *dh;
	if (handler)
		dh = handler;
	else
		dh = this;
	jClient->disco()->getDiscoItems(JID(utils::toStd(jid)), utils::toStd(node), dh, 0);
}

void jProtocol::getDiscoInfo(const QString &jid, const QString &node, DiscoHandler *handler)
{
	/*int context;
	if (tid == "request-conference")
		context = 1000;
	else if (tid == "request-conference-item")
		context = 1001;*/
	DiscoHandler *dh;
	if (handler)
		dh = handler;
	else
		dh = this;
	jClient->disco()->getDiscoInfo(JID(utils::toStd(jid)), utils::toStd(node), dh, 0);
}

void jProtocol::handleDiscoInfo(const JID &from, const Disco::Info &info, int context)
{
	if(context==100)
	{
		m_pep_support = false;
		const Disco::IdentityList &identities = info.identities();
		Disco::IdentityList::const_iterator ilt = identities.begin();
		for( ; ilt != identities.end(); ++ilt )
		{
			if((*ilt)->category()=="pubsub" && (*ilt)->type()=="pep")
				m_pep_support = true;
		}
		m_mood_action->setEnabled(m_pep_support);
		m_activity_action->setEnabled(m_pep_support);
		m_server_features.clear();
		const StringList &sl = info.features();
		StringList::const_iterator it = sl.begin();
		for( ; it != sl.end(); ++it )
			m_server_features << utils::fromStd((*it));
		qSort(m_server_features);
		m_last_gmail_date = 0;
		if(hasFeature("google:mail:notify") && m_gmail_notify_type>-1)
		{
			IQ iq(IQ::Get, JID(utils::toStd(m_account_name)), jClient->getID());
			iq.addExtension(new GMailExtension(m_last_gmail_date));
			jClient->send(iq);
		}
		m_ping_timer.start();
	}
	/*else if (context == 1000 || context == 1001)
	{
		QString type;
		if (context == 1000)
			type = "request-conference";
		else if (context == 1001)
			type = "request-conference-item";
		emit receiveSDiscoInfo(from, info, type);
	}*/
	else
	{
		QString bare = utils::fromStd(from.bare());
		QString resource = utils::fromStd(from.resource());
		if(m_jabber_roster->contactExist(bare))
		{
			jBuddy *o_contact = m_jabber_roster->getBuddy(bare);
			jBuddy::ResourceInfo *resource_info = o_contact->getResourceInfo(resource);
			jClientIdentification::instance().newInfo(info,resource_info);
		}
	}
}

void jProtocol::handleDiscoItems(const JID &from, const Disco::Items &items, int context)
{
	/*QString type;
	if (context == 1000)
		type = "request-conference";
	else if (context == 1001)
		type = "request-conference-item";
	emit receiveSDisco(from, items, type);*/
}

void jProtocol::handleDiscoError(const JID &/*from*/, const Error * /*error*/, int /*context*/)
{
}

bool jProtocol::handleDiscoSet(const IQ &iq)
{
	return false;
}

QStringList jProtocol::getAdditionalInfoAboutContact(const QString &item_name)
{
	QString node = item_name;
	QString resource;
	if(item_name.indexOf("/")>-1)
	{
		node = getBare(item_name);
		resource = getResource(item_name);
	}

	if(m_conference_management_object->JIDIsRoom(node))
	{
		return m_conference_management_object->getAdditionalInfoAboutContact(node,resource);
	}
	else
	{
		jBuddy *buddy = m_jabber_roster->getBuddy(node);
		if(!buddy)
			return QStringList();
		if(resource.isEmpty() || resource.isNull())
		{
			resource = buddy->getMaxPriorityResource();
		}
		QStringList list_info;
		QString name;
		if ( buddy->getName().isEmpty() )
			name = node;
		else
			name = buddy->getName();
		list_info << ((item_name.indexOf("/")>-1)?name+"/"+resource:name);
		QString hash = buddy->getAvatarHash();
		if(hash.isEmpty())
			list_info << QString();
		else
			list_info << m_jabber_account->getPathToAvatars()+"/"+buddy->getAvatarHash();
		jBuddy::ResourceInfo * info = buddy->getResourceInfo(resource);
		if(!info)
			return list_info;
		list_info << (info->m_client_name + " " + info->m_client_version + (info->m_client_os.isEmpty()?"":(" / " + info->m_client_os)));
		list_info << resource;
		return list_info;
	}
	return QStringList();
}

void jProtocol::fetchVCard(const QString &jid, bool is_auto)
{
	if(is_auto)
		m_jids_auto_vcard.append(jid);
	const JID &jid_tmp = JID(utils::toStd(jid));
	m_vcardManager->fetchVCard(JID(utils::toStd(jid)), this);
}

void jProtocol::handleVCard(const JID &jid, const VCard *fetchedVCard)
{
	QString full = utils::fromStd(jid.full());
	QString avatarUrl = "";
	QString hex = "";
	const VCard *vcard = (!fetchedVCard) ? new VCard() : fetchedVCard;
	QString str_date = utils::fromStd(vcard->bday());
	bool isValidDate = QDate::fromString(str_date, "yyyy-MM-dd").isValid();
	if (isValidDate) // valid date
	{
		QDate m_date = QDate::fromString(str_date, "yyyy-MM-dd");
		m_jabber_roster->setBirthday(jid, m_date);
	}
	const VCard::Photo & photo = vcard->photo();
	if(!photo.binval.empty())
	{
		QByteArray data(photo.binval.c_str(),photo.binval.length());
		SHA sha;
		sha.feed(photo.binval);
		sha.finalize();
		hex = utils::fromStd(sha.hex());
		QDir dir(m_jabber_account->getPathToAvatars());
		if(!dir.exists())
			dir.mkpath(dir.absolutePath());
		avatarUrl = m_jabber_account->getPathToAvatars()+"/"+hex;
		QFile file(m_jabber_account->getPathToAvatars()+"/"+hex);
		if(file.open(QIODevice::WriteOnly))
		{
			file.write(data);
			file.close();
			if(utils::fromStd(jid.bare())==m_account_name)
			{
				/*if(m_avatar_hash!=hex)
				{
					m_avatar_hash=hex;
					StanzaExtensionList *extensions = (StanzaExtensionList *)&(jClient->presence().extensions());
					StanzaExtensionList::iterator it = extensions->begin();
					StanzaExtensionList::iterator it2;
					while( it != extensions->end() )
					{
						it2 = it++;
						if((*it2)->extensionType()==ExtVCardUpdate)
						{
							delete (*it2);
							extensions->erase(it2);
						}
					}
					jClient->presence().addExtension(new VCardUpdate(utils::toStd(m_avatar_hash)));
					QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
					account_settings.setValue("main/avatarhash",hex);
				}*/
			}
			else
				setAvatar(jid,hex);
		}
	}
	else if(!photo.extval.empty())
	{
	}
	else
	{
		setAvatar(jid,"");
	}
	if(utils::fromStd(jid.bare())==m_account_name)
	{
		updateAvatarPresence(hex);
		QString my_nick = utils::fromStd(vcard->nickname());
		if(my_nick.isEmpty())
			my_nick = m_account_name;
		if(m_my_nick!=my_nick)
		{
			m_my_nick = my_nick;
			QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
			account_settings.setValue("main/nickname",m_my_nick);
		}
	}
	if(!m_jids_auto_vcard.contains(full))
	{
		VCard *tmp_vcard = new VCard(vcard->tag());
		emit onFetchVCard(utils::fromStd(jid.full()), tmp_vcard, avatarUrl);
	}
	else
		m_jids_auto_vcard.removeAll(full);
}

void jProtocol::updateAvatarPresence(const QString &hash)
{
	if(m_avatar_hash!=hash)
	{
		if(m_avatar_hash.isEmpty() && hash.isEmpty())
			return;
		m_avatar_hash=hash;
		StanzaExtensionList *extensions = (StanzaExtensionList *)&(jClient->presence().extensions());
		StanzaExtensionList::iterator it = extensions->begin();
		StanzaExtensionList::iterator it2;
		while( it != extensions->end() )
		{
			it2 = it++;
			if((*it2)->extensionType()==ExtVCardUpdate)
			{
				delete (*it2);
				extensions->erase(it2);
			}
		}
		jClient->addPresenceExtension(new VCardUpdate(utils::toStd(m_avatar_hash)));
		QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
		account_settings.setValue("main/avatarhash",hash);
		m_conference_management_object->sendPresenceToAll();
		jClient->setPresence();
	}
}

void jProtocol::handleVCardResult(VCardContext context, const JID &jid, StanzaError se)
{
	if (context == StoreVCard && se == StanzaErrorUndefined)
		emit systemNotification(m_account_name, tr("vCard is succesfully saved"));
}

void jProtocol::storeVCard(VCard *vcard)
{
	m_vcardManager->storeVCard(vcard, this);
}

void jProtocol::setAvatar(const JID &jid, const QString &hash)
{
	QString node = utils::fromStd(jid.bare());
	if(m_conference_management_object->JIDIsRoom(node))
	{
		QString resource = utils::fromStd(jid.resource());
		if (hash.isEmpty())
			m_conference_management_object->setAvatar(node,resource,"");
		else
			m_conference_management_object->setAvatar(node,resource,m_jabber_account->getPathToAvatars()+"/"+hash);
	}
	else
	{
		m_jabber_roster->setAvatar(node,hash);
	}
}

void jProtocol::moveContact(const QString &item_name, const QString &parent_name)
{
	RosterItem *item = jClient->rosterManager()->getRosterItem(JID(utils::toStd(item_name)));
	if(!item)
		return;
	if(item_name.indexOf('/')>-1)
		return;
	QString group = parent_name;
	if(group.isEmpty())
		return;
	if(group=="General")
		group="";
	StringList groups;
	groups.push_back(utils::toStd(group));
	item->setGroups(groups);
	jClient->rosterManager()->synchronize();
}

void jProtocol::addContact(const QString &jid, QString name, const QString &group, bool authorize)
{
	if (jid.isEmpty())
		return;
	JID to = JID(utils::toStd(jid));
	if (name.isEmpty())
		name = jid;
	StringList groups;
	groups.push_back(utils::toStd(group));
	if (authorize)
		jClient->rosterManager()->subscribe(to, utils::toStd(name), groups, utils::toStd("Please authorize me and add to your contacr list"));
	else
		jClient->rosterManager()->add(to, utils::toStd(name), groups);
	jClient->rosterManager()->synchronize();
}

void jProtocol::handleLog(LogLevel level, LogArea area, const std::string &message)
{
	static bool *debug = 0;
	if(!debug)
	{
		QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim", "qutimsettings");
		debug = new bool(settings.value("debug/jabber", true).toBool());
	}
	bool in;
	if(area == LogAreaXmlIncoming)
		in = true;
	else if(area == LogAreaXmlOutgoing)
		in = false;
	else if(*debug)
	{
		QString number = QString::number(area, 16);
		while(number.length() < 4)
			number.prepend('0');
		switch(level)
		{
		case LogLevelError:
			qCritical("0x%s: \"%s\"", qPrintable(number), qPrintable(utils::fromStd(message)));
			break;
		case LogLevelWarning:
			qWarning("0x%s: \"%s\"", qPrintable(number), qPrintable(utils::fromStd(message)));
			break;
		case LogLevelDebug:
		default:
			qDebug("0x%s: \"%s\"", qPrintable(number), qPrintable(utils::fromStd(message)));
			break;
		}
		return;
	}
	QString xml = utils::fromStd(message);
//	qDebug() << (in?"> ":"< ") << xml;
	emit tagHandled(xml, in);
}

//Debug: <  "<iq id='uid193' type='set' from='euroelessar@jabber.ru/qutIM' xmlns='jabber:client'><pubsub xmlns='http://jabber.org/protocol/pubsub'><publish node='http://jabber.org/protocol/tune'><item><tune xmlns='http://jabber.org/protocol/tune'><artist>Flёur</artist><length>253</length><rating>0</rating><source>Два Облака (сингл)</source><title>Тёплые коты</title><track>2</track><uri>file:///mnt/sdb13/%D0%9C%D1%83%D0%B7%D1%8B%D0%BA%D0%B0/Fl%D1%91ur/2007%20-%20%D0%94%D0%B2%D0%B0%20%D0%9E%D0%B1%D0%BB%D0%B0%D0%BA%D0%B0%20(%D1%81%D0%B8%D0%BD%D0%B3%D0%BB)/02%20-%20%D0%A2%D1%91%D0%BF%D0%BB%D1%8B%D0%B5%20%D0%BA%D0%BE%D1%82%D1%8B.mp3</uri></tune></item></publish></pubsub></iq>"
//Debug: >  "<iq from='euroelessar@jabber.ru' to='euroelessar@jabber.ru/qutIM' id='uid193' type='error'><error code='403' type='auth'><forbidden xmlns='urn:ietf:params:xml:ns:xmpp-stanzas'/></error><pubsub xmlns='http://jabber.org/protocol/pubsub'><publish node='http://jabber.org/protocol/tune'><item><tune xmlns='http://jabber.org/protocol/tune'><artist>Flёur</artist><length>253</length><rating>0</rating><source>Два Облака (сингл)</source><title>Тёплые коты</title><track>2</track><uri>file:///mnt/sdb13/%D0%9C%D1%83%D0%B7%D1%8B%D0%BA%D0%B0/Fl%D1%91ur/2007%20-%20%D0%94%D0%B2%D0%B0%20%D0%9E%D0%B1%D0%BB%D0%B0%D0%BA%D0%B0%20(%D1%81%D0%B8%D0%BD%D0%B3%D0%BB)/02%20-%20%D0%A2%D1%91%D0%BF%D0%BB%D1%8B%D0%B5%20%D0%BA%D0%BE%D1%82%D1%8B.mp3</uri></tune></item></publish></pubsub></iq>"


void jProtocol::setActions(/*QAction *bookmark, */QAction *mood, QAction *activity)
{
	//m_bookmark_action = bookmark;
	m_mood_action = mood;
	connect(m_mood_action,SIGNAL(triggered()),this,SLOT(onSetMood()));
	m_activity_action = activity;
	//m_activity_action->setEnabled(true);
	connect(m_activity_action,SIGNAL(triggered()),this,SLOT(onSetActivity()));
}

void jProtocol::onSetMood()
{
	CustomStatusDialog dialog(m_account_name,m_profile_name);
	dialog.setStatuses(m_current_mood, m_current_mood_text);
	dialog.show();
	if(dialog.exec()==QDialog::Accepted)
	{
		QStringList list;
		list << dialog.current_mood << dialog.status_message;
		setMood(list);
	}
}

void jProtocol::setMood(const QStringList &list)
{
	MoodsExtenstion *mood = new MoodsExtenstion(list.at(0), list.at(1));
	PubSub::ItemList items;
	Tag *tag = new Tag("item");
	tag->addChild(mood->tag());
	items.push_back(new  PubSub::Item(tag));
	m_pubsub_manager->publishItem(JID(), "http://jabber.org/protocol/mood", items, 0, this);
	delete mood;
}

void jProtocol::onSetActivity()
{
	ActivityDialog dialog(m_account_name,m_profile_name);
	dialog.setActivity(m_current_activity.first, m_current_activity.second);
	dialog.show();
	if(dialog.exec()==QDialog::Accepted)
	{
		QStringList list;
		list << dialog.getGeneral() << dialog.getSpecific() << dialog.getText();
		setActivity(list);
	}
}

void jProtocol::setActivity(const QStringList &list)
{
	ActivityExtension *activity = new ActivityExtension(list.at(0), list.at(1), list.at(2));
	PubSub::ItemList items;
	Tag *tag = new Tag("item");
	tag->addChild(activity->tag());
	items.push_back(new  PubSub::Item(tag));
	m_pubsub_manager->publishItem(JID(), "http://jabber.org/protocol/activity", items, 0, this);
	delete activity;
}

void jProtocol::setTune(const QStringList &list)
{
	TuneExtension *tune = new TuneExtension();
	tune->setArtist(list.at(0));
	tune->setLength(list.at(1).toInt());
	tune->setRating(list.at(2).toInt());
	tune->setSource(list.at(3));
	tune->setTitle(list.at(4));
	tune->setTrack(list.at(5));
	tune->setUri(list.at(6));
	PubSub::ItemList items;
	Tag *tag = new Tag("item");
	tag->addChild(tune->tag());
	items.push_back(new  PubSub::Item(tag));
	m_pubsub_manager->publishItem	(JID(), "http://jabber.org/protocol/tune", items, 0, this);
	delete tune;
}

void jProtocol::onTimer()
{
    jClient->whitespacePing();
}

void jProtocol::handleEvent (const gloox::Event &/*event*/)
{
}

void jProtocol::receiveInvite(const JID &room, const JID &from, const QString &reason, const QString &password)
{
	emit conferenceInvite(room, from, reason, password);
}

void jProtocol::handlePrivacyListNames (const std::string &active, const std::string &def, const StringList &lists)
{
	m_privacy_list.clear();
	m_privacy_items.clear();
	foreach (std::string list, lists)
		m_privacy_list << utils::fromStd(list);
	m_store_count = 0;
	m_privacy_active = utils::fromStd(active);
	if (!m_privacy_default_created)
	{
		if (!m_privacy_list.contains("invisible"))
		{
			QList<PrivacyItem> invisible_for_all;
			invisible_for_all << PrivacyItem(PrivacyItem::TypeUndefined, PrivacyItem::ActionDeny, PrivacyItem::PacketPresenceOut);
			m_store_count++;
			storePrivacyList("invisible", invisible_for_all.toStdList());
		}
		if (!m_privacy_list.contains("visible"))
		{
			QList<PrivacyItem> visible_for_all;
			visible_for_all << PrivacyItem(PrivacyItem::TypeUndefined, PrivacyItem::ActionAllow, PrivacyItem::PacketPresenceOut);
			m_store_count++;
			storePrivacyList("visible", visible_for_all.toStdList());
		}
		if (!m_privacy_list.contains("visible list"))
		{
			QList<PrivacyItem> visible_list;
			visible_list << PrivacyItem(PrivacyItem::TypeUndefined, PrivacyItem::ActionDeny, PrivacyItem::PacketPresenceOut);
			m_store_count++;
			storePrivacyList("visible list", visible_list.toStdList());
		}
		if (!m_privacy_list.contains("invisible list"))
		{
			QList<PrivacyItem> invisible_list;
			invisible_list << PrivacyItem(PrivacyItem::TypeUndefined, PrivacyItem::ActionAllow, PrivacyItem::PacketPresenceOut);
			m_store_count++;
			storePrivacyList("invisible list", invisible_list.toStdList());
		}
	}
	m_privacy_default_created = true;
	if (!m_store_count)
		getListItem();
}

void jProtocol::handlePrivacyList (const std::string &name, const PrivacyList &items)
{
	m_list_count--;
	m_privacy_items.insert(utils::fromStd(name), QList<PrivacyItem>::fromStdList(items));
	if (!m_list_count)
		emit setPrivacyLists(m_privacy_list, m_privacy_active);
}

void jProtocol::handlePrivacyListChanged (const std::string &name)
{
	//m_privacy_list << utils::fromStd(name);
	//m_privacy_items.remove(utils::fromStd(name));
	if (m_privacy_items.contains(utils::fromStd(name)))
		return;
	if (m_store_count)
		m_store_count--;
	if (!m_store_count)
		utils::fromStd(m_privacy_manager->requestListNames());
		//getListItem();
}

void jProtocol::handlePrivacyListResult (const std::string &id, PrivacyListResult plResult)
{

}

void jProtocol::getListItem(const QString &p_list)
{
	m_list_count = 0;
	if (p_list.isEmpty())
	{
		foreach(QString list, m_privacy_list)
			if (!m_privacy_items.contains(list))
			{
				m_list_count++;
				m_privacy_manager->requestList(utils::toStd(list));
			}
	}
	else
	{
		m_privacy_list.removeOne(p_list);
		m_list_count++;
		m_privacy_manager->requestList(utils::toStd(p_list));
	}
}

void jProtocol::setPrivacyStatus(QString &list)
{
	if (m_privacy_active == list)
		return;
	Presence::PresenceType presence = m_presence;
	jClient->setPresence(Presence::Unavailable, m_priority[presence]);
	m_privacy_manager->setActive(utils::toStd(list));
	m_privacy_manager->setDefault(utils::toStd(list));
	jClient->setPresence(presence, m_priority[presence]);
	m_privacy_active = list;
}

void jProtocol::storePrivacyList(const QString &list, PrivacyList items)
{
	if (m_privacy_items.contains(list))
		m_privacy_items.insert(list, QList<PrivacyItem>::fromStdList(items));
	m_privacy_manager->store(utils::toStd(list), items);
}

void jProtocol::showTransportReg(const QString &jid)
{
	jTransport *tran = new jTransport(jClient, jid);
	tran->addRegistration();
}

void jProtocol::removeTransportReg(const QString &jid)
{
	jTransport *tran = new jTransport(jClient, jid);
	tran->removeRegistration();
}

void jProtocol::LogInTransport(const QString &jid)
{
    Presence pres(jClient->presence().presence(), JID(utils::toStd(jid)));
    jClient->send(pres);
}

void jProtocol::LogOutTransport(const QString &jid)
{
    Presence pres(Presence::Unavailable, JID(utils::toStd(jid)));
    jClient->send(pres);
}

void jProtocol::handleLastActivityResult (const JID &jid, long seconds, const std::string &message)
{
	QString sjid = utils::fromStd(jid.full());
	QString bare = getBare(sjid);
	QString resource = getResource(sjid);
	if (!resource.isEmpty())
		emit systemNotification(m_account_name, tr("JID: %1<br/>Idle: %2").arg(sjid).arg(utils::timeToString(seconds)));
	else
		if (!m_jabber_roster->getBuddy(bare)->getCountResources())
		{
			m_jabber_roster->getBuddy(bare)->m_last_activity_time.setTime_t(QDateTime::currentDateTime().toTime_t()-seconds);
			m_jabber_roster->getBuddy(bare)->m_last_activity_message = utils::fromStd(message);
		}
}

void jProtocol::handleLastActivityError (const JID &jid, StanzaError error)
{
	QString sjid = utils::fromStd(jid.full());
	QString bare = getBare(sjid);
	QString resource = getResource(sjid);
	if (!resource.isEmpty())
	{
		if (error == StanzaErrorFeatureNotImplemented)
			emit systemNotification(m_account_name, tr("JID: %1<br/>The feature requested is not implemented by the recipient or server.").arg(sjid));
		else if (error = StanzaErrorForbidden)
			emit systemNotification(m_account_name, tr("JID: %1<br/>The requesting entity does not possess the required permissions to perform the action.").arg(sjid));
		else
			emit systemNotification(m_account_name, tr("JID: %1<br/>It is unknown StanzaError! Please notify developers.<br/>Error: %2").arg(sjid).arg(QString::number(error)));
	}
}

void jProtocol::reconnecting()
{
	emit setRealStatus(-1);
}

void jProtocol::requestBookmarks()
{
	m_jabber_account->clearRecentBookmarks();
	if (!m_state_online)
		return;
	if (m_local_bookmark)
	{
		m_jabber_account->setRecentBookmarks(BookmarkList(), m_jabber_account->getRecentBookmarks(true).toStdList());
		emit bookmarksHandled();
	}
	else
		m_bookmark_storage->requestBookmarks();
}
