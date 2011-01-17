/*****************************************************************************
    MRIMContact

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

#ifndef MRIMCONTACT_H
#define MRIMCONTACT_H

#include "mrimclitem.h"
#include <QStringList>
#include "Status.h"
#include "useragent.h"

class MRIMContact : public MRIMCLItem
{
    Q_OBJECT
public:
        MRIMContact(const QString& aAccount, quint32 aFlags, const QString& aName, const QString& aEmail, qint32 aContactId, qint32 aGroupId, const Status& aStatus, quint32 aServerFlags, QString aContPhone, const UserAgent& aUserAgent, quint32 aComSupport, bool aIsAuthed, bool aIsAuthedMe = false);
        ~MRIMContact();
        inline void SetStatus(const Status& aNewStatus) { m_Status.Clone(aNewStatus,true); }
        inline const QString& Email() const { return m_Email; }
        inline const QStringList& Phone() const { return m_Phone; }
        inline void SetPhone(QStringList aNumbers) { m_Phone = aNumbers; }
        inline const Status& GetStatus() const { return m_Status; }
        inline const UserAgent& GetUserAgent() const { return m_UserAgent; }
        inline void SetUserAgent(const UserAgent& aNewAgent) { m_UserAgent.Set(aNewAgent); }
        inline qint32 Id() const { return m_contactId; }
        inline void SetId(qint32 aNewId) { m_contactId = aNewId; }
        inline qint32 GroupId() const { return m_GroupId; }
        inline quint32 ServerFlags() const { return m_ServerFlags; }
        inline bool IsAuthed() const { return m_isAuthed; }
        inline bool IsPurePhoneCnt() const { return (m_Email == "phone"); }
        inline void SetAuthed(bool aAuthed) { m_isAuthed = aAuthed; }
        inline bool IsAuthedMe() const { return m_isAuthedMe; }
        inline void SetAuthedMe(bool aAuthed) { m_isAuthedMe = aAuthed; UpdateAuthInUi(); }
        inline bool InList() { return (m_GroupId != -1); }
        bool HasAvatar();
        QString SmallAvatarPath();
        QString BigAvatarPath();
        void ShowSmallAvatar();
        void Rename(QString aNewName);
        QString GetTooltip();
        inline bool HasPhone() { return m_Phone.count(); }
        void FetchSmallAvatar();
        void FetchBigAvatar();
        void FetchAvatars();
        virtual TreeModelItem GetTreeModel();
        void LoadSettings();
private slots:
        void AvatarFetched(QString aEmail);
        void UpdateStatusInUi();
        void UpdateUserAgentInUi();
        void UpdateAuthInUi();
private:
        virtual void SyncWithUi();

        QString m_Email;
        qint32 m_contactId;
        qint32 m_GroupId;
        Status m_Status;
        quint32 m_ServerFlags;
        QStringList m_Phone;
        UserAgent m_UserAgent;
        bool m_isAuthed;
        bool m_isAuthedMe;
        bool m_showStatusText;
};


#endif // MRIMCONTACT_H
