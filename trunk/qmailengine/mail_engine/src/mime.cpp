/****************************************************************************
* http://sourceforge.net/projects/nlcreator/
*
* QNewsletterCreator - Business Email Client for Mass Mails
* Nuntius Leo - Personal Qt Email Client
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License Version 2 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
* USA. http://www.gnu.org/copyleft/library.html
*
* Please report all bugs and problems to the project admins:
* http://sourceforge.net/projects/nlcreator/
* 
*
* Copyright by dontinelli@users.sourceforge.net if no additional copyright information is given
* otherwise copyright by given authors/organizations 
* and modfified, e.g. ported, by dontinelli@users.sourceforge.net.
* Some code has been taken from 
* http://sourceforge.net/projects/lhmail - License : GNU Library General Public License    
* Authors: lukasz.iwaszkiewicz@gmail.com
* lukasz.iwaszkiewicz@lefthand.com.pl, lukasz.iwaszkiewicz@software.com.pl 
* Copyright (C) 2004/2005 LeftHand sp.z.o.o. info@lefthand.com.pl 
*
*****************************************************************************/


#include "mime.h"
#include <cstdlib>
//
Mime::Mime(  ) 
{
	// TODO
}

QString Mime::decodeQP(QString s)
{
	s.replace("=\r\n","");
	s.replace("=\n","");
	for(int i=0;i<s.size();i++)
	{
		if(s.at(i)==QChar('=') && s.at(i+1)!=QChar('"')){
			QString str=s.mid(i+1,2);
			char **c;
			str=QChar((int)strtol(str.toAscii().data(), c,16));
			s.replace(i,3,str);
		}
	};
	return s;
}

QString Mime::encodeQP(QString s)
{
	for(int i=0;i<s.size();i++)
	{
		int c=s.at(i).unicode();
		if(c==9 || c==10 || c==13 ) continue;
		if ((c < 32 || s.at(i) == 61 || c >= 126)){
			//the char has to be replaced (only "=" and non-ascii characters)
			QString hex("=");
			if(QString::number(c,16).size()==1) hex.append(QString::number(0));
			hex.append(QString::number(c,16).toUpper());
			
			s.replace(i,1,"=" + QString::number(c,16).toUpper());
			i+=QString::number(c,16).size(); //jump over inserted hex-code
		}
	}
	return s;
}
