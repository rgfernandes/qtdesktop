/*****************************************************************************
    MRIMCLItem

    Copyright (c) 2009 by Rusanov Peter <tazkrut@mail.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "mrimclitem.h"
#include "MRIMContactList.h"

MRIMCLItem::MRIMCLItem(QString aAccount, quint32 aFlags, QString aName): m_account(aAccount), m_Flags(aFlags), m_Name(aName), m_isInUi(false)
{
        m_isNew = true;
}

MRIMCLItem::~MRIMCLItem()
{

}
