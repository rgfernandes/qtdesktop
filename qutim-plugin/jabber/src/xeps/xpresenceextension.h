/*****************************************************************************
    QIP X-Presences, UserId+1

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

#ifndef xpresenceextension_H
#define xpresenceextension_H

#include <gloox/stanzaextension.h>

using namespace gloox;

class XPresenceExtension : public StanzaExtension
{
public:
	XPresenceExtension(const Tag* tag = 0);
	const std::string &filterString () const;
	StanzaExtension *newInstance (const Tag *tag) const;
	Tag* tag() const;
	StanzaExtension* clone() const
	{
		XPresenceExtension *xpresence = new XPresenceExtension();
		xpresence->m_value = m_value;
		return xpresence;
	}
	int value() const { return m_value; }

private:
	int m_value;
};

#endif
