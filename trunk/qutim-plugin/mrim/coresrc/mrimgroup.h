/*****************************************************************************
    MRIMGroup

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

#ifndef MRIMGROUP_H
#define MRIMGROUP_H

#include "mrimclitem.h"

class MRIMGroup: public MRIMCLItem
{
    Q_OBJECT
public:
        MRIMGroup(QString aAccount, quint32 aFlags, QString aId, QString aName);
        QString Id() const;
        virtual TreeModelItem GetTreeModel();
        ~MRIMGroup();
private:
        QString m_Id;
        virtual void SyncWithUi();
};

#endif // MRIMGROUP_H
