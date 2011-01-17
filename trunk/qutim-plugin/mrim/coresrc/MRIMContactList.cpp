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

#include "MRIMContactList.h"
#include "MRIMUtils.h"
#include <qdebug.h>

MRIMContactList::MRIMContactList(QString aAccount) : m_account(aAccount)
{
    Init();
}

void MRIMContactList::Init()
{
    m_isPhoneGroupAdded = false;
    m_clBuffer = new QBuffer(this);
    m_clBuffer->open(QIODevice::ReadWrite);
    m_parsedList = new QList<MRIMCLItem*>;
}

void MRIMContactList::SetData(QByteArray& data)
{
    if (m_clBuffer)
    {
            delete m_clBuffer;
            m_clBuffer = new QBuffer(this);
            m_clBuffer->open(QIODevice::ReadWrite);
    }
    qint64 written = m_clBuffer->write(data);
    Q_UNUSED(written);
}

MRIMContactList::MRIMContactList(QString aAccount, QByteArray& cl) : m_account(aAccount)
{	
    Init();
    SetData(cl);
}


MRIMContactList::~MRIMContactList()
{
    if (m_parsedList)
    {
        delete m_parsedList;
    }

    if (m_clBuffer)
    {
        m_clBuffer->close();
        delete m_clBuffer;
    }

}

QList<MRIMCLItem*>* MRIMContactList::Parse()
{
    m_clBuffer->seek(0);
    m_opResult = ByteUtils::ReadToUL(*m_clBuffer);
    m_grCount =	ByteUtils::ReadToUL(*m_clBuffer);
    m_grMask = ByteUtils::ReadToString(*m_clBuffer);
    m_contMask = ByteUtils::ReadToString(*m_clBuffer);

    if (m_grMask == "" || m_contMask == "" )
            return NULL;

    if (m_opResult == GET_CONTACTS_OK)
    {
#ifdef DEBUG_LEVEL_DEV
            qDebug()<<"GroupMask,ContactMask = "<<m_grMask<<","<<m_contMask;
#endif
            ParseGroups();
            ParseContacts();
    }
    return NULL;
}

qint32 MRIMContactList::ParseGroups()
{
	if (m_grCount <= 0) return 0;

	if (!m_parsedList)
	{
		m_parsedList = new QList<MRIMCLItem*>();	
	}
	
	for (quint32 i=0; i < m_grCount; i++)
	{
		quint32 grFlags = 0;
                QString grName;

                for (qint32 j=0; j < m_grMask.length(); j++)
		{	
                        QChar chr = m_grMask.at(j);
			switch (chr.toAscii())
			{
				case 'u':
				{
					quint32 val = ByteUtils::ReadToUL(*m_clBuffer);
					if (j == 0)
					{
						grFlags = val;
                                        #ifdef DEBUG_LEVEL_DEV
						qDebug("UL for group #%d read. grFlags=%x",i+1,grFlags);						
                                        #endif
					}
				}
				break;
				case 's':
				{
                                        QString valStr = ByteUtils::ReadToString(*m_clBuffer,true);
					if (j == 1)
					{
                                                grName = valStr;
                                        #ifdef DEBUG_LEVEL_DEV
                                                qDebug()<<"String for group #"<<i+1<<" read. grName="<<grName;
                                        #endif
					}
				}
				break;
				
				default:
				break;
			}
		}
                MRIMGroup* group = new MRIMGroup(m_account, grFlags,QString::number(i),grName);
                AddItem(group);
	}
	return m_grCount;
}

qint32 MRIMContactList::ParseContacts()
{
	if (!m_parsedList)
	{
		m_parsedList = new QList<MRIMCLItem*>();	
	}
	quint32 count = 0;
	quint32 idCounter = 20;
	while(m_clBuffer->pos() < m_clBuffer->size())
	{
		quint32 contFlags = 0;
                quint32 grId = 0;
		quint32 serverFlags = 0;
		quint32 currentStatus = 0;                
                quint32 comSupport;
                QString email; //ANSI
                QString contName; //UNICODE
                QString contPhone; //ANSI
                QString specStatusUri; //ANSI
                QString statusTitle; //UNICODE
                QString statusDesc; //UNICODE
                QString userAgent; //ANSI

                for (qint32 j=0; j < m_contMask.length(); j++)
		{	
                        QChar chr = m_contMask.at(j);
			switch (chr.toAscii())
			{
				case 'u':
				{
					quint32 val = ByteUtils::ReadToUL(*m_clBuffer);
					switch (j)
					{
						case 0:
						{
							contFlags = val;
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"UL for contact #"<<count+1<<" read. contFlags="<<QString::number(contFlags,16);
                                                #endif
						}
						break;
						case 1:
						{
							grId = val;
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"UL for contact #"<<count+1<<" read. grId="<<QString::number(grId,16);
                                                #endif
						}
						break;
						case 4:
						{
							serverFlags = val;
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"UL for contact #"<<count+1<<" read. serverFlags="<<QString::number(serverFlags,16);
                                                #endif
						}
						break;
						case 5:
						{
							currentStatus = val;
                                                #ifdef DEBUG_LEVEL_DEV
							qDebug()<<"UL for contact #"<<count+1<<" read. currentStatus="<<currentStatus;						
                                                #endif
						}
						break;
                                                case 10:
                                                {
                                                        comSupport = val;
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"UL for contact #"<<count+1<<" read. comSupport="<<comSupport;
                                                #endif
                                                }
                                                break;
					}
				}
				break;
				case 's':
				{
                                        switch (j)
					{
						case 2:
						{
							email = ByteUtils::ReadToString(*m_clBuffer);;
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"String for contact #"<<count+1<<" read. email="<<email;
                                                #endif
						}
						break;
						case 3:
						{
                                                        contName = ByteUtils::ReadToString(*m_clBuffer,true);
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"String for contact #"<<count+1<<" read. contName="<<contName;
                                                #endif
                                                }
						break;
						case 6:
						{
							contPhone = ByteUtils::ReadToString(*m_clBuffer);
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"String for contact #"<<count+1<<" read. contPhone="<<contPhone;
                                                #endif
                                                }
						break;
                                                case 7:
                                                {
                                                        specStatusUri = ByteUtils::ReadToString(*m_clBuffer);
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"String for contact #"<<count+1<<" read. spec. status URI="<<specStatusUri;
                                                #endif
                                                }
                                                break;
                                                case 8:
                                                {
                                                        statusTitle = ByteUtils::ReadToString(*m_clBuffer,true);
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"String for contact #"<<count+1<<" read. statusTitle="<<statusTitle;
                                                #endif
                                                }
                                                break;
                                                case 9:
                                                {
                                                        statusDesc = ByteUtils::ReadToString(*m_clBuffer,true);
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"String for contact #"<<count+1<<" read. statusDesc="<<statusDesc;
                                                #endif
                                                }
                                                break;
                                                case 11:
                                                {
                                                        userAgent = ByteUtils::ReadToString(*m_clBuffer);
                                                #ifdef DEBUG_LEVEL_DEV
                                                        qDebug()<<"String for contact #"<<count+1<<" read. userAgent="<<userAgent;
                                                #endif
                                                }
                                                break;
                                                default:
							ByteUtils::ReadToString(*m_clBuffer); //just skeep bytes, temp solution TODO
						break;
					}
				}
				break;				
				default:
				break;
			}
		}
		bool isAuthedMe = !(serverFlags & CONTACT_INTFLAG_NOT_AUTHORIZED);

                if (!(contFlags & CONTACT_FLAG_REMOVED) && grId < 20)
                {
                    UserAgent *contAgent = UserAgent::Parse(userAgent);
                    Status contFullStatus(currentStatus,statusTitle,statusDesc,specStatusUri);

                    MRIMContact* contact = new MRIMContact(m_account, contFlags,contName,email,idCounter,grId,contFullStatus,serverFlags,contPhone,*contAgent,comSupport,true,isAuthedMe);
                    delete contAgent;

                    AddItem(contact);

                    if (contact->Email() == "phone" && !m_isPhoneGroupAdded)
                    {
                        MRIMGroup* phoneGroup = new MRIMGroup(m_account, 0,QString::number(contact->GroupId()),tr("Phone contacts"));
                        AddItem(phoneGroup);
                        m_isPhoneGroupAdded = true;
                    }
                }
		count++;
		idCounter++;
	}
        #ifdef DEBUG_LEVEL_DEV
        qDebug()<<"Total contacts read:"<<count;
        #endif
	return count;
}

MRIMContact* MRIMContactList::CntByEmail(const QString& aEmail)
{	
	MRIMContact * foundContact = NULL;
	for (qint32 i=0; i < m_parsedList->count(); i++)
	{
		if ( m_parsedList->at(i)->Type() == EContact ) 
		{	
			foundContact = static_cast<MRIMContact*>(m_parsedList->at(i));
			if (foundContact->Email() == aEmail)
			{
				break;
			}
			else
			{
				foundContact = NULL;
			}
		}
	}
	return foundContact;
}

MRIMContact* MRIMContactList::CntByName(const QString& aName)
{
	MRIMContact * foundContact = NULL;
	for (qint32 i=0; i < m_parsedList->count(); i++)
	{
		if ( m_parsedList->at(i)->Type() == EContact ) 
		{	
			foundContact = static_cast<MRIMContact*>(m_parsedList->at(i));
			if (foundContact->Name() == aName)
			{
				break;
			}
			else
			{
				foundContact = NULL;
			}
		}
	}
	return foundContact;
}

MRIMGroup* MRIMContactList::GroupById(const QString& aId)
{
	MRIMGroup * foundGroup = NULL;
	for (qint32 i=0; i < m_parsedList->count(); i++)
	{
		if ( m_parsedList->at(i)->Type() == EGroup ) 
		{	
			foundGroup = static_cast<MRIMGroup*>(m_parsedList->at(i));
			if (foundGroup->Id() == aId)
			{
				break;
			}
			else
			{
				foundGroup = NULL;
			}
		}
	}
	return foundGroup;
}

quint32 MRIMContactList::GetItemsCount()
{
	return m_parsedList->count();
}

MRIMCLItem* MRIMContactList::ItemByIndex(qint32 aIndex)
{
	if (aIndex < 0 || aIndex >m_parsedList->count()) return NULL;
	return m_parsedList->at(aIndex);
}

bool MRIMContactList::AddItem(MRIMCLItem* aItem)
{
	if (!m_parsedList) return true;
	bool isNew = true;
        bool isInUi = false;

	if (aItem->Type() == EContact)
	{
		MRIMContact* newCnt = reinterpret_cast<MRIMContact*>(aItem);
                MRIMContact* cnt = CntByEmail(newCnt->Email());

                if (cnt && cnt->Email() == "phone")
		{
                        cnt = CntByName(newCnt->Name());
		}

		if (cnt != NULL)
		{
			isNew = false;
                        aItem->SetIsInUi(cnt->IsInUi());
                        aItem->m_isNew = false;
			m_parsedList->removeOne(cnt);
			delete cnt;
		}
		m_parsedList->append(aItem);
	}

	if (aItem->Type() == EGroup)
	{
		MRIMGroup* newGr = reinterpret_cast<MRIMGroup*>(aItem);
                MRIMGroup* gr = GroupById(newGr->Id());

                if (gr != NULL)
		{
			isNew = false;
                        aItem->SetIsInUi(gr->IsInUi());
                        aItem->m_isNew = false;
                        m_parsedList->removeOne(gr);
			delete gr;
		}
		m_parsedList->append(aItem);
	}

        aItem->SyncWithUi();
	return isNew;
}

void MRIMContactList::DeleteEntry(MRIMCLItem* aItemToDelete)
{
	int index = m_parsedList->indexOf(aItemToDelete);

	if (index != -1)
	{
		m_parsedList->removeAt(index);
		delete aItemToDelete;
	}
}

QList<MRIMCLItem*>* MRIMContactList::GetCL()
{
        return m_parsedList;
}

void MRIMContactList::UpdateContactList()
{
    foreach (MRIMCLItem *item, *m_parsedList)
            item->SyncWithUi();
}
