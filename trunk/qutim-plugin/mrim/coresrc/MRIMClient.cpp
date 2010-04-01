/*****************************************************************************
    MRIMClient

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

#include <qutim/iconmanagerinterface.h>
#include "MRIMClient.h"
#include "proto.h"
#include "../uisrc/authwidget.h"
#include "../uisrc/addcontactwidget.h"
#include "../uisrc/mrimsearchwidget.h"
#include "../uisrc/searchresultswidget.h"
#include "../uisrc/contactdetails.h"
#include "../uisrc/movetogroupwidget.h"
#include "../uisrc/renamewidget.h"
#include "../uisrc/smswidget.h"
#include "../uisrc/addnumberwidget.h"
#include "../uisrc/filetransferrequest.h"
#include "../uisrc/filetransferwidget.h"
#include "../uisrc/editaccount.h"
#include "statusmanager.h"
#include "mrimeventhandler.h"
#include "MRIMUtils.h"
#include <QUrl>
#include <QDesktopServices>
#include <QNetworkInterface>

MRIMClient::MRIMClient(QString accountName, QString profileName, PluginSystemInterface *pluginSystem, QHBoxLayout *protoButtonLayout)
: m_accountButtonMenu(0), m_statusGroup(0)
{
    m_accountName = accountName;
    m_profileName = profileName;
    m_phoneCntCounter = 0;
    LoadSettings();
    m_protoInstance = new MRIMProto(m_profileName,m_proxy);
    m_accountButton = NULL;
    m_protoButtonLayout = protoButtonLayout;
    m_pluginSystem = pluginSystem; 
    m_isAccountItemAdded = false;
    m_isAddingContact = false;	
    m_updateAccSettingsOnNextLogon = false;
    m_inAutoAway = false;
    m_contextCntMenu = NULL;
    m_actRemoveCnt = NULL;
    m_actAuthorizeCnt = NULL;
    m_actRequestAuthFromCnt = NULL;
    m_actRenameCnt = NULL;
    m_actMoveToGroup = NULL;
    m_actAddToList = NULL;
    m_actSearchCnts = NULL;
    m_actSendSms = NULL;
    m_actAddNumber = NULL;
    m_separator = NULL;
    m_menuTitle = NULL;
    m_menuLabel = NULL;
    m_searchCntsWidget = new MRIMSearchWidget(this);
    m_searchResWidget = new SearchResultsWidget(this);
    m_cntDetailsWidget = new ContactDetails(this);
    m_moveToGroupWidget = new MoveToGroupWidget;
    m_smsWidget = new SMSWidget(this);
    m_addNumberWidget = new AddNumberWidget(this);
    connect(m_moveToGroupWidget,SIGNAL(MoveContactToGroup(QString,QString)),this,SLOT(MoveContact(QString,QString)));
    connect(m_protoInstance,SIGNAL(NotifyUI(QString)),this,SLOT(HandleNotifyUI(QString)));
    connect(m_protoInstance,SIGNAL(LogoutReceived(LogoutReason)),this,SLOT(HandleLogoutReceived(LogoutReason))); //exception
}

MRIMClient::~MRIMClient()
{
    delete m_searchResWidget;
    delete m_searchCntsWidget;
    delete m_cntDetailsWidget;
    delete m_moveToGroupWidget;
    delete m_smsWidget;
    delete m_protoInstance;
    delete m_addNumberWidget;
}

void MRIMClient::LoadSettings()
{
    m_settings = new QSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_accountName, "accountsettings");
    m_login = m_settings->value("main/login").toString();
    m_pass = m_settings->value("main/password").toString();

    LoadAccountSettings();

    if (m_host == "")
    {
        m_host = "mrim.mail.ru";
    }

    if (m_port == 0)
    {
        m_port = 2042;
    }
}

void MRIMClient::LoadAccountSettings()
{
    QSettings account_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_login,"accountsettings");
    QSettings profile_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName, "mrimsettings");

    bool useProfileDefaults = account_settings.value("main/useProfileDefaults",true).toBool();

    QString defaultHost = profile_settings.value("main/host","mrim.mail.ru").toString();
    m_host = (useProfileDefaults) ? defaultHost : account_settings.value("main/host",defaultHost).toString();

    quint32 defaultPort = profile_settings.value("main/port",2042).toUInt();
    m_port = (useProfileDefaults) ? defaultPort : account_settings.value("main/port",defaultPort).toUInt();

    bool defaultUseProxy = profile_settings.value("main/useProxy").toBool();
    bool useProxy = (useProfileDefaults) ? defaultUseProxy : account_settings.value("main/useProxy",defaultUseProxy).toBool();

    bool defaultshowPhoneCnts = profile_settings.value("main/phoneCnts").toBool();
    m_showPhoneCnts = (useProfileDefaults) ? defaultshowPhoneCnts : account_settings.value("main/phoneCnts",defaultshowPhoneCnts).toBool();

    if (useProxy)
    {
        QNetworkProxy::ProxyType defaultProxyType = (QNetworkProxy::ProxyType)profile_settings.value("main/proxyType",QNetworkProxy::NoProxy).toUInt();
        QNetworkProxy::ProxyType proxyType = (useProfileDefaults) ? defaultProxyType : (QNetworkProxy::ProxyType)account_settings.value("main/proxyType",defaultProxyType).toUInt();
        m_proxy.setType(proxyType);

        QString defaultProxyHostName = profile_settings.value("main/proxyHost").toString();
        m_proxy.setHostName((useProfileDefaults) ? defaultProxyHostName : account_settings.value("main/proxyHost",defaultProxyHostName).toString());

        quint32 defaultProxyPort = profile_settings.value("main/proxyPort").toUInt();
        m_proxy.setPort((useProfileDefaults) ? defaultProxyPort : account_settings.value("main/proxyPort",defaultProxyPort).toUInt());

        QString defaultProxyUser = profile_settings.value("main/proxyUser").toString();
        m_proxy.setUser((useProfileDefaults) ? defaultProxyUser : account_settings.value("main/proxyUser",defaultProxyUser).toString());

        QString defaultPassword = profile_settings.value("main/proxyPass").toString();
        m_proxy.setPassword((useProfileDefaults) ? defaultPassword : account_settings.value("main/proxyPass",defaultPassword).toString());
    }
    else
    {
        m_proxy.setType(QNetworkProxy::NoProxy);
    }
}

const TreeModelItem MRIMClient::AccountItem()
{
    TreeModelItem accountItem;
    accountItem.m_account_name = m_accountName;
    accountItem.m_protocol_name = "MRIM";
    accountItem.m_item_name = m_accountName;
    accountItem.m_item_type = EAccount;
    accountItem.m_parent_name = "";
    return accountItem;
}

void MRIMClient::RemoveAccountButton()
{
    if (!m_protoButtonLayout || !m_accountButton) return;
    m_accountButton->setVisible(false);
    m_protoButtonLayout->removeWidget(m_accountButton);
}

void MRIMClient::ClearCL(CLItemType aDepth, bool aDeleteFromSettings)
{
    QList<MRIMCLItem*>* cl = m_protoInstance->GetAllCL();
    if (!cl) return;
    TreeModelItem treeItem;
    treeItem.m_account_name = m_accountName;
    treeItem.m_protocol_name = "MRIM";

    if (aDepth >= EContact)
    {
        foreach (MRIMCLItem* clItem, *cl)
        {
            treeItem.m_item_type = clItem->Type();
            if (clItem->Type() == EContact)
            {
                MRIMContact* cnt = reinterpret_cast<MRIMContact*>(clItem);
                treeItem.m_item_name = cnt->Email();
                if (cnt->GroupId() != -1)
                {
                    treeItem.m_parent_name = QString::number(cnt->GroupId());
                }
                else { treeItem.m_parent_name = ""; }
            }
            m_pluginSystem->removeItemFromContactList(treeItem);
            if (aDeleteFromSettings)
            {
                DeleteFromLocalSettings(EContact,treeItem.m_item_name);
            }
        }
    }

    if (aDepth >= EGroup)
    {
        QList<MRIMGroup*> groups = m_protoInstance->GetAllGroups();
        treeItem.m_item_type = EGroup;
        for (int i = 0; i< groups.count(); i++)
        {
            treeItem.m_item_name = groups.at(i)->Id();
            treeItem.m_parent_name = m_accountName;
            m_pluginSystem->removeItemFromContactList(treeItem);
            if (aDeleteFromSettings)
            {
                DeleteFromLocalSettings(EGroup,treeItem.m_item_name);
            }
        }
    }

    if (aDepth == EAccount)
    {
        m_pluginSystem->removeItemFromContactList(AccountItem());
    }
}

void MRIMClient::CreateAccountButton()
{
    if (m_protoButtonLayout == NULL) return;

    m_accountButton = new QToolButton();
    m_accountButton->setMinimumSize(QSize(22, 22));
    m_accountButton->setMaximumSize(QSize(22, 22));
    m_accountButton->setAutoRaise(true);

    m_accountButtonMenu = new QMenu(); //account menu
    m_statusGroup = new QActionGroup(this); //statuses group

    m_accountButtonMenu->setTitle(m_accountName);

    connect(m_statusGroup, SIGNAL(triggered(QAction*)), this, SLOT(ChangeStatusClicked(QAction*)));
    connect(m_accountButtonMenu, SIGNAL(triggered(QAction*)), this, SLOT(AccountMenuItemClicked(QAction*)));

    m_actUnreadEmails = m_accountButtonMenu->addAction("");
    m_actUnreadEmails->setVisible(false);

    QStringList statusActions;
    statusActions.append(Status::Stringify(STATUS_ONLINE));
    statusActions.append(Status::Stringify(STATUS_USER_DEFINED,"chat"));
    statusActions.append(Status::Stringify(STATUS_AWAY));
    statusActions.append(Status::Stringify(STATUS_USER_DEFINED,"6"));
    statusActions.append(Status::Stringify(STATUS_USER_DEFINED,"38"));
    statusActions.append(Status::Stringify(STATUS_USER_DEFINED,"34"));
    statusActions.append(Status::Stringify(STATUS_USER_DEFINED,"5"));
    statusActions.append(Status::Stringify(STATUS_USER_DEFINED,"18"));
    statusActions.append(Status::Stringify(STATUS_USER_DEFINED,"19"));
    statusActions.append(Status::Stringify(STATUS_USER_DEFINED,"dnd"));
    statusActions.append(Status::Stringify(STATUS_FLAG_INVISIBLE));

    QAction *statusAct = 0;

    foreach (QString status, statusActions)
    {
        statusAct = m_statusGroup->addAction(Status::GetIcon(status),StatusMan().GetTooltip(status));
        statusAct->setData(status);
    }

    m_accountButtonMenu->addActions(m_statusGroup->actions());
    m_accountButtonMenu->addSeparator();

    m_actAddContact = new QAction(tr("Add contact"), this);
    m_actAddContact->setIcon(Icon("add_user",IconInfo::System));
    m_accountButtonMenu->addAction(m_actAddContact);

    m_actOpenMbox = new QAction(tr("Open mailbox"), this);
    m_actOpenMbox->setIcon(Icon("day",IconInfo::System));
    m_accountButtonMenu->addAction(m_actOpenMbox);

    m_actSearchCnts = new QAction(tr("Search contacts"), this);
    m_actSearchCnts->setIcon(Icon("search",IconInfo::System));
    m_accountButtonMenu->addAction(m_actSearchCnts);

    m_accountButtonMenu->addSeparator();   
    QString offlineStatus = Status::Stringify(STATUS_OFFLINE);
    m_accountButtonMenu->addAction(Status::GetIcon(offlineStatus),StatusMan().GetTooltip(offlineStatus),this,SLOT(DisconnectMenuItemClicked()));

    m_accountButton->setToolTip(m_accountName);
    m_accountButton->setMenu(m_accountButtonMenu);
    m_accountButton->setPopupMode(QToolButton::InstantPopup);

#if defined(Q_OS_MAC)
    m_protoButtonLayout->addWidget(m_accountButton, 0, Qt::AlignLeft);
#else
    m_protoButtonLayout->addWidget(m_accountButton, 0, Qt::AlignRight);
#endif

    if (!m_isAccountItemAdded)
    {        
        m_pluginSystem->addItemToContactList(AccountItem(),AccountName());
        m_isAccountItemAdded = true;
    }
    LoadCL();

    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrimsettings");
    bool restoreStatus = settings.value("main/restoreStatus",true).toBool();

    UpdateStatusIcon(m_protoInstance->CurrentStatus());

    if (restoreStatus)
    {
        QSettings acc_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_accountName, "accountsettings");
        QString savedStatusId = acc_settings.value("main/status","STATUS_ONLINE").toString();
        Status *savedStatus = StatusMan().GetCustomStatus(m_accountName,savedStatusId);
        ChangeStatus(*savedStatus);
        delete savedStatus;
    }
}

void MRIMClient::ChangeStatusClicked(QAction* action)
{//status changed
    QString statusId = action->data().toString();
    Status *status = StatusMan().GetCustomStatus(m_accountName,statusId);
    ChangeStatus(*status);
    delete status;
}

void MRIMClient::DisconnectMenuItemClicked()
{
    if (m_protoInstance->IsOnline())
    {
        Status status(STATUS_OFFLINE);
        ChangeStatus(status);
    }
}

void MRIMClient::SelectXStatusesClicked()
{
    //TODO: show statuses selection widget
}

void MRIMClient::AccountMenuItemClicked(QAction* action)
{
    if (action == m_actAddContact && m_protoInstance->IsOnline())
    {
        HandleAddContact();
    }

    if (action == m_actOpenMbox && m_protoInstance->IsOnline())
    {
        if (m_mpopKey.length() > 0)
        {
            //TODO: make pro/win/edu selection as an option
            QUrl mpopUrl("http://pro.mail.ru/cgi-bin/auth?Login="+m_login+"&agent="+m_mpopKey);
            QDesktopServices::openUrl(mpopUrl);
        }
        else
        {
            m_pluginSystem->systemNotifiacation(AccountItem(),tr("No MPOP session available for you, sorry..."));
        }
    }

    if (action == m_actSearchCnts && m_protoInstance->IsOnline())
        m_searchCntsWidget->show();
}

void MRIMClient::HandleAddContact(QString aEmail,QString aNick)
{
    AddContactWidget* cntAddWidget = new AddContactWidget(this);

    if (aEmail.length() > 0)
        cntAddWidget->SetEmail(aEmail,true);
    if (aNick.length() > 0)
        cntAddWidget->SetNick(aNick);

    cntAddWidget->FillGroups();
    cntAddWidget->show();
}

void MRIMClient::ChangeStatus(const Status &aNewStatus)
{
    if (aNewStatus == m_protoInstance->CurrentStatus()) return;
    bool goingOnline = (!m_protoInstance->CurrentStatus().IsOnline() && MRIMProto::IsOnline(aNewStatus));
    
    switch (aNewStatus.Get())
    {
    case STATUS_OFFLINE:
        if (m_protoInstance->IsOnline())
        {
            m_protoInstance->DisconnectFromIM();
        }
        break;
    default:
        if (goingOnline)
        {
            if (m_updateAccSettingsOnNextLogon)
            {
                LoadSettings();
                m_protoInstance->SetProxy(m_proxy);
                m_updateAccSettingsOnNextLogon = false;
            }

            UpdateStatusIcon(Icon("connecting",IconInfo::Status,"mrim"));
            ConnectAllProtoEvents();
            m_protoInstance->Connect(m_login,m_pass,m_host,m_port,aNewStatus);
        }
        else
        {
            m_protoInstance->ChangeStatus(aNewStatus);
        }
        break;
    }
}

void MRIMClient::ChangeStatus( qint32 aNewStatus, const QString &aCustomID)
{
    Status *newStatus = NULL;

    if (aCustomID.length() > 0)
    {
        newStatus = StatusMan().GetCustomStatus(m_accountName,aCustomID);
    }
    else
    {
        newStatus = StatusMan().GetStatus(m_accountName,aNewStatus);
    }
    ChangeStatus(*newStatus);
    delete newStatus;
}

void MRIMClient::HandleProtoStatusChanged( StatusData aNewStatusData )
{
    Status newStatus(aNewStatusData);
    Status &prevStatus = Protocol()->PreviousStatus();
    qDebug()<<"Protocol status changed! Old status: "<< prevStatus.Get() << ". New status: "<<newStatus.Get();
    if (newStatus == prevStatus && newStatus != STATUS_OFFLINE) return;
    bool numStatusChanged = (newStatus.Get() != prevStatus.Get());

    if (newStatus.Get() == STATUS_OFFLINE)
    {
        m_actUnreadEmails->setVisible(false);
        DisconnectAllProtoEvents();
    }

    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_accountName, "accountsettings");
    settings.setValue("main/status",newStatus.Stringify());

    UpdateStatusIcon(newStatus);
}

void MRIMClient::HandleItemAdditionToUI(CLItemType aType, QString aParentId, QString aID, QString aName, StatusData aStatusData, bool aAuthed, bool aIsNew)
{//need to add group, contact or smth else to CL
    QString parentID;
    if (aID == "phone" && !m_showPhoneCnts) return;

    if (aParentId == "-1")
    {
        parentID == "";
    }
    else
    {
        parentID = aParentId;
    }

    TreeModelItem item;
    item.m_account_name = m_accountName;
    item.m_protocol_name = "MRIM";
    item.m_item_name = aID;
    item.m_item_type = aType;

    if (aType == EGroup)
    {
        item.m_parent_name = m_accountName;
    }
    else
    {
        item.m_parent_name = parentID;
    }

    if (m_isAddingContact)
    {
        TreeModelItem oldItem;
        oldItem.m_account_name = m_accountName;
        oldItem.m_protocol_name = "MRIM";
        oldItem.m_item_name = aID;
        oldItem.m_item_type = aType;
        oldItem.m_parent_name = "";

        m_pluginSystem->removeItemFromContactList(oldItem);
        //m_pluginSystem->addItemToContactList(item,aName); //may loose auth icon, TODO
    }

    if (aIsNew || m_isAddingContact)
    {
        m_pluginSystem->addItemToContactList(item,aName);		
        SaveCLItem(aType,item,aName,aAuthed,m_protoInstance->IsContactAuthedMe(item.m_item_name),""); //TODO: add phones support to all contacts
        m_isAddingContact = false;
    }
    else if (!aIsNew) 
    {
        m_pluginSystem->setContactItemName(item,aName);
    }

    if (aType == EContact)
    {
        Status currStat(aStatusData);
        QIcon statusIcon;

        if (aID == "phone")
        {
            statusIcon=m_pluginSystem->getIcon("phone_mobile");
        }
        else
        {
            statusIcon = currStat.GetIcon();
        }
        m_pluginSystem->setContactItemStatus(item,statusIcon,"",currStat.GetMass());

        if (!aAuthed && aID != "phone")
        {
            m_pluginSystem->setContactItemIcon(item,Icon("auth"),5);
        }
        else
        {
            m_pluginSystem->setContactItemIcon(item,QIcon(),5);
        }	
    }
}

void MRIMClient::HandleRemoveItemFromUI(CLItemType aType,QString aParentID,QString aId)
{
    QString parentID = aParentID;
    if (aParentID == "-1")
    {
        parentID == "";
    }

    if (aType == EContact)
    {
        RemoveContactFromCL(aId);
    }
}

void MRIMClient::HandleAccountInfoRecieved(MRIMUserInfo aInfo)
{
    TreeModelItem accountItem = AccountItem();

    m_userInfo = aInfo;
    QString userInfoMessage;
    userInfoMessage.append(tr("Messages in mailbox: ")+aInfo.messagesTotal+"<br>"+tr("Unread messages: ")+aInfo.messagesUnread);
    m_pluginSystem->systemNotifiacation(accountItem,userInfoMessage);
    HandleMailboxStatusChanged(aInfo.messagesUnread.toUInt());
}

void MRIMClient::HandleMessageRecieved(QString aContactEmail, QString aGroupId, QString aMessage, QDateTime aDate, bool/* aIsRtf*/, bool aIsAuth)
{
    QString groupId;
    if (aGroupId == "-1" || aGroupId == "")
    {
        groupId == "";
    }
    else
    {
        groupId = aGroupId;
    }

    TreeModelItem contactItem;
    contactItem.m_account_name = m_accountName;
    contactItem.m_protocol_name = "MRIM";
    contactItem.m_item_name = aContactEmail;
    contactItem.m_item_type = EContact;
    contactItem.m_parent_name = groupId;

    if (aIsAuth)
    {
        authwidget* request = new authwidget(this);
        QString authText = tr("User %1 is requesting authorization:\n").arg(aContactEmail)+aMessage;
        request->SetupAuthRequest(authText,aContactEmail);
        request->show();
    }
    else
    {
        m_pluginSystem->addMessageFromContact(contactItem,aMessage,aDate);
    }
}

void MRIMClient::HandleContactTyping(QString aContactEmail, QString aGroupId)
{
    TreeModelItem contactItem;
    contactItem.m_account_name = m_accountName;
    contactItem.m_protocol_name = "MRIM";
    contactItem.m_item_name = aContactEmail;
    contactItem.m_item_type = EContact;
    contactItem.m_parent_name = aGroupId;

    m_pluginSystem->contactTyping(contactItem,true);
}

void MRIMClient::SendMessageToContact(QString aContactEmail, QString aMessage, int aMessageIconPosition)
{
    if (m_protoInstance && m_protoInstance->IsOnline())
        m_protoInstance->SendMessageToContact(aContactEmail,aMessage,aMessageIconPosition);
}

void MRIMClient::HandleContactTypingStopped(QString aContactEmail, QString aGroupId)
{
    TreeModelItem contactItem;
    contactItem.m_account_name = m_accountName;
    contactItem.m_protocol_name = "MRIM";
    contactItem.m_item_name = aContactEmail;
    contactItem.m_item_type = EContact;
    contactItem.m_parent_name = aGroupId;

    m_pluginSystem->contactTyping(contactItem,false);
}

void MRIMClient::HandleMessageDelivered(QString aContactEmail, QString aGroupId, quint32 aKernelMessageId)
{
    if (aGroupId == "-1")
    {
        aGroupId == "";
    }

    TreeModelItem contactItem;
    contactItem.m_account_name = m_accountName;
    contactItem.m_protocol_name = "MRIM";
    contactItem.m_item_name = aContactEmail;
    contactItem.m_item_type = EContact;
    contactItem.m_parent_name = aGroupId;

    m_pluginSystem->messageDelievered(contactItem,aKernelMessageId);
}

void MRIMClient::ConnectAllProtoEvents()
{
    connect(m_protoInstance,SIGNAL(ProtoStatusChanged(StatusData)),this,SLOT(HandleProtoStatusChanged(StatusData)));
    connect(m_protoInstance,SIGNAL(AddItemToUI(CLItemType,QString,QString,QString,StatusData,bool,bool)),this,SLOT(HandleItemAdditionToUI(CLItemType,QString,QString,QString,StatusData,bool,bool)));
    connect(m_protoInstance,SIGNAL(AccountInfoRecieved(MRIMUserInfo)),this,SLOT(HandleAccountInfoRecieved(MRIMUserInfo)));
    connect(m_protoInstance,SIGNAL(MessageRecieved(QString,QString,QString,QDateTime,bool,bool)),this,SLOT(HandleMessageRecieved(QString,QString,QString,QDateTime,bool,bool)));
    connect(m_protoInstance,SIGNAL(ContactTyping(QString,QString)),this,SLOT(HandleContactTyping(QString,QString)));
    connect(m_protoInstance,SIGNAL(ContactTypingStopped(QString,QString)),this,SLOT(HandleContactTypingStopped(QString,QString)));
    connect(m_protoInstance,SIGNAL(MessageDelivered(QString,QString,quint32)),this,SLOT(HandleMessageDelivered(QString,QString,quint32)));
    connect(m_protoInstance,SIGNAL(AuthorizeResponseReceived(QString,QString)),this,SLOT(HandleAuthorizeResponseReceived(QString,QString)));
    connect(m_protoInstance,SIGNAL(MailboxStatusChanged(quint32)),this,SLOT(HandleMailboxStatusChanged(quint32)));
    connect(m_protoInstance,SIGNAL(MPOPKeyReceived(QString)),this,SLOT(HandleMPOPKeyReceived(QString)));
    connect(m_protoInstance,SIGNAL(CLOperationFailed(CLOperationError)),this,SLOT(HandleCLOperationFailed(CLOperationError)));	
    connect(m_protoInstance,SIGNAL(SearchFinished(QList<MRIMSearchParams*>)),this,SLOT(HandleSearchFinished(QList<MRIMSearchParams*>)));
    connect(m_protoInstance,SIGNAL(RemoveItemFromUI(CLItemType,QString,QString)),this,SLOT(HandleRemoveItemFromUI(CLItemType,QString,QString)));
    connect(m_protoInstance,SIGNAL(NewCLReceived()),this,SLOT(HandleNewCLReceived()));
    connect(m_protoInstance,SIGNAL(FileTransferRequested(FileTransferRequest)),this,SLOT(HandleFileTransferRequest(FileTransferRequest)));
}

void MRIMClient::DisconnectAllProtoEvents()
{
    disconnect(m_protoInstance,SIGNAL(ProtoStatusChanged(StatusData)),this,SLOT(HandleProtoStatusChanged(StatusData)));
    disconnect(m_protoInstance,SIGNAL(AddItemToUI(CLItemType,QString,QString,QString,StatusData,bool,bool)),this,SLOT(HandleItemAdditionToUI(CLItemType,QString,QString,QString,StatusData,bool,bool)));
    disconnect(m_protoInstance,SIGNAL(AccountInfoRecieved(MRIMUserInfo)),this,SLOT(HandleAccountInfoRecieved(MRIMUserInfo)));
    disconnect(m_protoInstance,SIGNAL(MessageRecieved(QString,QString,QString,QDateTime,bool,bool)),this,SLOT(HandleMessageRecieved(QString,QString,QString,QDateTime,bool,bool)));
    disconnect(m_protoInstance,SIGNAL(ContactTyping(QString,QString)),this,SLOT(HandleContactTyping(QString,QString)));
    disconnect(m_protoInstance,SIGNAL(ContactTypingStopped(QString,QString)),this,SLOT(HandleContactTypingStopped(QString,QString)));
    disconnect(m_protoInstance,SIGNAL(MessageDelivered(QString,QString,quint32)),this,SLOT(HandleMessageDelivered(QString,QString,quint32)));
    disconnect(m_protoInstance,SIGNAL(AuthorizeResponseReceived(QString,QString)),this,SLOT(HandleAuthorizeResponseReceived(QString,QString)));
    disconnect(m_protoInstance,SIGNAL(MailboxStatusChanged(quint32)),this,SLOT(HandleMailboxStatusChanged(quint32)));
    disconnect(m_protoInstance,SIGNAL(MPOPKeyReceived(QString)),this,SLOT(HandleMPOPKeyReceived(QString)));
    disconnect(m_protoInstance,SIGNAL(CLOperationFailed(CLOperationError)),this,SLOT(HandleCLOperationFailed(CLOperationError)));	
    disconnect(m_protoInstance,SIGNAL(SearchFinished(QList<MRIMSearchParams*>)),this,SLOT(HandleSearchFinished(QList<MRIMSearchParams*>)));
    disconnect(m_protoInstance,SIGNAL(RemoveItemFromUI(CLItemType,QString,QString)),this,SLOT(HandleRemoveItemFromUI(CLItemType,QString,QString)));
    disconnect(m_protoInstance,SIGNAL(NewCLReceived()),this,SLOT(HandleNewCLReceived()));
    disconnect(m_protoInstance,SIGNAL(FileTransferRequested(FileTransferRequest)),this,SLOT(HandleFileTransferRequest(FileTransferRequest)));
}

void MRIMClient::HandleAuthorizeResponseReceived(QString aContactEmail, QString aGroupId)
{
    if (aGroupId == "-1")
    {
        aGroupId == "";
    }

    TreeModelItem accountItem = AccountItem();

    QString authedMessage(tr("Authorization request accepted by ")+aContactEmail);
    m_pluginSystem->systemNotifiacation(accountItem,authedMessage);

    TreeModelItem cntItem;
    cntItem.m_account_name = m_accountName;
    cntItem.m_protocol_name = "MRIM";
    cntItem.m_item_name = aContactEmail;
    cntItem.m_item_type = EContact;
    cntItem.m_parent_name = aGroupId;

    m_pluginSystem->setContactItemIcon(cntItem,QIcon(),5);
    MRIMContact* cnt = m_protoInstance->GetCnt(aContactEmail);
    cnt->SetAuthedMe(true);
}

void MRIMClient::UpdateSettings()
{
    m_updateAccSettingsOnNextLogon = true;
	MRIMContactList *cl = m_protoInstance->GetContactList();
	
	if (cl)
	{
		cl->UpdateContactList();
	}
}

void MRIMClient::SaveCLItem(CLItemType aItemType, TreeModelItem aItem, QString aName, bool aisAuthed, bool aIsAuthedMe, QString aPhone)
{
    QSettings clSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_accountName, "contactlist");
    QStringList groups = clSettings.value("cl/groups").toStringList();
    QStringList contacts = clSettings.value("cl/contacts").toStringList();

    if (aItemType == EGroup)
    {
        if (!groups.contains(aItem.m_item_name))
        {
            groups<<aItem.m_item_name;
            clSettings.setValue("cl/groups",groups);
        }

        clSettings.beginGroup(aItem.m_item_name);
        clSettings.setValue("id",aItem.m_item_name);
        clSettings.setValue("name",aName);
        clSettings.endGroup();	
    }

    if (aItemType == EContact)
    {
        MRIMContact* cnt = m_protoInstance->GetContactByEmail(aItem.m_item_name);
        QString contEntry = aItem.m_item_name;
        if (contEntry == "phone")
        {			
            contEntry+= QString::number(m_phoneCntCounter);
            m_phoneCntCounter++;
        }
        if (!contacts.contains(contEntry))
        {
            contacts<<contEntry;
            clSettings.setValue("cl/contacts",contacts);
        }

        clSettings.beginGroup(contEntry);
        if (cnt)
        {
            clSettings.setValue("id",cnt->Id());
        }
        clSettings.setValue("email",aItem.m_item_name);
        clSettings.setValue("name",aName);
        clSettings.setValue("groupId",aItem.m_parent_name);
        clSettings.setValue("authed",aisAuthed);
        clSettings.setValue("authedMe",aIsAuthedMe);
        clSettings.setValue("phone",aPhone);
        clSettings.endGroup();	
    }
}

void MRIMClient::HandleNewCLReceived()
{
    ClearCL(EGroup);
    QSettings clSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_accountName, "contactlist");
    QFile::remove(clSettings.fileName());
}

void MRIMClient::LoadCL()
{
    QSettings clSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_accountName, "contactlist");
    QStringList groups = clSettings.value("cl/groups").toStringList();
    QStringList contacts = clSettings.value("cl/contacts").toStringList();

    foreach (QString groupId,groups)
    {
        QString id = clSettings.value(groupId+"/id").toString();
        QString name = clSettings.value(groupId+"/name").toString();
        //HandleItemAdditionToUI(EGroup,m_accountName,id,name,STATUS_UNDETERMINATED,true,true);
        m_protoInstance->AddGroup(name,id.toUInt());
    }

    foreach (QString cntEmail,contacts)
    {
        QString email = clSettings.value(cntEmail+"/email").toString();
        QString name = clSettings.value(cntEmail+"/name").toString();
        QString grId = clSettings.value(cntEmail+"/groupId").toString();
        bool authed = clSettings.value(cntEmail+"/authed").toBool();
        bool authedMe = clSettings.value(cntEmail+"/authedMe").toBool();
        Q_UNUSED(authedMe);
        quint32 status = STATUS_OFFLINE;
        if (email.contains("phone"))
        {
            email = "phone";
            status = STATUS_ONLINE;
        }
        //HandleItemAdditionToUI(EContact,grId,email,name,status,authedMe,true);
        m_protoInstance->AddContact(email,name,grId.toUInt(),authed,true);
    }
}

void MRIMClient::HandleLogoutReceived(LogoutReason aReason)
{
    QString logountMsg;
    switch (aReason)
    {
    case EAuthenticationFailed:
        logountMsg = tr("Server closed the connection. Authentication failed!");
        break;
    case EAnotherClientConnected:
        logountMsg = tr("Server closed the connection. Another client with same login connected!");
        break;
    case EUnknownReason:
        logountMsg = tr("Server closed the connection for unknown reason...");
        break;
    }

    TreeModelItem accountItem = AccountItem();

    m_pluginSystem->systemNotifiacation(accountItem,logountMsg);
}

void MRIMClient::HandleMailboxStatusChanged(quint32 aUnreadMessages)
{
    m_actUnreadEmails->setText(tr("Unread emails: %1").arg(aUnreadMessages));
    m_actUnreadEmails->setVisible(true);
    m_actUnreadEmails->setEnabled(false);
//  Deprecated
//
//  TreeModelItem accountItem = AccountItem();
//
//  QString mboxStatusMsg = tr("Mailbox status changed!")+"\n"+tr("Unread messages: ")+QString::number(aUnreadMessages);
//  m_pluginSystem->systemNotifiacation(accountItem,mboxStatusMsg);
}

void MRIMClient::HandleMPOPKeyReceived(QString aMPOPKey)
{
    m_mpopKey = aMPOPKey;
}

void MRIMClient::ShowCntContextPopup(const QList<QAction*> &aDefActList,TreeModelItem aItemData, const QPoint &aMenuPoint)
{
    if (!m_protoInstance->IsOnline()) return;

    if (aItemData.m_item_type == EContact)
    {
        MRIMContact* cnt = Protocol()->GetCnt(aItemData.m_item_name);
        if (!m_contextCntMenu)
        {
            m_contextCntMenu = new QMenu();
            connect(m_contextCntMenu, SIGNAL(triggered(QAction*)), this, SLOT(CntContextMenuClicked(QAction*)));
            m_menuTitle = new QWidgetAction(this);
            m_menuLabel = new QLabel;
            m_menuLabel->setAlignment(Qt::AlignCenter);
            m_menuTitle->setDefaultWidget(m_menuLabel);
        }
        m_contextCntMenu->clear();
        m_contextCntMenu->addAction(m_menuTitle);
        m_menuLabel->setText("<b>"+ Qt::escape(cnt->Name()) +"</b>");

        if (cnt->InList() && cnt->HasPhone())
        {
            if (!m_actSendSms)
            {
                m_actSendSms = new QAction(tr("Send SMS"), this);
                m_actSendSms->setIcon(m_pluginSystem->getIcon("phone_mobile"));
            }
            m_actSendSms->setData(aItemData.m_item_name);
            m_contextCntMenu->addAction(m_actSendSms);
        }

        m_contextCntMenu->addAction(aDefActList.at(0));
        m_contextCntMenu->addAction(aDefActList.at(1));
        m_contextCntMenu->addAction(aDefActList.at(2));

        if (!m_separator)
        {
            m_separator = new QAction(this);
            m_separator->setSeparator(true);
        }
        m_contextCntMenu->addAction(m_separator);


        if (cnt->InList() && !cnt->IsAuthed())
        {
            if (!m_actAuthorizeCnt)
            {
                m_actAuthorizeCnt = new QAction(tr("Authorize contact"), this);
                m_actAuthorizeCnt->setIcon(m_pluginSystem->getIcon("auth"));
            }
            m_actAuthorizeCnt->setData(aItemData.m_item_name);
            m_contextCntMenu->addAction(m_actAuthorizeCnt);
        }

        if (cnt->InList() && !cnt->IsAuthedMe())
        {
            if (!m_actRequestAuthFromCnt)
            {
                m_actRequestAuthFromCnt = new QAction(tr("Request authorization"), this);
                m_actRequestAuthFromCnt->setIcon(m_pluginSystem->getIcon("auth"));
            }
            m_actRequestAuthFromCnt->setData(aItemData.m_item_name);
            m_contextCntMenu->addAction(m_actRequestAuthFromCnt);
        }

        if (cnt->InList())
        {
            if (!m_actRenameCnt)
            {
                m_actRenameCnt = new QAction(tr("Rename contact"), this);
                m_actRenameCnt->setIcon(m_pluginSystem->getIcon("edituser"));
            }
            m_actRenameCnt->setData(aItemData.m_item_name);
            m_contextCntMenu->addAction(m_actRenameCnt);
        }

        if (!m_actRemoveCnt)
        {
            m_actRemoveCnt = new QAction(tr("Delete contact"), this);
            m_actRemoveCnt->setIcon(m_pluginSystem->getIcon("deleteuser"));
        }
        m_actRemoveCnt->setData(aItemData.m_item_name);
        m_contextCntMenu->addAction(m_actRemoveCnt);

        if (!m_actMoveToGroup)
        {
            m_actMoveToGroup = new QAction(tr("Move to group"), this);
            m_actMoveToGroup->setIcon(m_pluginSystem->getIcon("moveuser"));
        }
        m_actMoveToGroup->setData(aItemData.m_item_name);
        m_contextCntMenu->addAction(m_actMoveToGroup);

        if (cnt->InList() && !cnt->HasPhone())
        {
            if (!m_actAddNumber)
            {
                m_actAddNumber = new QAction(tr("Add phone number"),this);
                m_actAddNumber->setIcon(m_pluginSystem->getIcon("phone_unknown"));
            }
            m_actAddNumber->setData(aItemData.m_item_name);
            m_contextCntMenu->addAction(m_actAddNumber);
        }

        if (!cnt->InList())
        {
            if (!m_actAddToList)
            {
                m_actAddToList = new QAction(tr("Add to list"), this);
                m_actAddToList->setIcon(m_pluginSystem->getIcon("add_user"));
            }
            m_actAddToList->setData(aItemData.m_item_name);
            m_contextCntMenu->addAction(m_actAddToList);
        }

        m_contextCntMenu->addSeparator();
        int actionsCount = aDefActList.count() - 3;
        if ( actionsCount > 0 )
        {
            for ( int i = 0; i < actionsCount; i++ )
            {
                m_contextCntMenu->addAction(aDefActList.at(3 + i));
            }
        }

        m_contextCntMenu->popup(aMenuPoint);
    }

}

void MRIMClient::CntContextMenuClicked(QAction* aAction)
{
    QString cntEmail = aAction->data().toString();
    MRIMContact* cnt = m_protoInstance->GetContactByEmail(cntEmail);
    if (!cnt || !m_protoInstance->IsOnline()) return;

    if (aAction == m_actRemoveCnt)
    {
        RemoveContactFromCL(cntEmail);
    }

    if (aAction == m_actAuthorizeCnt)
    {
        m_protoInstance->SendAuthorizationTo(cntEmail);
    }

    if (aAction == m_actRequestAuthFromCnt)
    {
        QString authMsg = tr("Pls authorize and add me to your contact list! Thanks! Email: ")+m_accountName;
        m_protoInstance->SendMessageToContact(cnt->Email(),authMsg,0,true);
    }

    if (aAction == m_actRenameCnt)
    {
        RenameWidget* renameWidget = new RenameWidget;
        renameWidget->show(cnt);
    }

    if (aAction == m_actAddToList)
    {
        MRIMSearchParams params;
        QStringList addr = cnt->Email().split("@");
        params.EmailAddr = addr[0];
        params.EmailDomain = addr[1];
        Protocol()->StartSearch(params);
    }

    if (aAction == m_actMoveToGroup)
    {
        m_moveToGroupWidget->show(cnt->Email(),m_protoInstance->GetAllGroups());
    }

    if (aAction == m_actSendSms)
    {
        m_smsWidget->show(cnt);
    }

    if (aAction == m_actAddNumber)
    {
        m_addNumberWidget->show(cnt);
    }
}

void MRIMClient::RemoveContactFromCL(QString aCntId)
{
    MRIMContact* cnt = m_protoInstance->GetContactByEmail(aCntId);
    if (!cnt) return;
    QString parent = QString::number(cnt->GroupId());
    if (parent == "-1")
    {
        parent == "";
    }

    TreeModelItem contactItem;
    contactItem.m_account_name = m_accountName;
    contactItem.m_protocol_name = "MRIM";
    contactItem.m_item_name = aCntId;
    contactItem.m_item_type = EContact;
    contactItem.m_parent_name = parent;

    if (cnt->InList())
    {
        m_protoInstance->RemoveUserFromCL(aCntId);
    }
    m_pluginSystem->removeItemFromContactList(contactItem);
    DeleteFromLocalSettings(EContact,aCntId);
}

void MRIMClient::DeleteFromLocalSettings(CLItemType aType, QString aId)
{
    QSettings clSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_accountName, "contactlist");
    if (aType == EContact)
    {
        QStringList contacts = clSettings.value("cl/contacts").toStringList();
        contacts.removeAll(aId);
        clSettings.remove(aId);
        clSettings.setValue("cl/contacts",contacts);
    }
    else if (aType == EGroup)
    {
        QStringList groups = clSettings.value("cl/groups").toStringList();
        groups.removeAll(aId);
        clSettings.remove(aId);
        clSettings.setValue("cl/contacts",groups);
    }
}

void MRIMClient::HandleCLOperationFailed(CLOperationError aClErrCode)
{
    TreeModelItem accountItem = AccountItem();

    QString clOperErrorMsg = tr("Contact list operation failed!")+"\n";
    switch (aClErrCode)
    {
    case ECLNoSuchUser:
        clOperErrorMsg += tr("No such user!");
        break;
    case ECLInternalServerError:
        clOperErrorMsg += tr("Internal server error!");
        break;
    case ECLInvalidInfo:
        clOperErrorMsg += tr("Invalid info provided!");
        break;
    case ECLUserAlreadyExists:
        clOperErrorMsg += tr("User already exists!");
        break;
    case ECLGroupLimitReached:
        clOperErrorMsg += tr("Group limit reached!");
        break;
    default:
        clOperErrorMsg += tr("Unknown error!");
        break;
    }

    m_pluginSystem->systemNotifiacation(accountItem,clOperErrorMsg);
}

ContactAdditionalInfo MRIMClient::GetContactAdditionalInfo(QString aEmail)
{	
    ContactAdditionalInfo info;

    MRIMContact* cnt = m_protoInstance->GetContactByEmail(aEmail);

    if (cnt)
    {
        info.Nick = cnt->Name();
        info.ClientName = cnt->GetUserAgent().HumanReadable();
        info.AvatarPath = (cnt->HasAvatar())?cnt->BigAvatarPath() : "";
    }
    return info;
}

MRIMUserInfo MRIMClient::GetUserInfo()
{
    return m_userInfo;
}

void MRIMClient::HandleSearchFinished(QList<MRIMSearchParams*> aFoundList)
{
    qint32 cntsCount = aFoundList.count();
    m_searchCntsWidget->SearchFinished(cntsCount);

    if (cntsCount > 1)
    {
        m_searchResWidget->Reset();
        m_searchResWidget->show(aFoundList,m_searchCntsWidget->ShowAvatars());
    }
    else if (cntsCount == 1)
    {
        m_cntDetailsWidget->show(*aFoundList.at(0));        
        delete aFoundList.at(0);
    }
    else
    {
        TreeModelItem accountItem = AccountItem();
        QString msg = tr("Sorry, no contacts found :(\n Try to change search parameters");
        m_pluginSystem->systemNotifiacation(accountItem,msg);
    }
}

void MRIMClient::ShowContactDetails(QString aEmail)
{
    m_protoInstance->RequestCntInfo(aEmail);
}

AccountStructure MRIMClient::GetAccountInfo()
{
    AccountStructure account;
    account.account_name = m_accountName;
    account.protocol_icon = m_protoInstance->CurrentStatus().GetIcon();
    account.protocol_name = "MRIM";
    return account;
}

void MRIMClient::SetAutoAway()
{
    if (!m_inAutoAway && m_protoInstance->IsOnline())
    {
        m_inAutoAway = true;        
        ChangeStatus(STATUS_AWAY);
    }
}

void MRIMClient::RestoreFromAutoAway()
{
    if (m_inAutoAway && m_protoInstance->IsOnline())
    {
        Status prevStatus(m_protoInstance->PreviousStatus().GetData());
        ChangeStatus(prevStatus);
        m_inAutoAway = false;
    }
}

//CL translators
QString MRIMClient::GetItemToolTip(const QString &aCntId)
{
    QString toolTip;
    MRIMContact* cnt = m_protoInstance->GetContactByEmail(aCntId);
    if (cnt)
    {
        toolTip = cnt->GetTooltip();
    }
    return toolTip;
}

void MRIMClient::MoveContact(QString aCntId, QString aNewGroup)
{
    MRIMContact* cnt = m_protoInstance->GetCnt(aCntId);

    if (cnt && m_protoInstance->IsOnline())
    {
        QString parent = QString::number(cnt->GroupId());
        if (parent == "-1")
        {
            parent = "";
        }
        TreeModelItem contactItem;
        contactItem.m_account_name = m_accountName;
        contactItem.m_protocol_name = "MRIM";
        contactItem.m_item_name = aCntId;
        contactItem.m_item_type = EContact;
        contactItem.m_parent_name = parent;

        TreeModelItem contactNewItem;
        contactNewItem.m_account_name = m_accountName;
        contactNewItem.m_protocol_name = "MRIM";
        contactNewItem.m_item_name = aCntId;
        contactNewItem.m_item_type = EContact;
        contactNewItem.m_parent_name = aNewGroup;

        m_pluginSystem->moveItemInContactList(contactItem,contactNewItem);     
        if (cnt->InList())
        {
            m_protoInstance->SendModifyContact(aCntId,cnt->Name(),aNewGroup.toInt(),0,MRIMProto::ENoFlags);
        }
        else
        {
            m_protoInstance->AddContact(cnt->Email(),cnt->Email(),cnt->GroupId(),true,false);
        }
    }
}

void MRIMClient::RenameContact(QString aEmail, QString aNewName)
{    
    m_protoInstance->GetCnt(aEmail)->Rename(aNewName);
}

void MRIMClient::HandleNotifyUI(QString aMessage)
{
    m_pluginSystem->systemNotifiacation(AccountItem(),aMessage);
}

void MRIMClient::HandleFileTransferRequest(FileTransferRequest aReq)
{
    FileTransferRequestWidget* frWidget = new FileTransferRequestWidget(this,aReq);
    frWidget->show();
}

void MRIMClient::SendFileTo(QString aTo, QStringList aFiles)
{
    FileTransferRequest req;
    req.UniqueId = ( (double)qrand() / (double)RAND_MAX )*MAX_INT32;
    req.To = aTo;
    req.SummarySize = 0;

    foreach (QString file, aFiles)
    {
        QFileInfo info(file);
        if (info.exists())
        {
            req.FilesDict.insert(info.fileName(),info.size());
            req.FilesInfo.append(info);
            req.SummarySize += info.size();
        }
    }

    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+m_accountName, "accountsettings");
    bool ok;
    qint32 ftPort = settings.value("ftPort").toInt(&ok);
    if (!ok || ftPort == 0)
    {
        ftPort = 2040;
    }

    QList<QHostAddress> IPs = QNetworkInterface::allAddresses();
    foreach (QHostAddress ip, IPs)
    {
        QString hostIPString = ip.toString();
        if (!ip.isNull() && !hostIPString.contains(':'))
        {
            req.IPsDict.insert(hostIPString,ftPort);
        }
    }
    req.IPsDict.insert(GetUserInfo().userClientEndpoint.split(':')[0],ftPort);
    Protocol()->SendFileTransferRequest(req);
    FileTransferWidget* ftWindow = new FileTransferWidget(this,req);
    ftWindow->show();
}

void MRIMClient::ShowEditAccountWindow()
{
    EditAccount* editAccountWidget = new EditAccount(this);
    editAccountWidget->show();
}

void MRIMClient::UpdateStatusIcon( const Status &newStatus )
{
    QIcon newIcon = newStatus.GetIcon();
    UpdateStatusIcon(newIcon);
}

void MRIMClient::UpdateStatusIcon( const QIcon &newIcon )
{
    m_accountButton->setIcon(newIcon);
    m_accountButtonMenu->setIcon(newIcon);
    m_pluginSystem->updateStatusIcons();
}
