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

#ifndef MSNBUDDY_H
#define MSNBUDDY_H

#include <QString>

class MSNBuddy
{
public:
    MSNBuddy(){}
    MSNBuddy(const QString &id, const QString &group_id);
    QString m_id;
    QString m_friendly_name;
    QString m_group_id;
    bool m_online;
};

#endif // MSNBUDDY_H
