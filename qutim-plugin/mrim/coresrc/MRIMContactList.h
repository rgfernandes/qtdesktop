/*****************************************************************************
    MRIMContactList

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

#ifndef MRIMContactList_H_
#define MRIMContactList_H_
#include <QByteArray>
#include <QList>
#include <QBuffer>
#include <QString>
#include <QObject>
#include "proto.h"
#include "mrimcontact.h"
#include "mrimgroup.h"

class MRIMContactList: public QObject
{
Q_OBJECT
public:
        MRIMContactList(QString aAccount, QByteArray& cl);
        MRIMContactList(QString aAccount);
	void SetData(QByteArray& data);
	QList<MRIMCLItem*>* Parse();
	bool AddItem(MRIMCLItem* aItem);
        MRIMContact* CntByEmail(const QString& aName);
        MRIMContact* CntByName(const QString& aName);
        MRIMGroup* GroupById(const QString& aId);
	quint32 GetItemsCount();
	void DeleteEntry(MRIMCLItem* aItemToDelete);
        MRIMCLItem* ItemByIndex(qint32 aIndex);
        QList<MRIMCLItem*>* GetCL();
        virtual ~MRIMContactList();
        void UpdateContactList();
private:
	void Init();
        qint32 ParseGroups();
        qint32 ParseContacts();
        quint32 m_opResult;
	quint32 m_grCount;
        QString m_account;
        QString m_grMask;
        QString m_contMask;
        QBuffer* m_clBuffer;
	QList<MRIMCLItem*>* m_parsedList;
	bool m_isPhoneGroupAdded;
};

#endif /*MRIMContactList_H_*/
