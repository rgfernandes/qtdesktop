/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on Trolltech Qt Assistant 4.3, http://trolltech.com
****************************************************************************/

#include "profile.h"

//====================== class Profile ============================
Profile::Profile()
    : dparser( 0 )
{
}
void Profile::addProperty(const QString &name, const QString &value)
{
    props[name] = value;
}

