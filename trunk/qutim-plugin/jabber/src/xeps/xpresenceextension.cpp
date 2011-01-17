/*****************************************************************************
    QIP X-Presences, UserId+4

    Copyright (c) 2009 by Belov Nikita <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "xpresenceextension.h"
#include <gloox/tag.h>
#include "utils.h"

XPresenceExtension::XPresenceExtension(const Tag* tag) : StanzaExtension(SExtXPresence)
{
	/*
	 * кип уг
	 */

	if(!tag)
		return;
	m_value = utils::fromStd(tag->findAttribute("value")).toInt();
	if (m_value < 4 || m_value > 10 || m_value == 9)
		m_value = -1;
}

const std::string &XPresenceExtension::filterString () const
{
	static const std::string filter = "/presence/x[@xmlns='qip:x:status']";
	return filter;
}

StanzaExtension *XPresenceExtension::newInstance (const Tag *tag) const
{
	return new XPresenceExtension(tag);
}

Tag* XPresenceExtension::tag() const
{
	Tag* t = new Tag("x");
	t->setXmlns("qip:x:status");
	t->addAttribute("value",m_value);
	return t;
}
