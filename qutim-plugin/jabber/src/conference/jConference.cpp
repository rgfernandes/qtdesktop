/*
	jConference

	Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>
						2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
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

#include "jConference.h"
#include "src/jPluginSystem.h"
#include "src/jAccount.h"
#include "src/jProtocol.h"
#include "src/jClientIdentification.h"
#include "src/jConferenceConfig.h"
#include "src/jConferenceParticipant.h"
#include "reasondialog.h"
#include "utils.h"
#include <gloox/vcardupdate.h>
#include <QUrl>
#include <QMessageBox>

jConference::Room::~Room()
{
	if(entity)
		delete entity;
	if(config)
		delete config.data();
	if(participant)
		delete participant.data();
}

jConference::jConference(jProtocol *real_parent, Client *client, jAccount *plugin_system, QObject *parent) :
		QObject(parent),
		m_jabber_account(plugin_system),
		m_client_base(client),
		m_real_parent(real_parent),
		m_plugin_system(jPluginSystem::instance())
{
	m_account_name = utils::fromStd(client->jid().bare());
	m_presence = &client->presence();


	m_context_menu = new QMenu();
	m_menu_label = new QLabel;
	m_menu_label->setAlignment(Qt::AlignCenter);
	m_menu_title = new QWidgetAction(this);
	m_menu_title->setDefaultWidget(m_menu_label);
	m_kick_user = new QAction(tr("Kick"), this);
	m_ban_user = new QAction(tr("Ban"), this);
	m_role_user = new QActionGroup(this);
	m_user_visitor = new QAction(tr("Visitor"), m_role_user);
	m_user_participant = new QAction(tr("Participant"), m_role_user);
	m_user_moderator = new QAction(tr("Moderator"), m_role_user);
	m_user_visitor->setCheckable(true);
	m_user_participant->setCheckable(true);
	m_user_moderator->setCheckable(true);
	connect(m_kick_user, SIGNAL(triggered()), this, SLOT(kickUser()));
	connect(m_ban_user, SIGNAL(triggered()), this, SLOT(banUser()));
	connect(m_user_visitor, SIGNAL(triggered()), this, SLOT(setVisitorUser()));
	connect(m_user_participant, SIGNAL(triggered()), this, SLOT(setParticipantUser()));
	connect(m_user_moderator, SIGNAL(triggered()), this, SLOT(setModeratorUser()));


	m_join_form = 0;

	//	m_client_base->registerPresenceHandler(this);
}

jConference::~jConference()
{
}

void jConference::loadSettings()
{
	QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_jabber_account->getProfileName()+"/jabber."+m_account_name, "accountsettings");
	m_auto_retrieve_vcards = account_settings.value("main/getavatars",true).toBool();

}

void jConference::joinGroupchat()
{
	joinGroupchat("", "", "", true);
}

void jConference::joinGroupchat(const QString &room, const QString &nick, const QString &password, bool showBookmarks)
{
	m_join_form = new jJoinChat(m_jabber_account, room, nick, password, showBookmarks);
	m_join_form->setWindowTitle(QObject::tr("Join groupchat on") + " " + m_jabber_account->getAccountName());
	m_join_form->setWindowIcon(Icon("chat"));
	m_join_form->show();
	connect(m_join_form, SIGNAL(createConferenceRoom(QString, QString, QString, MUCRoom::HistoryRequestType, QString)), this, SLOT(s_createConferenceRoom(QString, QString, QString, MUCRoom::HistoryRequestType, QString)));
	connect(m_join_form, SIGNAL(destroyed()), this, SLOT(destroyJoinGroupChat()));
}

void jConference::destroyJoinGroupChat()
{
	m_join_form = 0;
}

void jConference::s_conferenceInvite(const JID &room, const JID &from, const QString &reason_sent, const QString &password)
{
	QString reason = reason_sent;
	if (reason.isEmpty())
		reason = "no reason";
	if (QMessageBox(QMessageBox::Question, tr("Invite to groupchat"), tr("User %1 invite you\nto conference %2\nwith reason \"%3\"\nAccept invitation?").arg(utils::fromStd(from.bare())).arg(utils::fromStd(room.full())).arg(reason), QMessageBox::Yes | QMessageBox::No).exec() == QMessageBox::Yes)
		joinGroupchat(utils::fromStd(room.full()), "", password, true);
}

/*void jConference::createConferenceRoom(const QString &host, const QString &room,
		const QString &nick, const QString &password)
{
	QString conference = QString("%1@%2").arg(room).arg(host);
	if(m_room_list.contains(conference))
	{
		MUCRoom *room = m_room_list[conference];
		if ( !password.isEmpty() )
			room->setPassword(utils::toStd(password));
			room->join(m_presence->subtype(), m_presence->status());
			QHash<QString, MucContact> *contacts = m_contacts_list.value(conference,0);
			if(!contacts)
				return;
			QStringList nicknames = contacts.keys();
			foreach(QString nick, nicknames)
			{
				MucContact contact = contacts.take(nick);
				emit removeConferenceItem("Jabber", conference, m_account_name, nick);
			}
		return;
	}
	//m_jabber_account->getPluginSystem().
	emit createConference("Jabber", conference, m_account_name );
	JID jid(utils::toStd(room) + "@" + utils::toStd(host) + "/" + utils::toStd(nick));
	MUCRoom *new_room = new MUCRoom(m_client_base,jid,this);
	m_client_base->registerPresenceHandler(jid, this);
	new_room->registerMUCRoomConfigHandler(this);
	if ( !password.isEmpty() )
	{
		new_room->setPassword(utils::toStd(password));
	}

	m_room_list.insert(conference, new_room);
	m_contacts_list.insert(conference, new QHash<QString,MucContact>());
	//m_jabber_account->getPluginSystem().
	emit changeOwnConferenceNickName("Jabber",
	conference, m_account_name, utils::fromStd(new_room->nick()));
	new_room->join(m_presence->subtype(), m_presence->status());
	m_jabber_account->getJabberRoster()->joinConference(room+"@"+host);
//	sendPresence(conference);
}*/

void jConference::handleMUCParticipantPresence (MUCRoom * /*room*/, const MUCRoomParticipant /*participant*/, const Presence &/*presence*/)
{
}

void jConference::handleMUCMessage (MUCRoom *room, const Message &msg, bool privateMessage)
{
	if ( privateMessage )
	{
		JID jid(room->name() + "@" + room->service() + "/" + msg.from().resource());
		m_real_parent->addMessageFrom(jid, msg, true);
	}
	else
	{
		const DelayedDelivery* dd = msg.when();
		if(msg.from().resource().empty())
			emit addSystemMessageToConference("Jabber",utils::fromStd(room->name() +
																	  "@" + room->service()),m_account_name,utils::fromStd(msg.body()),
											  dd==0?QDateTime::currentDateTime():utils::fromStamp(dd->stamp()),dd==0);
		else
			emit addMessageToConference(utils::fromStd(room->name() +
													   "@" + room->service()), m_account_name,utils::fromStd(msg.from().resource()),
										utils::fromStd(msg.body()), dd==0?QDateTime::currentDateTime():utils::fromStamp(dd->stamp()), dd!=0);
	}
}

bool jConference::handleMUCRoomCreation (MUCRoom *room)
{
	qDebug() << "handleMUCRoomCreation" << room;
	if(room)
	{
		createConfigDialog(utils::fromStd(room->name() + "@" + room->service()));
		qDebug()<<"connected";
		return true;
	}
	qDebug()<<"not connected";
	return false;

}

void jConference::handleMUCSubject (MUCRoom *room_entity, const std::string &nick, const std::string &subject)
{
	QString conference = utils::fromStd(room_entity->name()+"@"+room_entity->service());
	Room *room = m_rooms.value(conference);
	if(!room)
		return;
	QString nickname = utils::fromStd(nick);
	QString topic = utils::fromStd(subject);
	if(!nickname.isEmpty())
		emit addSystemMessageToConference("Jabber",conference,m_account_name,tr("%1 has set the subject to:\n%2").arg(nickname).arg(topic),QDateTime(),false);
	else
		emit addSystemMessageToConference("Jabber",conference,m_account_name,tr("The subject is:\n%2").arg(topic),QDateTime(),false);
	room->topic = topic;
	topic.replace("\n"," | ");
	emit setConferenceTopic("Jabber",conference,m_account_name,topic);
}

void jConference::handleMUCInviteDecline (MUCRoom * /*room*/, const JID &/*invitee*/, const std::string &/*reason*/)
{
}

void jConference::handleMUCError (MUCRoom *room_entity, StanzaError error)
{
	/*	PresenceJidHandlerList::const_iterator itj = m_client_base->m_presenceJidHandlers.begin();
	for( ; itj != m_client_base->m_presenceJidHandlers.end(); ++itj )
	{
		qDebug() << utils::fromStd((*itj).jid->bare());
		if( (*itj).jid->bare() == pres.from().bare() && (*itj).ph )
		{
			(*itj).ph->handlePresence( pres );
			match = true;
		}
	}*/
	QString conference_name = utils::fromStd(room_entity->name()+"@"+room_entity->service());
	Room *room = m_rooms.value(conference_name);
	if(!room)
		return;
	//	JID jid(room->name()+"@"+room->service());
	//	m_client_base->removePresenceHandler( jid, this);//(PresenceHandler *)room );
	//m_client_base->removePresenceHandler( (PresenceHandler *)room );
	/*if ( m_room_list.contains(conference_name))
	{
		MUCRoom *tmp_room = m_room_list.value(conference_name);
		qDebug() << "tmp_room" << tmp_room;
		m_room_list.remove(m_room_list.key(tmp_room));
		tmp_room->leave();
		delete tmp_room;
		delete static_cast<QHash<QString,jBuddy::ResourceInfo> *>(m_contacts_list[conference_name]);
		m_contacts_list.remove(conference_name);
	}*/

	bool nnr = false;
	QString error_tr;
	switch(error)
	{
	case StanzaErrorNotAuthorized:
		error_tr=tr("Not authorized: Password required.");
		break;
	case StanzaErrorForbidden:
		error_tr=tr("Forbidden: Access denied, user is banned.");
		break;
	case StanzaErrorItemNotFound:
		error_tr=tr("Item not found: The room does not exist.");
		break;
	case StanzaErrorNotAllowed:
		error_tr=tr("Not allowed: Room creation is restricted.");
		break;
	case StanzaErrorNotAcceptable :
		error_tr=tr("Not acceptable: Room nicks are locked down.");
		break;
	case StanzaErrorRegistrationRequired:
		error_tr=tr("Registration required: User is not on the member list.");
		break;
	case StanzaErrorConflict:
		error_tr=tr("Conflict: Desired room nickname is in use or registered by another user.");
		break;
	case StanzaErrorServiceUnavailable:
		error_tr=tr("Service unavailable: Maximum number of users has been reached.");
		break;
	default:
		nnr = true;
		error_tr=tr("Unknown error: No description.");
		break;
	}

	if ( nnr )
	{
		emit addSystemMessageToConference( "Jabber", conference_name, m_jabber_account->getAccountName(), error_tr,QDateTime(), false);
	}
	else
	{
		QMessageBox::warning(0, tr("Join groupchat on")+" "+m_jabber_account->getAccountName(), error_tr);
		joinGroupchat(conference_name, "", room->password, true);
		leaveConference(conference_name);
	}
}

void jConference::handleMUCInfo (MUCRoom * /*room*/, int /*features*/, const std::string &/*name*/, const DataForm * /*infoForm*/)
{
}

void jConference::handleMUCItems (gloox::MUCRoom*, const std::list<gloox::Disco::Item*, std::allocator<gloox::Disco::Item*> >&)
{
}

void jConference::sendMessageToConference(const QString &conference_name, const QString &message)
{
	if(Room *room = m_rooms.value(conference_name))
	{
		room->last_message = QDateTime::currentDateTime();
		bool handled = false;
		if(message.startsWith("/nick "))
		{
			QString nick = message.section(' ', 1);
			if(!nick.isEmpty())
			{
				room->entity->setNick(utils::toStd(nick));
				handled = true;
			}
		}
		if(message.startsWith("/topic "))
		{
			QString topic = message.section(' ',1);
			if(!topic.isEmpty())
			{
				room->entity->setSubject(utils::toStd(topic));
				handled = true;
			}
		}
		if(!handled)
			room->entity->send(utils::toStd(message));

		m_real_parent->getLastActivityObj()->resetIdleTimer();
	}
}

void jConference::leaveConference(const QString& conference_name)
{
	if(Room *tmp_room = m_rooms.take(conference_name))
	{
		tmp_room->entity->leave("qutIM : Jabber plugin");
		delete tmp_room;
	}
}

void jConference::handleDiscoInfo(const JID &from, const Disco::Info &info, int /*context*/)
{
	QString bare = utils::fromStd(from.bare());
	QString resource = utils::fromStd(from.resource());
	if(Room *room = m_rooms.value(bare))
	{
		QHash<QString,MucContact> &contacts = room->contacts_list;
		if(!contacts.contains(resource))
			return;
		jBuddy::ResourceInfo &resource_info = contacts[resource].m_info;
		jClientIdentification::instance().newInfo(info, &resource_info);
	}
}

void jConference::handleDiscoItems(const JID &/*from*/, const Disco::Items &/*items*/, int /*context*/)
{
}

void jConference::handleDiscoError(const JID &/*from*/, const Error * /*error*/, int /*context*/)
{
}

bool jConference::handleDiscoSet(const IQ &/*iq*/)
{
	return false;
}

void jConference::handlePresence (const Presence &presence)
{
	if(presence.subtype()==Presence::Error)
		return;
  const MUCRoom::MUCUser* mu = presence.findExtension<MUCRoom::MUCUser>(ExtMUCUser);
	if(!mu)
		return;
	QString conference = utils::fromStd(presence.from().bare());
	Room *room = m_rooms.value(conference);
	if(!room)
		return;
	QString nick = utils::fromStd(presence.from().resource());
	QHash<QString,MucContact> &contacts = room->contacts_list;

	int flags = mu->flags();
	bool send_role=false;
	if( flags & UserNickChanged && mu->newNick() )
	{
		// changed nick to new_nick
		QString new_nick = utils::fromStd(*mu->newNick());
		if(!contacts.contains(nick))
			return;
		MucContact contact = contacts[nick];
		jBuddy::ResourceInfo *info = &contact.m_info;
		info->m_name = new_nick;
		contacts.remove(nick);
		contacts.insert(new_nick, contact);
		emit addSystemMessageToConference("Jabber",conference,m_account_name,tr("%1 is now known as %2").arg(nick).arg(new_nick),QDateTime(),false);
		emit renameConferenceItem("Jabber", conference, m_account_name, nick, new_nick);
		return;
	}

	bool if_join=false;
	if(presence.presence()==Presence::Unavailable && contacts.contains(nick))
	{
		QString reason = "";
		if(mu->reason())
			reason = utils::fromStd(*mu->reason());
		if (utils::fromStd(room->entity->nick()) == nick)
		{
			QString msgtxt;
			if(flags & UserKicked){
				msgtxt += tr("You have been kicked from")+" "+conference+".\n";
				if (!reason.isEmpty()){
					msgtxt += tr("with reason:")+" "+reason+".\n";
				} else {
					msgtxt += tr("without reason")+".\n";
				}
				QMessageBox::warning(0, tr("You have been kicked"), msgtxt, QMessageBox::Ok);
				//				//TODO: autorejoin?
				//				msgtxt += tr("Do you want to rejoin?");
				//				int ret = QMessageBox::warning(0, tr("You have been kicked"), msgtxt, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				//				if(ret==QMessageBox::Yes) {
				//					JID jid(utils::toStd(conference));
				//					m_room_list[conference]->leave("");
				//					m_room_list[conference]->join(m_presence->presence(), m_presence->status());
				////					createConferenceRoom(utils::fromStd(jid.server()), utils::fromStd(jid.username()), nick, "" );
				//					return;
				//				}
			}
			else if(flags & UserBanned){
				msgtxt += tr("You have been banned from")+" "+conference+".\n";
				if (!reason.isEmpty()){
					msgtxt += tr("with reason:")+" "+reason+".\n";
				} else {
					msgtxt += tr("without reason")+".\n";
				}
				QMessageBox::warning(0, tr("You have been banned"), msgtxt, QMessageBox::Ok);
			}
		}
		//offline
		contacts.remove(nick);
		emit removeConferenceItem("Jabber", conference, m_account_name, nick);
		QString text;
		if(flags & UserKicked)
			text = tr("%1 has been kicked").arg(nick);
		else if(flags & UserBanned)
			text = tr("%1 has been banned").arg(nick);
		else
			text = tr("%1 has left the room").arg(nick);
		if (!reason.isEmpty())
			text += " ("+reason+")";
		emit addSystemMessageToConference("Jabber",conference,m_account_name,text,QDateTime(),false);
		return;
	}
	else if(presence.presence()==Presence::Unavailable)
		return;
	else
	{
		QString status;
		int mass;
		switch(presence.presence())
		{
		case Presence::Chat:
			status="ffc";
			mass=1;
			break;
		case Presence::Away:
			status="away";
			mass=2;
			break;
		case Presence::DND:
			status="dnd";
			mass=3;
			break;
		case Presence::XA:
			status="na";
			mass=4;
			break;
		case Presence::Error:
			return;
		case Presence::Available:
		default:
			status="online";
			mass=0;
			break;
		}
		if(!contacts.contains(nick) || contacts[nick].m_info.m_name.isNull() || contacts[nick].m_info.m_name.isEmpty())
		{
			//online
			if(!contacts.contains(nick))
			{
				MucContact contact;
				jBuddy::ResourceInfo info;
				info.m_name = nick;
				info.m_chat_state = ChatStateActive;
				contact.m_info = info;
				contact.m_affiliation = AffiliationNone;
				contact.m_role = RoleInvalid ;
				contacts.insert(nick,contact);
				emit addConferenceItem("Jabber", conference, m_account_name, nick);
			}
			MucContact *contact = &contacts[nick];
			jBuddy::ResourceInfo *info = &contact->m_info;
			info->m_name = nick;
			info->m_presence = presence.presence();
			if(presence.presence() == Presence::Invalid)
				info->m_presence = Presence::Available;
			info->m_priority = 0;
			if_join=true;
			emit setConferenceItemRole("Jabber", conference, m_account_name, nick, status, mass);
			send_role=true;
			//			JID jid(room->name()+"@"+room->service()+"/"+participant.nick->resource());
			//			jClientIdentification::instance().setClient(&(contacts[nick]),jid,m_client_base,this);
		}
		else
			contacts[nick].m_info.m_presence = presence.presence();
		if(contacts[nick].m_real_jid.isEmpty() && mu->jid())
			contacts[nick].m_real_jid = utils::fromStd(*mu->jid());
		//changed status
		emit setConferenceItemStatus("Jabber", conference, m_account_name, nick, status, mass);
	}
	if(contacts.contains(nick))
	{
		MucContact *contact = &contacts[nick];
		QString message;
		if(!if_join && mu->role()==contact->m_role && mu->affiliation()==contact->m_affiliation)
			return;
		if(if_join && contact->m_real_jid.isEmpty())
		{
			if(mu->role()!=contact->m_role&&mu->affiliation()!=contact->m_affiliation)
				message = tr("%3 has joined the room as %1 and %2");
			else if(mu->role()!=contact->m_role||mu->affiliation()!=contact->m_affiliation)
				message = tr("%2 has joined the room as %1");
			else
				message = tr("%2 has joined the room");
		}
		else if(if_join)
		{
			if(mu->role()!=contact->m_role&&mu->affiliation()!=contact->m_affiliation)
				message = tr("%4 (%3) has joined the room as %1 and %2");
			else if(mu->role()!=contact->m_role||mu->affiliation()!=contact->m_affiliation)
				message = tr("%3 (%2) has joined the room as %1");
			else
				message = tr("%2 (%1) has joined the room");
		}
		else if(contact->m_real_jid.isEmpty())
		{
			if(mu->role()!=contact->m_role&&mu->affiliation()!=contact->m_affiliation)
				message = tr("%3 now is %1 and %2");
			else if(mu->role()!=contact->m_role||mu->affiliation()!=contact->m_affiliation)
				message = tr("%2 now is %1");
		}
		else
		{
			if(mu->role()!=contact->m_role&&mu->affiliation()!=contact->m_affiliation)
				message = tr("%4 (%3) now is %1 and %2");
			else if(mu->role()!=contact->m_role||mu->affiliation()!=contact->m_affiliation)
				message = tr("%3 (%2) now is %1");
		}
		if(mu->role()!=contact->m_role)
		{
			int mass;
			QString role;
			switch(mu->role())
			{
			case RoleModerator:
				role=tr("moderator");
				mass=1;
				break;
			case RoleParticipant:
				role=tr("participant");
				mass=2;
				break;
			default:
			case RoleVisitor:
				role=tr("visitor");
				mass=3;
				break;
			}
			contact->m_role = mu->role();
			contact->m_role_string = role;
			message = message.arg(role);
			contact->m_info.m_priority = mass;
			emit setConferenceItemRole("Jabber", conference, m_account_name, nick, role, mass);
		}
		if(mu->affiliation()!=contact->m_affiliation)
		{
			QString affiliation;
			switch(mu->affiliation())
			{
			case AffiliationOutcast:
				affiliation=tr("banned");
				break;
			case AffiliationMember:
				affiliation=tr("member");
				break;
			case AffiliationOwner:
				affiliation=tr("owner");
				break;
			case AffiliationAdmin:
				affiliation=tr("administrator");
				break;
			default:
				affiliation=tr("guest");
				break;
			}
			contact->m_affiliation = mu->affiliation();
			contact->m_affiliation_string = mu->affiliation()==AffiliationNone?"":affiliation;
			message = message.arg(affiliation);
		}
		if(!contact->m_real_jid.isEmpty())
			message = message.arg(contact->m_real_jid);
		message = message.arg(nick);
		emit addSystemMessageToConference("Jabber",conference,m_account_name,message,QDateTime(),false);
	}
	jBuddy::ResourceInfo *info = &contacts[nick].m_info;
	info->m_status_message = utils::fromStd(presence.status());
	info->m_status_message.replace("\n","<br/>");
	if(info->m_client_name.isEmpty()||info->m_client_name.isNull())
	{
		const Capabilities *caps = presence.capabilities();
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
		jClientIdentification::instance().setClient(info,presence.from(),m_client_base,this);
		emit conferenceClientVersion("Jabber",conference,m_account_name,nick, info->m_client_name);
	}
	const VCardUpdate *vcard_update = presence.findExtension<VCardUpdate>(ExtVCardUpdate);
	if(vcard_update)
	{
		QString hash = utils::fromStd(vcard_update->hash());
		if(!hash.isEmpty())
		{
			if(QFile(m_jabber_account->getPathToAvatars()+"/"+hash).exists())
			{
				setAvatar(conference,nick,m_jabber_account->getPathToAvatars()+"/"+hash);
			}
			else
			{
				m_real_parent->fetchVCard(conference+"/"+nick,true);
			}
		}
	}
	const XStatusExtension *xstatus = presence.findExtension<XStatusExtension>(SExtXStatus);
	if(xstatus)
	{
		if(xstatus->status_id()<0)
			info->m_x_status="";
		else
			info->m_x_status = QString("icq_xstatus%1").arg(xstatus->status_id());
		emit setConferenceItemIcon("Jabber",conference,m_account_name,nick, info->m_x_status, 5);
	}
}

void jConference::setClient(const QString &conference, const QString &nick, const VersionExtension *version)
{
	Room *room = m_rooms.value(conference);
	if(!room)
		return;
	QHash<QString,MucContact> &contacts = room->contacts_list;
	if(!contacts.contains(nick))
		return;
	jBuddy::ResourceInfo *info = &contacts[nick].m_info;
	jClientIdentification::instance().newInfo(version, info);
	emit conferenceClientVersion("Jabber",conference,m_account_name,nick, info->m_client_name);
	if(info->m_client_name.isEmpty())
		qDebug() << (conference+"/"+nick) << "no client";
	else
		qDebug() << (conference+"/"+nick) << info->m_client_name;
}

void jConference::sendPresence(const QString &conference)
{
	if(Room *room = m_rooms.value(conference))
		room->entity->setPresence(m_presence->subtype(), m_presence->status());
}

void jConference::sendPresenceToAll()
{
	foreach(Room *room, m_rooms)
		room->entity->setPresence(m_presence->subtype(), m_presence->status());
}

QString jConference::getToolTip(const QString &conference, const QString &nickname)
{
	Room *room = m_rooms.value(conference);
	if(!room)
		return nickname;
	QHash<QString,MucContact> &contacts = room->contacts_list;
	if(!contacts.contains(nickname))
		return nickname;
	MucContact *contact = &contacts[nickname];
	jBuddy::ResourceInfo *info = &contact->m_info;
	QString html="<table><tr><td>";
	html.append("<img src='" + m_plugin_system.getStatusIconName(info->m_presence) + "'>");
	html.append("<b>"+info->m_name+"</b><br/>");
	if(!contact->m_affiliation_string.isEmpty())
		html.append(tr("<font size='2'><b>Affiliation:</b> %1</font>").arg(contact->m_affiliation_string)+"<br/>");
	if(!contact->m_role_string.isEmpty())
		html.append(tr("<font size='2'><b>Role:</b> %1</font>").arg(contact->m_role_string)+"<br/>");
	if(!contact->m_real_jid.isEmpty())
		html.append(tr("<font size='2'><b>JID:</b> %1</font>").arg(contact->m_real_jid)+"<br/>");
	html.append(jBuddy::getBaseToolTip(*info));
	html.append("</td></tr>");
	html.append("</table>");
	if(!contact->m_avatar_hash.isEmpty())
	{
		html = QString("<table><tr><td>%2</td><td><img src=\"%1\"/></td></tr></table>").arg(contact->m_avatar_hash).arg(html);
	}
	return html;
}

void jConference::itemContextMenu(const QList<QAction*> &action_list,const QString &conference_name,
								  const QString &nickname,const QPoint &menu_point)
{
	Room *room = m_rooms.value(conference_name);
	if(!room)
		return;
	QHash<QString,MucContact> &contacts = room->contacts_list;
	if(!contacts.contains(nickname))
		return ;
	MucContact *contact = &contacts[nickname];
	MucContact *mine_contact = &contacts[utils::fromStd(room->entity->nick())];
	m_context_menu->clear();
	m_context_menu->addAction(m_menu_title);
	m_menu_label->setText("<b>"+ nickname +"</b>");
	m_current_context_nick = nickname;
	m_current_context_conference_name = conference_name;
	// Default actions at menu
	for(int i=0;i<2 && i<action_list.size();i++)
		m_context_menu->addAction(action_list[i]);
	if (!contact->m_real_jid.isEmpty())
	{
		QAction *action = new QAction(Icon("copy_uin"), tr("Copy JID to clipboard"), this);
		action->setData(jProtocol::getBare(contact->m_real_jid));
		connect(action, SIGNAL(triggered()), this, SLOT(copyJID()));
		m_context_menu->addAction(action);
		action = new QAction(Icon("add_user"), tr("Add to contact list"), this);
		QStringList item_data;
		item_data << jProtocol::getBare(contact->m_real_jid) << nickname;
		action->setData(item_data);
		connect(action, SIGNAL(triggered()), this, SLOT(addToRoster()));
		m_context_menu->addAction(action);
	}
	if ( mine_contact->m_role == RoleModerator )
	{
		m_context_menu->addSeparator();
		m_context_menu->addAction(m_kick_user);
		m_context_menu->addAction(m_ban_user);
		m_context_menu->addSeparator();
		m_context_menu->addAction(m_user_moderator);
		m_context_menu->addAction(m_user_participant);
		m_context_menu->addAction(m_user_visitor);
		switch (contact->m_role)
		{
		case RoleModerator:
			m_user_moderator->setChecked(true);
			break;
		case RoleParticipant:
			m_user_participant->setChecked(true);
			break;
		default:
			m_user_visitor->setChecked(true);
		}
	}

	// Actions at menu from plugins
	for(int i=3;i<action_list.size();i++)
		m_context_menu->addAction(action_list[i]);

	m_context_menu->exec(menu_point);
}

void jConference::kickUser()
{
	Room *room = m_rooms.value(m_current_context_conference_name);
	if(!room)
		return;

	ReasonDialog dialog;
	dialog.setWindowTitle(tr("Kick message"));
	if ( dialog.exec() )
		room->entity->kick(utils::toStd(m_current_context_nick), utils::toStd(dialog.m_reason_text));
}



void jConference::setVisitorUser()
{
	Room *room = m_rooms.value(m_current_context_conference_name);
	if(room)
		room->entity->setRole(utils::toStd(m_current_context_nick), RoleVisitor );
}

void jConference::setParticipantUser()
{
	Room *room = m_rooms.value(m_current_context_conference_name);
	if(room)
		room->entity->setRole(utils::toStd(m_current_context_nick), RoleParticipant );
}

void jConference::setModeratorUser()
{
	Room *room = m_rooms.value(m_current_context_conference_name);
	if(room)
		room->entity->setRole(utils::toStd(m_current_context_nick), RoleModerator );
}


void jConference::banUser()
{
	Room *room = m_rooms.value(m_current_context_conference_name);
	if(!room)
		return;

	ReasonDialog dialog;
	dialog.setWindowTitle(tr("Ban message"));
	if ( dialog.exec() )
		room->entity->ban(utils::toStd(m_current_context_nick), utils::toStd(dialog.m_reason_text));
}


QStringList jConference::getAdditionalInfoAboutContact(const QString &conference, const QString &nickname)
{
	QStringList info_list;
	info_list << nickname;
	Room *room = m_rooms.value(conference);
	if(!room)
		return info_list;
	QHash<QString,MucContact> &contacts = room->contacts_list;
	if(!contacts.contains(nickname))
		return info_list;
	jBuddy::ResourceInfo *info = &contacts[nickname].m_info;
	info_list << QString();
	info_list << (info->m_client_name + " " + info->m_client_version + (info->m_client_os.isEmpty()?"":(" / " + info->m_client_os)));
	info_list << conference+"/"+nickname;
	return info_list;
}

void jConference::setAvatar(const QString &conference, const QString &nickname, const QString &path)
{
	Room *room = m_rooms.value(conference);
	if(!room)
		return;
	QHash<QString,MucContact> &contacts = room->contacts_list;
	if(!contacts.contains(nickname))
		return;
	MucContact *contact = &contacts[nickname];
	contact->m_avatar_hash = path;
	emit setConferenceItemIcon("Jabber",conference,m_account_name,nickname,path,1);
}

void jConference::connectAll()
{
	foreach(Room *room, m_rooms)
	{
		QString room_jid = utils::fromStd(room->entity->name() + '@' + room->entity->service());
		if (room->last_message.isValid())
			room->entity->setRequestHistory(utils::toStamp(room->last_message));
		room->entity->join(m_presence->subtype(), m_presence->status());
	}
}

void jConference::disconnectAll()
{
	foreach(Room *room, m_rooms)
		room->entity->leave("qutIM : Jabber plugin");
}

void jConference::setConferences()
{
	if (m_join_form)
		m_join_form->setConferences();
}

void jConference::s_createConferenceRoom(const QString &conference, const QString &nickname, const QString &password, MUCRoom::HistoryRequestType request_type, const QString &value)
{
	if(Room *room = m_rooms.value(conference))
	{
		if ( !password.isEmpty() )
			room->entity->setPassword(utils::toStd(password));
		room->entity->join(m_presence->subtype(), m_presence->status());
		return;
	}
	//m_jabber_account->getPluginSystem().
	emit createConference("Jabber", conference, m_account_name);
	JID jid(utils::toStd(conference) + "/" + utils::toStd(nickname));
	Room *new_room = new Room;
	new_room->entity = new MUCRoom(m_client_base,jid,this);
	//	MUCRoom *new_room = new MUCRoom(m_client_base,jid,this);
	m_client_base->registerPresenceHandler(jid, this);
	new_room->entity->registerMUCRoomConfigHandler( this );
	if ( !password.isEmpty() )
	{
		new_room->password = password;
		new_room->entity->setPassword(utils::toStd(password));
	}
	m_rooms.insert(conference, new_room);

	emit changeOwnConferenceNickName("Jabber", conference, m_account_name, utils::fromStd(new_room->entity->nick()));
	if (request_type == MUCRoom::HistoryMaxStanzas || request_type == MUCRoom::HistorySeconds)
		new_room->entity->setRequestHistory(value.toInt(), request_type);
	else if (request_type == MUCRoom::HistorySince)
		new_room->entity->setRequestHistory(utils::toStd(value));
	new_room->entity->join(m_presence->subtype(), m_presence->status());
}

void jConference::showTopicConfig(const QString &conference)
{
	if(Room *room = m_rooms.value(conference))
	{
		topicConfigDialog configDialog;
		configDialog.setTopic(room->topic);
		if (configDialog.exec())
			room->entity->setSubject(utils::toStd(configDialog.getTopic()));
	}
}

void jConference::chatWindowAboutToBeOpened(const QString &conference, const QString &nick)
{
}

void jConference::chatWindowClosed(const QString &conference, const QString &nick)
{
}

void jConference::inviteUser()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString conference = action->text();
	QString jid = action->data().toString();
	if(Room *room = m_rooms.value(conference))
		room->entity->invite(utils::toStd(jid), "");
}

void jConference::handleMUCConfigList(MUCRoom *room, const MUCListItemList &items, MUCOperation operation)
{
	QString room_jid = utils::fromStd(room->name() + '@' + room->service());
	if(Room *room = m_rooms.value(room_jid))
	{
		if(!room->participant)
			return;
		if (operation == RequestVoiceList     || operation == RequestBanList
			|| operation == RequestMemberList || operation == RequestModeratorList
			|| operation == RequestOwnerList  || operation == RequestAdminList)
			room->participant->setUserList(items, operation);
	}
}

void jConference::handleMUCConfigForm(MUCRoom *room, const DataForm &form)
{
	QString room_jid = utils::fromStd(room->name() + '@' + room->service());
	if(Room *room = m_rooms.value(room_jid))
	{
		if(!room->config)
			return;
		room->config->setDataForm( form );
	}
}

void jConference::handleMUCConfigResult(MUCRoom *room, bool success, MUCOperation operation)
{
}

void jConference::handleMUCRequest(MUCRoom *room, const DataForm &form)
{
}

void jConference::showConferenceMenu(const QString &room, const QPoint &menu_point)
{
	Room *room_ptr = m_rooms.value(room);
	if(!room_ptr)
		return;
	QMenu *configMenu = new QMenu();
	QAction *actionJoin = new QAction(Icon("chat", "*"), tr("Rejoin to conference"), configMenu);
	configMenu->addAction(actionJoin);
	actionJoin->setData(room);
	connect(actionJoin, SIGNAL(triggered()), this, SLOT(createConferenceRoom()));
	QAction *actionSave = new QAction(tr("Save to bookmarks"), configMenu);
	configMenu->addAction(actionSave);
	actionSave->setData(room);
	connect(actionSave, SIGNAL(triggered()), this, SLOT(saveToBookmark()));
	QAction *actionConfig = new QAction(tr("Room configuration"), configMenu);
	QHash<QString,MucContact> &contacts = room_ptr->contacts_list;
	QString my_nick = utils::fromStd(room_ptr->entity->nick());
	if(contacts.contains(my_nick))
	{
		MucContact *contact = &contacts[my_nick];
		if(contact->m_affiliation == AffiliationOwner || contact->m_affiliation == AffiliationAdmin)
		{
			configMenu->addSeparator();
			actionConfig->setData(room);
			configMenu->addAction(actionConfig);
			connect(actionConfig, SIGNAL(triggered()), this, SLOT(createConfigDialog()));
			QAction *actionAffiliation = new QAction(tr("Room participants"), configMenu);
			//if (m_room_list.value(room)->affiliation() != AffiliationOwner)
			//	actionConfig->setEnabled(false);
			actionAffiliation->setData(room);
			configMenu->addAction(actionAffiliation);
			connect(actionAffiliation, SIGNAL(triggered()), this, SLOT(createParticipantDialog()));
		}
}
	configMenu->popup(menu_point);
}

void jConference::createConfigDialog()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString room = action->data().toString();
		createConfigDialog(room);
	}
}

void jConference::createConfigDialog(const QString &room)
{
	Room *room_ptr = m_rooms.value(room);
	if(!room_ptr || room_ptr->config)
		return;
	jConferenceConfig *config = new jConferenceConfig(m_jabber_account, room, room_ptr->entity);
	room_ptr->config = config;
	config->setWindowIcon(Icon("edituser"));
	config->setWindowTitle(tr("Room configuration: %1").arg(room));
	connect(config, SIGNAL(storeRoomConfig(const DataForm&)), this, SLOT(storeRoomConfig(const DataForm&)));
	room_ptr->entity->requestRoomConfig();
	config->show();
}

void jConference::createParticipantDialog()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString room = action->data().toString();
		createParticipantDialog(room);
	}
}

void jConference::createParticipantDialog(const QString &room)
{
	Room *room_ptr = m_rooms.value(room);
	if(!room_ptr || room_ptr->participant)
		return;
	jConferenceParticipant *config = new jConferenceParticipant(m_jabber_account, room);
	room_ptr->participant = config;
	config->setWindowIcon(m_jabber_account->getPluginSystem().getIcon("edituser"));
	config->setWindowTitle(tr("Room participants: %1").arg(room));
	connect(config, SIGNAL(storeRoomParticipant(const QString&, const MUCListItemList&, MUCOperation)), this, SLOT(storeRoomParticipant(const QString&, const MUCListItemList&, MUCOperation)));
	room_ptr->entity->requestList(RequestMemberList);
	room_ptr->entity->requestList(RequestBanList);
	room_ptr->entity->requestList(RequestAdminList);
	room_ptr->entity->requestList(RequestOwnerList);
	config->show();
}

void jConference::storeRoomConfig(const DataForm &form)
{
}

void jConference::storeRoomParticipant(const QString &room, const MUCListItemList &items, MUCOperation operation)
{
	if(Room *room_ptr = m_rooms.value(room))
		room_ptr->entity->storeList (items, operation);
}

void jConference::copyJID()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString jid = action->data().toString();
		QApplication::clipboard()->setText(jid);
	}
}

void jConference::addToRoster()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QStringList item_data = action->data().toStringList();
		if (!item_data[0].isEmpty() && !item_data[1].isEmpty())
			emit addToRoster(item_data[0], item_data[1]);
	}
}

void jConference::createConferenceRoom()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString conference(action->data().toString());
		if(Room *room = m_rooms.value(conference))
		{
			room->entity->leave("");
			room->entity->join(m_presence->subtype(), m_presence->status());
		}
	}
}

void jConference::saveToBookmark()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString conference(action->data().toString());
		if(Room *room = m_rooms.value(conference))
		{
			QString nick(utils::fromStd(room->entity->nick()));
			jSaveBookmark *saveBookmarkWidget = new jSaveBookmark(m_jabber_account, conference, nick, room->password);
			saveBookmarkWidget->show();
		}
	}
}
