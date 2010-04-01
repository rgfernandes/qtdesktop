/*
    MSNBuddy

    Copyright (c) 2009 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "msnbuddy.h"

MSNBuddy::MSNBuddy(const QString &id, const QString &group_id)
    :m_id(id),
    m_group_id(group_id)
{
    m_online = false;
}
