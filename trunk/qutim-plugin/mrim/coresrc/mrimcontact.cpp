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

#include "mrimcontact.h"
#include "MRIMUtils.h"
#include "MRIMPluginSystem.h"
#include <qutim/iconmanagerinterface.h>
#include <QSettings>
#include <QDateTime>
#include <QTextDocument>
#include <QMessageBox>
#include "avatarfetcher.h"

MRIMContact::MRIMContact(const QString& aAccount, quint32 aFlags, const QString& aName,
            const QString& aEmail, qint32 aContactId, qint32 aGroupId, const Status& aStatus,
            quint32 aServerFlags, QString aContPhone, const UserAgent& aUserAgent,
            quint32 aComSupport, bool aIsAuthed, bool aIsAuthedMe)
: MRIMCLItem(aAccount,aFlags,aName), m_GroupId(aGroupId), m_ServerFlags(aServerFlags),
  m_Email(aEmail), m_isAuthed(aIsAuthed), m_isAuthedMe(aIsAuthedMe), m_contactId(aContactId)
{
    LoadSettings();
    m_Type = EContact;
    m_Phone = aContPhone.split(',',QString::SkipEmptyParts);
    m_Status.Clone(aStatus);
    m_UserAgent.Set(aUserAgent);
    connect(&m_Status,SIGNAL(Changed()),this,SLOT(UpdateStatusInUi()));
    connect(&m_UserAgent,SIGNAL(Changed()),this,SLOT(UpdateUserAgentInUi()));
    connect(AvatarFetcher::Instance(),SIGNAL(SmallAvatarFetched(QString)),this,SLOT(AvatarFetched(QString))); //TODO: as local var, not singleton
}

QString MRIMContact::GetTooltip()
{
    QString toolTip = "<table><tr><td><img src='"+m_Status.GetIconPath()+"'/> ";

    if (!m_Name.isEmpty())
    {
        toolTip+="<b>"+Qt::escape(m_Name)+"</b> ("+m_Email+")<br />";
    }
    else
    {
        toolTip+="<b>"+m_Email+"</b><br />";
    }

    if (!m_Status.GetTitle().isEmpty())
    {
        toolTip+= "<b>"+m_Status.GetTitle();

        if (!m_Status.GetDescription().isEmpty())
        {
            toolTip+= " - "+m_Status.GetDescription();
        }
        toolTip+="</b><br />";
    }

    toolTip+="<font size='2'>";

    if (!m_UserAgent.IsEmpty())
    {
        toolTip+= "<b>"+tr("Possible client:")+"</b> "+m_UserAgent.HumanReadable()+"<br />";
    }

    toolTip+="</font>";
    toolTip+="</td><td>";

    if (HasAvatar())
    {
        toolTip+="<img src='"+AvatarFetcher::BigAvatarPath(m_Email)+"' align='right'/>";
    }
    toolTip+="</td></tr></table>";
    return toolTip;
}

MRIMContact::~MRIMContact()
{
    disconnect(AvatarFetcher::Instance(),SIGNAL(SmallAvatarFetched(const QString&)),this,SLOT(AvatarFetched(const QString&)));
    disconnect(&m_Status,SIGNAL(Changed()),this,SLOT(UpdateStatusInUi()));
    disconnect(&m_UserAgent,SIGNAL(Changed()),this,SLOT(UpdateUserAgentInUi()));
}

void MRIMContact::Rename(QString aNewName)
{
    MRIMProto *proto = MRIMPluginSystem::ImplPointer()->FindClientInstance(m_account)->Protocol();

    if (proto && proto->IsOnline())
    {
        m_Name = aNewName;
        TreeModelItem contactItem = GetTreeModel();
        proto->SendModifyContact(m_Email,aNewName,GroupId(),0,MRIMProto::ENoFlags);
        MRIMPluginSystem::PluginSystem()->setContactItemName(contactItem,aNewName);
    }
    else
    {
        QMessageBox::warning(0,tr("Renaming %1").arg(m_Name),tr("You can't rename a contact while you're offline!"),QMessageBox::Ok);
    }
}

bool MRIMContact::HasAvatar()
{
    return QFile::exists(AvatarFetcher::SmallAvatarPath(m_Email));
}

QString MRIMContact::SmallAvatarPath()
{
    return AvatarFetcher::SmallAvatarPath(m_Email);
}

QString MRIMContact::BigAvatarPath()
{
    return AvatarFetcher::BigAvatarPath(m_Email);
}

void MRIMContact::FetchAvatars()
{
    AvatarFetcher::Instance()->FetchSmallAvatar(m_Email);
    AvatarFetcher::Instance()->FetchBigAvatar(m_Email);
}

void MRIMContact::ShowSmallAvatar()
{
    MRIMPluginSystem::ImplPointer()->PluginSystem()->setContactItemIcon(GetTreeModel(),QIcon(AvatarFetcher::SmallAvatarPath(m_Email)),1);
}

TreeModelItem MRIMContact::GetTreeModel()
{
    TreeModelItem cntItem;
    cntItem.m_protocol_name = "MRIM";
    cntItem.m_account_name = m_account;
    cntItem.m_item_name = m_Email;
    cntItem.m_parent_name = (m_GroupId == -1)?"":QString::number(m_GroupId);
    cntItem.m_item_type = EContact;
    return cntItem;
}

void MRIMContact::AvatarFetched(QString aEmail)
{
    if (aEmail != m_Email || !IsInUi()) return;
    ShowSmallAvatar();
}

void MRIMContact::UpdateUserAgentInUi()
{
    if (!IsInUi()) return;
    MRIMPluginSystem::PluginSystem()->setContactItemIcon(GetTreeModel(),m_UserAgent.GetIcon(),12);
}

void MRIMContact::UpdateStatusInUi()
{
    if (!IsInUi()) return;

    if (!IsPurePhoneCnt())
    {
        QString statusText = m_Status.GetTitle();

        if (m_showStatusText && !statusText.isEmpty())
        {
            if (!m_Status.GetDescription().isEmpty())
            {
                statusText.append(" - ");
                statusText.append(m_Status.GetDescription());
            }
            QList<QVariant> textList;
            textList.append("       "+statusText);
            MRIMPluginSystem::PluginSystem()->setContactItemRow(GetTreeModel(), textList, 1);
        }
        else
        {
            QList<QVariant> list;
            MRIMPluginSystem::PluginSystem()->setContactItemRow(GetTreeModel(), list, 1);
        }
        MRIMPluginSystem::PluginSystem()->setContactItemStatus(GetTreeModel(),m_Status.GetIcon(),"",m_Status.GetMass());
    }
    else
    {
        MRIMPluginSystem::PluginSystem()->setContactItemStatus(GetTreeModel(),Icon("phone_mobile"),"",Status::GetMass(STATUS_UNDETERMINATED));
    }
}

void MRIMContact::UpdateAuthInUi()
{
    if (!IsInUi()) return;
    MRIMPluginSystem::PluginSystem()->setContactItemIcon(GetTreeModel(),(!IsAuthedMe() && !IsPurePhoneCnt()) ? Icon("auth") : QIcon(),5);
}

void MRIMContact::SyncWithUi()
{
    if (!IsInUi())
    {
        MRIMPluginSystem::PluginSystem()->addItemToContactList(GetTreeModel(),m_Name);
        SetIsInUi(true);
    }

    if (HasAvatar())
    {
        ShowSmallAvatar();
    }
    FetchAvatars();
    LoadSettings();
    UpdateStatusInUi();
    UpdateUserAgentInUi();
    UpdateAuthInUi();
}

void MRIMContact::LoadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+MRIMPluginSystem::ImplPointer()->Profile(), "mrimsettings");
    m_showStatusText = settings.value("roster/statustext", true).toBool();
}
