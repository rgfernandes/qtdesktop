/*
    ActivityExtenstion, XEP-0108

    Copyright (c) 2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "activityextension.h"
#include "src/utils.h"
#include "gloox/tag.h"
#include "jPluginSystem.h"

ActivityExtension::ActivityExtension(const Tag* tag)
: StanzaExtension( SExtActivity )
{
	if(!tag)
		return;
	TagList tag_list = tag->children();
	if ( !tag_list.empty() )
	{
		jPluginSystem &jps = jPluginSystem::instance();
		Tag *text_tag = tag->findChild("text");
		if(text_tag)
			m_text = utils::fromStd(text_tag->cdata());
		Tag *tmp_tag = tag_list.front();
		m_general = utils::fromStd(tmp_tag->name());
		if(!jps.getGeneralActivityTr().contains(m_general))
			m_general = "unknown";
		else
		{
			tag_list = tmp_tag->children();
			if ( !tag_list.empty() )
			{
				m_specific = utils::fromStd(tag_list.front()->name());
				if(!jps.getSpecificActivityTr(m_general).contains(m_specific))
					m_specific="";
			}
		}
	}
}

ActivityExtension::ActivityExtension(const QString &general, const QString &specific, const QString &text)
: StanzaExtension( SExtActivity )
{
	m_general = general;
	m_specific = specific;
	m_text = text;
}

const std::string &ActivityExtension::filterString () const
{
	static const std::string filter = "/message/event/items/item/activity[@xmlns='http://jabber.org/protocol/activity']";
	return filter;
}

StanzaExtension *ActivityExtension::newInstance (const Tag *tag) const
{
	return new ActivityExtension(tag);
}

Tag* ActivityExtension::tag() const
{
	Tag* t = new Tag( "activity" );
	t->setXmlns( "http://jabber.org/protocol/activity");
	
	if ( !m_general.isEmpty() )
	{
		Tag *g = new Tag(t, utils::toStd(m_general), "");
		if ( !m_specific.isEmpty() )
			new Tag(g, utils::toStd(m_specific), "");
		if ( !m_text.isEmpty() )
			new Tag(t, "text", utils::toStd(m_text));
	}
	return t;
}
