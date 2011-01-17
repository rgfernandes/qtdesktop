/*****************************************************************************
    Gmail Notifications, UserId+2

    Copyright (c) 2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "gmailextension.h"
#include "src/utils.h"
#include "gloox/tag.h"

GMailExtension::GMailExtension(const Tag* tag)
: StanzaExtension( SExtGmail )
{
	m_query = false;
	if(!tag)
		return;
	if(tag->name()=="new-mail")
	{
		m_notification=true;
		return;
	}
	m_date = utils::fromStd(tag->findAttribute("result-time")).toLongLong();
	m_total_matched = utils::fromStd(tag->findAttribute("total-matched")).toInt();
	m_notification=false;
	TagList tag_list = tag->children();
	TagList::const_iterator it = tag_list.begin();
	for( ; it != tag_list.end(); ++it )
	{
		if((*it)->name()=="mail-thread-info")
		{
			MailThread mail;
			mail.time = QDateTime::fromTime_t(utils::fromStd((*it)->findAttribute("date")).toLongLong()/1000);
			mail.tid = utils::fromStd((*it)->findAttribute("tid"));
			mail.url = utils::fromStd((*it)->findAttribute("url"));
			mail.messages = utils::fromStd((*it)->findAttribute("messages")).toInt();
			Tag *t = (*it)->findChild("labels");
			if(t)
				mail.labels = utils::fromStd(t->cdata()).split('|');
			t = (*it)->findChild("subject");
			if(t)
				mail.subject = utils::fromStd(t->cdata());
			t = (*it)->findChild("snippet");
			if(t)
				mail.snippet = utils::fromStd(t->cdata());
			t = (*it)->findChild("senders");
			if(t)
			{
				TagList tag_list_2 = t->children();
				TagList::const_iterator it2 = tag_list_2.begin();
				for( ; it2 != tag_list_2.end(); ++it2 )
				{
					Sender sender;
					sender.name = utils::fromStd((*it2)->findAttribute("name"));
					sender.address = utils::fromStd((*it2)->findAttribute("address"));
					sender.originator = utils::fromStd((*it2)->findAttribute("originator")).toInt()==1;
					sender.unread = utils::fromStd((*it2)->findAttribute("unread")).toInt()==1;
					mail.senders.append(sender);
				}
			}
			m_mail_threads.append(mail);
		}
	}
}
GMailExtension::GMailExtension(quint64 date)
: StanzaExtension( SExtGmail )
{
	m_query = true;
	m_date = date;
}

const std::string &GMailExtension::filterString () const
{
	static const std::string filter = "/iq/mailbox[@xmlns='google:mail:notify']"
		"|/iq/new-mail[@xmlns='google:mail:notify']";
	return filter;
}

StanzaExtension *GMailExtension::newInstance (const Tag *tag) const
{
	return new GMailExtension(tag);
}

Tag *GMailExtension::tag() const
{
	if(m_query)
	{
		Tag *t = new Tag("query");
		t->setXmlns("google:mail:notify");
		t->addAttribute("newer-than-time",QString::number(m_date).toStdString());
		return t;
	}
	return new Tag(m_notification?"new-mail":"mailbox");
}
