/* Copyright (C) 2008 Jan Kundrát <jkt@gentoo.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "Exceptions.h"

#include <QTextStream>
#include "Parser/Response.h"

namespace Imap {

const char* ParserException::what() const throw()
{
    if ( _offset == -1 )
        return _msg.c_str();
    else {
        QByteArray out(_msg.c_str());
        out += " when parsing this:\n";
        out += _line;
        out += QByteArray( _offset, ' ' );
        out += "^ here\n";
        return out.constData();
    }
}


MailboxException::MailboxException( const char* const msg,
        const Imap::Responses::AbstractResponse& response )
{
    QByteArray buf;
    QTextStream s( &buf );
    s << msg << "\r\n" << response;
    s.flush();
    _msg = buf.constData();
}

MailboxException::MailboxException( const char* const msg ): _msg(msg)
{
}

}
