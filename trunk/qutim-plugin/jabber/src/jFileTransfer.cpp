/*****************************************************************************
	jFileTransfer

	Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "jFileTransfer.h"
#include "jFileTransferRequest.h"
#include "jPluginSystem.h"
#include <gloox/util.h>
#include <stdlib.h>
#include "utils.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>


jFileTransfer::StreamHostQuery::StreamHostQuery(const Tag *tag) : StanzaExtension(SExtFileTransfer)
{
	if(!tag)
		return;
	Tag *streamhost = tag->findChild("streamhost");
	if(!streamhost)
		return;
	m_jid = JID(streamhost->findAttribute("jid"));
	m_host = streamhost->findAttribute("host");
	m_port = atoi(streamhost->findAttribute("port").c_str());
	m_zeroconf = streamhost->findAttribute("zeroconf");
}

const std::string &jFileTransfer::StreamHostQuery::filterString () const
{
	static const std::string filter = "/iq/query[@xmlns='" + XMLNS_BYTESTREAMS + "']";
	return filter;
}

StanzaExtension *jFileTransfer::StreamHostQuery::newInstance (const Tag *tag) const
{
	return new StreamHostQuery(tag);
}

Tag *jFileTransfer::StreamHostQuery::tag() const
{
	Tag *tag = new Tag("query");
	tag->setXmlns(XMLNS_BYTESTREAMS);
	if(!m_jid)
		return tag;
	Tag *streamhost = new Tag(tag, "streamhost");
	streamhost->addAttribute("jid", m_jid.full());
	if(!m_host.empty())
		streamhost->addAttribute("host", m_host);
	if(m_port>0)
		streamhost->addAttribute("port", util::int2string(m_port));
	if(!m_zeroconf.empty())
		streamhost->addAttribute("zeroconf", m_zeroconf);
	return tag;
}

jFileTransfer::jFileTransfer(Client *client)
{
	//  m_manager = new SIManager(client);
	//  m_socks5_manager = new SOCKS5BytestreamManager( m_parent, this );
	m_client = client;
	m_ft = new SIProfileFT(m_client, this);
	m_client->registerStanzaExtension(new StreamHostQuery());
	m_ft->registerSOCKS5BytestreamServer(jPluginSystem::instance().getSOCKS5Server());
}

void jFileTransfer::sendFileTo(const QString &jid, const QStringList &fileNames)
{
	if(!fileNames.size())
		return;
	jPluginSystem::instance().newFtOpened();
	QFileInfo file(fileNames[0]);
	std::string sid = m_ft->requestFT( JID(utils::toStd(jid)), utils::toStd(file.fileName()), file.size() );
	jFileTransferWidget *ft_widget = new jFileTransferWidget(true, this, m_ft, utils::toStd(jid), sid, utils::toStd(file.absoluteFilePath()), file.size());
	m_ft_widgets[utils::fromStd(sid+"@"+m_client->jid().full())] = ft_widget;
	qDebug() << "you want to send file" << fileNames[0] << "to" << jid << utils::fromStd(sid);
	qDebug() << utils::fromStd(sid+"@"+m_client->jid().full());
	ft_widget->setFilePath();
	ft_widget->show();
}

void jFileTransfer::handleFTRequest (const JID &from, const JID &, const std::string &sid, const std::string &name, long size, const std::string &hash, const std::string &date, const std::string &mimetype, const std::string &desc, int stypes)
{    
	qDebug() << "handleFTRequest" << utils::fromStd(from.full()) << utils::fromStd(sid) << (int)stypes;
	if( !stypes & (SIProfileFT::FTTypeS5B | SIProfileFT::FTTypeIBB) )
	{
		m_ft->declineFT(from, sid, SIManager::NoValidStreams);
		return;
	}
	jFileTransferWidget *ft_widget = new jFileTransferWidget(false, this, m_ft, from, sid, name, size, hash, date, mimetype, desc, stypes);
	m_ft_widgets[utils::fromStd(sid+"@"+from.full())] = ft_widget;
	(new jFileTransferRequest(this, ft_widget, m_ft, from, sid, name, size, hash, date, mimetype, desc, stypes))->show();
}

void jFileTransfer::handleFTRequestError (const IQ &iq, const std::string &sid)
{
}

void jFileTransfer::handleFTBytestream (Bytestream *bs)
{
	jFileTransferWidget *ft_widget = m_ft_widgets.value(utils::fromStd(bs->sid()+"@"+bs->initiator().full()), 0);
	if(ft_widget)
		ft_widget->setBytestream(bs);
	qDebug() << utils::fromStd(bs->initiator().full()) << utils::fromStd(bs->sid()) << (int)bs->type();
}

const std::string jFileTransfer::handleOOBRequestResult (const JID &from, const JID&, const std::string &sid)
{
	return EmptyString;
}

bool jFileTransfer::handleIq (const IQ &)
{
	return false;
}

void jFileTransfer::handleIqID (const IQ &iq, int )
{
	const StreamHostQuery *stream_host = iq.findExtension<StreamHostQuery>(SExtFileTransfer);
	if(stream_host)
	{
		StreamHost new_host = stream_host->getStreamHost();
		appendStreamHost(new_host);
	}
}

void jFileTransfer::searchSocks5Proxy(const JID& jid)
{
	IQ iq(IQ::Get, jid, m_client->getID());
	iq.addExtension(new StreamHostQuery());
	m_client->send(iq, this, 0);
}

void jFileTransfer::searchSocks5Proxy(jDiscoItem *disco_item)
{
	searchSocks5Proxy(JID(utils::toStd(disco_item->jid())));
}

void jFileTransfer::appendStreamHost(const StreamHost &host)
{
	qDebug() << utils::fromStd(host.jid.full()) << utils::fromStd(host.host) << host.port;
	if(!host.jid)
		return;
	foreach(const StreamHost &h, m_hosts)
	{
		if(host.jid == h.jid && host.host == h.host && host.port == h.port)
			return;
	}
	m_hosts.append(host);
	m_ft->setStreamHosts(m_hosts.toStdList());
}

void jFileTransfer::prependStreamHost(const StreamHost &host)
{
	if(!host.jid)
		return;
	foreach(const StreamHost &h, m_hosts)
	{
		if(host.jid == h.jid && host.host == h.host && host.port == h.port)
			return;
	}
	m_hosts.prepend(host);
	m_ft->setStreamHosts(m_hosts.toStdList());
}

void jFileTransfer::replaceStreamHost(const StreamHost &host)
{
	if(!host.jid)
		return;
	bool has_host = false;
	for(int i=m_hosts.size()-1;i>-1;i--)
	{
		StreamHost &h = m_hosts[i];
		if(host.jid == h.jid && host.host == h.host && host.port == h.port)
			return;
		else if(host.jid == h.jid)
		{
			if(has_host)
				m_hosts.removeAt(i);
			else
			{
				h.host = host.host;
				h.port = host.port;
				has_host = true;
			}
		}
	}
	if(has_host)
		m_ft->setStreamHosts(m_hosts.toStdList());
}

void jFileTransfer::removeWidget(const JID &from, const std::string &sid, bool del, bool sending)
{
	jFileTransferWidget *ft_widget = m_ft_widgets.take(utils::fromStd(sid+"@"+from.full()));
	if(sending)
		jPluginSystem::instance().ftClosed();
	if(del)
		delete ft_widget;
}
