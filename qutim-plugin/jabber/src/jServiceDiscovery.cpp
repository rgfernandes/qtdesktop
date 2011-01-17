/*
Copyright (c) 2008-2009 by Denis Daschenko <daschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "jServiceDiscovery.h"
#include <gloox/error.h>
#include "utils.h"

jServiceDiscovery::jServiceDiscovery(const QString &serviceType)
{
	m_service_type = serviceType;
}

jServiceDiscovery::~jServiceDiscovery()
{
}

void jServiceDiscovery::search(jDiscoItem *disco_item)
{
	m_disco_item = disco_item;
	if (m_disco_item->empty())
		emit getDiscoInfo(m_disco_item->jid(), m_disco_item->node(), (DiscoHandler*) this);
	else
		emit getDiscoItem(m_disco_item->jid(), m_disco_item->node(), (DiscoHandler*) this);
}

void jServiceDiscovery::handleDiscoInfo(const JID &from, const Disco::Info &info, int context)
{
	if (m_disco_item->empty())
	{
		m_disco_item->setJID(utils::fromStd(from.full()));
		m_disco_item->setNode(utils::fromStd(info.node()));
		foreach (std::string feature, info.features())
			m_disco_item->addFeature(utils::fromStd(feature));
		foreach (Disco::Identity *identity, info.identities())
			modifyItem(m_disco_item, identity);
		setActions(m_disco_item);
		if (m_disco_item->expand())
			emit getDiscoItem(utils::fromStd(from.full()), utils::fromStd(info.node()), (DiscoHandler*) this);
		else if (m_service_type == "usersearch")
		{
			m_disco_item->setError("Server doesn't support searching");
			emit finishSelfSearch(m_disco_item);
		}
		if (m_service_type == "usersearch")
		{
			if (m_disco_item->hasIdentity("directory", "user"))
				emit finishSelfSearch(m_disco_item);
		}
		else if (m_service_type != "proxy")
			emit finishSelfSearch(m_disco_item);
	}
	else
	{
		m_count--;
		jDiscoItem *disco_item = new jDiscoItem();
		disco_item->setExpand(false);
		disco_item->setJID(utils::fromStd(from.full()));
		disco_item->setNode(utils::fromStd(info.node()));
		foreach (std::string feature, info.features())
			disco_item->addFeature(utils::fromStd(feature));
		foreach (Disco::Identity *identity, info.identities())
			modifyItem(disco_item, identity);
		setActions(disco_item);
		if (m_service_type == "conference")
		{
			if (disco_item->hasIdentity("conference") || disco_item->hasIdentity("server"))
				emit finishSelfSearch(disco_item);
				//emit finishSearch(m_disco_item->jid(), disco_item);
		}
		else if (m_service_type == "gateway")
		{
			if (disco_item->hasIdentity("gateway") || disco_item->hasIdentity("server") || disco_item->hasIdentity("automation"))
				emit finishSelfSearch(disco_item);
				//emit finishSearch(m_disco_item->jid(), disco_item);
		}
		else if (m_service_type == "proxy")
		{
			if (disco_item->hasIdentity("proxy"))
				emit finishSelfSearch(disco_item);
				//emit finishSearch(m_disco_item->jid(), disco_item);
		}
		else if (m_service_type == "usersearch")
		{
			if (disco_item->hasIdentity("directory", "user"))
				emit finishSelfSearch(disco_item);
			else if (!m_count)
			{
				m_disco_item->setError("Server doesn't support searching");
				emit finishSelfSearch(m_disco_item);
			}
		}
		else
			emit finishSelfSearch(disco_item);
				//emit finishSearch(m_disco_item->jid(), disco_item);
	}
}

void jServiceDiscovery::modifyItem(jDiscoItem *disco_item, Disco::Identity *identity)
{
	if (disco_item->name().isEmpty())
		disco_item->setName(utils::fromStd(identity->name()).replace("\n", " | "));
	jDiscoItem::jDiscoIdentity disco_identity;
	disco_identity.name = utils::fromStd(identity->name()).replace("\n", " | ");
	disco_identity.category = utils::fromStd(identity->category());
	disco_identity.type = utils::fromStd(identity->type());
	disco_item->addIdentity(disco_identity);
}

void jServiceDiscovery::setActions(jDiscoItem *disco_item)
{
	bool isIRC = false;
	foreach (jDiscoItem::jDiscoIdentity identity, disco_item->identities())
		if (identity.category == "conference" && identity.type == "irc")
			isIRC = true;
	disco_item->addAction(jDiscoItem::ADD);
	if (disco_item->hasFeature("http://jabber.org/protocol/muc") && (!utils::fromStd(JID(utils::toStd(disco_item->jid())).username()).isEmpty() || isIRC))
		disco_item->addAction(jDiscoItem::JOIN);
	if (disco_item->hasFeature("http://jabber.org/protocol/bytestreams"))
		disco_item->addAction(jDiscoItem::PROXY);
	if(disco_item->hasFeature("http://jabber.org/protocol/muc#register") || disco_item->hasFeature("jabber:iq:register"))
		disco_item->addAction(jDiscoItem::REGISTER);
	if (disco_item->hasFeature("jabber:iq:search"))
		disco_item->addAction(jDiscoItem::SEARCH);
	if (disco_item->hasFeature("vcard-temp"))
		disco_item->addAction(jDiscoItem::VCARD);
	if (disco_item->hasFeature("http://jabber.org/protocol/disco#items") || disco_item->hasFeature("http://jabber.org/protocol/muc") && !isIRC)
		disco_item->setExpand(true);
	if (disco_item->hasIdentity("automation"))
	{
		bool expand = false;
		foreach (jDiscoItem::jDiscoIdentity identity, disco_item->identities())
			if (identity.type == "command-list")
				expand = true;
		if (expand)
			disco_item->setExpand(true);
		else
			disco_item->addAction(jDiscoItem::EXEC);
	}
	else if (disco_item->hasFeature("http://jabber.org/protocol/commands"))
		disco_item->setExpand(true);
}

void jServiceDiscovery::handleDiscoItems(const JID &from, const Disco::Items &items, int context)
{
	bool isConf = false, isIRC = false;
	foreach (jDiscoItem::jDiscoIdentity identity, m_disco_item->identities())
		if (identity.category == "conference")
			isConf = true;
	QList<jDiscoItem*> item_list;
	m_count = 0;
	foreach (Disco::Item *item, items.items())
	{
		m_count++;
		jDiscoItem *disco_item = new jDiscoItem();
		disco_item->setExpand(false);
		disco_item->setName(utils::fromStd(item->name()).replace("\n", " | "));
		disco_item->setJID(utils::fromStd(item->jid().full()));
		disco_item->setNode(utils::fromStd(item->node()));
		if (isConf)
		{
			jDiscoItem::jDiscoIdentity disco_identity;
			disco_identity.name = utils::fromStd(item->name()).replace("\n", " | ");
			disco_identity.category = "conference";
			disco_identity.type = "text";
			if (utils::fromStd(from.username()).isEmpty())
			{
				disco_item->addAction(jDiscoItem::JOIN);
				disco_item->addAction(jDiscoItem::ADD);
				disco_item->setExpand(true);
			}
			disco_item->addIdentity(disco_identity);
		}
		if (!isConf) // || utils::fromStd(item->jid().resource()).isEmpty())
			emit getDiscoInfo(utils::fromStd(item->jid().full()), utils::fromStd(item->node()), (DiscoHandler*) this);
		item_list << disco_item;
	}
	emit finishSearch(item_list, m_disco_item->key());
}

void jServiceDiscovery::handleDiscoError(const JID &from, const Error *error, int context)
{
	jDiscoItem *disco_item = new jDiscoItem();
	disco_item->setJID(utils::fromStd(from.full()));
	disco_item->setError(utils::fromStd(error->text()));
	if (disco_item->error().isEmpty())
	{
		QString errorText;
		switch (error->error())
		{
			case StanzaErrorBadRequest:
				errorText = tr("The sender has sent XML that is malformed or that cannot be processed.");
				break;
			case StanzaErrorConflict:
				errorText = tr("Access cannot be granted because an existing resource or session exists with the same name or address.");
				break;
			case StanzaErrorFeatureNotImplemented:
				errorText = tr("The feature requested is not implemented by the recipient or server and therefore cannot be processed.");
				break;
			case StanzaErrorForbidden:
				errorText = tr("The requesting entity does not possess the required permissions to perform the action.");
				break;
			case StanzaErrorGone:
				errorText = tr("The recipient or server can no longer be contacted at this address.");
				break;
			case StanzaErrorInternalServerError:
				errorText = tr("The server could not process the stanza because of a misconfiguration or an otherwise-undefined internal server error.");
				break;
			case StanzaErrorItemNotFound:
				errorText = tr("The addressed JID or item requested cannot be found.");
				break;
			case StanzaErrorJidMalformed:
				errorText = tr("The sending entity has provided or communicated an XMPP address or aspect thereof that does not adhere to the syntax defined in Addressing Scheme.");
				break;
			case StanzaErrorNotAcceptable:
				errorText = tr("The recipient or server understands the request but is refusing to process it because it does not meet criteria defined by the recipient or server.");
				break;
			case StanzaErrorNotAllowed:
				errorText = tr("The recipient or server does not allow any entity to perform the action.");
				break;
			case StanzaErrorNotAuthorized:
					errorText = tr("The sender must provide proper credentials before being allowed to perform the action, or has provided impreoper credentials.");
				break;
			case StanzaErrorNotModified:
				errorText = tr("The item requested has not changed since it was last requested.");
				break;
			case StanzaErrorPaymentRequired:
				errorText = tr("The requesting entity is not authorized to access the requested service because payment is required.");
				break;
			case StanzaErrorRecipientUnavailable:
				errorText = tr("The intended recipient is temporarily unavailable.");
				break;
			case StanzaErrorRedirect:
				errorText = tr("The recipient or server is redirecting requests for this information to another entity, usually temporarily.");
				break;
			case StanzaErrorRegistrationRequired:
				errorText = tr("The requesting entity is not authorized to access the requested service because registration is required.");
				break;
			case StanzaErrorRemoteServerNotFound:
				errorText = tr("A remote server or service specified as part or all of the JID of the intended recipient does not exist.");
				break;
			case StanzaErrorRemoteServerTimeout:
				errorText = tr("A remote server or service specified as part or all of the JID of the intended recipient could not be contacted within a reasonable amount of time.");
				break;
			case StanzaErrorResourceConstraint:
				errorText = tr("The server or recipient lacks the system resources necessary to service the request.");
				break;
			case StanzaErrorServiceUnavailable:
				errorText = tr("The server or recipient does not currently provide the requested service.");
				break;
			case StanzaErrorSubscribtionRequired:
				errorText = tr("The requesting entity is not authorized to access the requested service because a subscription is required.");
				break;
			case StanzaErrorUndefinedCondition:
				errorText = tr("The unknown error condition.");
				break;
			case StanzaErrorUnexpectedRequest:
				errorText = tr("The recipient or server understood the request but was not expecting it at this time.");
				break;
			case StanzaErrorUnknownSender:
				errorText = tr("The stanza 'from' address specified by a connected client is not valid for the stream.");
		}
		disco_item->setError(errorText);
	}
	emit finishSelfSearch(disco_item);
}

bool jServiceDiscovery::handleDiscoSet(const IQ &iq)
{
	return false;
}
