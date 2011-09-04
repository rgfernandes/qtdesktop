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
#ifndef PROFILE_H
#define PROFILE_H

#include <QtCore>

class DocuParser;

//====================== class Profile ============================
class Profile
{
public:
    Profile();

    void addProperty( const QString &name, const QString &value );

    DocuParser *docuParser() const { return dparser; }
    void setDocuParser( DocuParser *dp ) { dparser = dp; }
    DocuParser *dparser;
    QMap<QString,QString> props;
};
	
#endif // PROFILE_H
