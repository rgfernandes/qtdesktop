/*
    MSNGroup

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

#ifndef MSNGROUP_H
#define MSNGROUP_H
#include <QString>

class MSNGroup
{
public:
    MSNGroup(){}
    MSNGroup(const QString &id, const QString &name);
    QString m_id;
    QString m_name;
};

#endif // MSNGROUP_H
