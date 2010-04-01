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

#ifndef MRIMCLITEM_H
#define MRIMCLITEM_H
#include <QObject>
#include "mrimdefs.h"
#include <qutim/plugininterface.h>

using namespace qutim_sdk_0_2;
class MRIMContactList;

class MRIMCLItem : public QObject
{
    Q_OBJECT
    friend class MRIMContactList;
public:
    MRIMCLItem(QString aAccount, quint32 aFlag, QString aName);
    ~MRIMCLItem();
    inline const QString& Name() const { return m_Name; }
    inline CLItemType Type() const { return m_Type; }
    inline bool IsNew() const { return m_isNew; }
    inline QString Account() { return m_account; }
    virtual TreeModelItem GetTreeModel() = 0;
    inline bool IsInUi() { return m_isInUi; }

protected:
    CLItemType m_Type;
    quint32 m_Flags;
    QString m_Name;
    QString m_account;
    bool m_isNew;
    bool m_isInUi;

    inline void SetIsInUi(bool aIsInUi) { m_isInUi = aIsInUi; }
    virtual void SyncWithUi() = 0;
};
#endif // MRIMCLITEM_H
