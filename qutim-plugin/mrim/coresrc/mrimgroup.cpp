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

#include "mrimgroup.h"
#include "MRIMPluginSystem.h"

MRIMGroup::MRIMGroup(QString aAccount, quint32 aFlags, QString aId, QString aName): MRIMCLItem(aAccount,aFlags,aName)
{
    m_Type = EGroup;
    m_Id = aId;
}

QString MRIMGroup::Id() const
{
    return m_Id;
}

MRIMGroup::~MRIMGroup()
{

}

TreeModelItem MRIMGroup::GetTreeModel()
{
    TreeModelItem groupItem;
    groupItem.m_protocol_name = "MRIM";
    groupItem.m_account_name = m_account;
    groupItem.m_item_name = (m_Id == "-1")?"":m_Id;
    groupItem.m_parent_name = m_account;
    groupItem.m_item_type = m_Type;
    return groupItem;
}

void MRIMGroup::SyncWithUi()
{
    if (!IsInUi())
    {
        MRIMPluginSystem::PluginSystem()->addItemToContactList(GetTreeModel(),m_Name);
        SetIsInUi(true);
    }
}
