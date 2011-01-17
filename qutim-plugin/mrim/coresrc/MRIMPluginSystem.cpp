/*****************************************************************************
    MRIMPluginSystem

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

#include "MRIMPluginSystem.h"
#include "MRIMUtils.h"
#include "ui_settingswidget.h"
#include "../uisrc/loginform.h"
#include "mrimeventhandler.h"
#include <qutim/iconmanagerinterface.h>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QList>

PluginSystemInterface* MRIMPluginSystem::m_static_plugin_system = 0;
MRIMPluginSystem* MRIMPluginSystem::m_selfPointer = 0;

MRIMPluginSystem::MRIMPluginSystem()
{
    qsrand(QDateTime::currentDateTime ().toTime_t());
    m_loginWidget = NULL;
    m_settingsWidget = NULL;
    m_generalSettingsWidget = NULL;
    m_generalSettItem = NULL;
    m_connectionSettItem = NULL;
    m_selfPointer = this;
    m_protoIcon = NULL;
    m_event_handler = NULL;
}

MRIMPluginSystem::~MRIMPluginSystem()
{
    delete m_event_handler;
}

QWidget* MRIMPluginSystem::loginWidget()
{
    if (!m_loginWidget)
    {
        m_loginWidget = new LoginForm(m_profileName);
    }
    return m_loginWidget;
}

bool MRIMPluginSystem::init(PluginSystemInterface *plugin_system)
{
    ProtocolInterface::init(plugin_system);
    m_static_plugin_system = plugin_system;
    m_event_handler = new MRIMEventHandlerClass(); //init event handler
    return true;
}

void MRIMPluginSystem::saveLoginDataFromLoginWidget()
{
    m_loginWidget->SaveSettings();
    addAccount(m_loginWidget->GetEmail());
}

QList<SettingsStructure> MRIMPluginSystem::getSettingsList()
{
    if (m_generalSettItem == NULL)
    {
        m_generalSettItem = new QTreeWidgetItem();
        m_generalSettItem->setIcon(0,*m_protoIcon);
        m_generalSettItem->setText(0,tr("General settings"));
    }

    if (m_connectionSettItem == NULL)
    {
        m_connectionSettItem = new QTreeWidgetItem();
        m_connectionSettItem->setIcon(0,*m_protoIcon);
        m_connectionSettItem->setText(0,tr("Connection settings"));
    }

    if (m_settingsWidget == NULL)
    {
        m_settingsWidget = new SettingsWidget(m_profileName);
    }

    if (m_generalSettingsWidget == NULL)
    {
        m_generalSettingsWidget = new GeneralSettings(m_profileName);
    }


    QList<SettingsStructure> settings;

    SettingsStructure generalSettingsStruct;
    generalSettingsStruct.settings_item = m_generalSettItem;
    generalSettingsStruct.settings_widget = m_generalSettingsWidget;

    SettingsStructure connectionSettingsStruct;
    connectionSettingsStruct.settings_item = m_connectionSettItem;
    connectionSettingsStruct.settings_widget = m_settingsWidget;

    settings.append(generalSettingsStruct);
    settings.append(connectionSettingsStruct);
    return settings;
}

void MRIMPluginSystem::removeProtocolSettings()
{
    if (m_generalSettItem)
    {
        delete m_generalSettItem;
        m_generalSettItem = NULL;
    }

    if (m_connectionSettItem)
    {
        delete m_connectionSettItem;
        m_connectionSettItem = NULL;
    }

    if (m_settingsWidget)
    {
        delete m_settingsWidget;
        m_settingsWidget = NULL;
    }

    if (m_generalSettingsWidget)
    {
        delete m_generalSettingsWidget;
        m_generalSettingsWidget = NULL;
    }

}

void MRIMPluginSystem::removeLoginWidget()
{
    if (m_loginWidget)
        delete m_loginWidget;
    m_loginWidget = NULL;
}

void MRIMPluginSystem::addAccountButtonsToLayout(QHBoxLayout *aLayout)
{
    m_protoButtonLayout = aLayout;
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName, "mrimsettings");
    QStringList accountList = settings.value("accounts/list").toStringList();

    foreach(QString account_from_list, accountList)
    {
        addAccount(account_from_list);
    }
}

void MRIMPluginSystem::removeAccount(const QString &account_name)
{
#ifdef DEBUG_LEVEL_DEV
    qDebug()<<"MRIM: removeAccount - profile "<<m_profileName<<"removing "<<account_name;
#endif
    QSettings profile_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrimsettings");
    QStringList accountList = profile_settings.value("accounts/list").toStringList();
    accountList.removeOne(account_name);
    profile_settings.setValue("accounts/list",accountList);

    QSettings dir_settings_path(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrim."+account_name, "accountsettings");
    QDir account_dir(dir_settings_path.fileName());
    account_dir.cdUp();
    //delete profile directory
    if( account_dir.exists() )
        removeProfileDir(account_dir.path());
    MRIMClient* client = FindClientInstance(account_name);
    if (client)
    {
        disconnect(this,SIGNAL(UpdateClientsSettings()),client,SLOT(UpdateSettings()));
        client->ClearCL();
        client->RemoveAccountButton();
        m_clients.remove(account_name);
        delete client;
    }
}

void MRIMPluginSystem::removeProfileDir(const QString &path)
{
    //recursively delete all files in directory
#ifdef DEBUG_LEVEL_DEV
    qDebug()<<"MRIM: removeProfileDir - will do cleanup at path: "<<path;
#endif
    QFileInfo fileInfo(path);
    if( fileInfo.isDir() )
    {
        QDir dir( path );
        QFileInfoList fileList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        for (int i = 0; i < fileList.count(); i++)
            removeProfileDir(fileList.at(i).absoluteFilePath());
        dir.rmdir(path);
    } else {
        QFile::remove(path);
    }
}

void MRIMPluginSystem::applySettingsPressed()
{//settings updated
    QSettings profile_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName+"/mrimsettings");

    bool phoneCnts = m_generalSettingsWidget->GetShowPhoneCnts();
    bool restoreStatus = m_generalSettingsWidget->GetRestoreStatus();
    profile_settings.setValue("main/phoneCnts", phoneCnts);
    profile_settings.setValue("main/restoreStatus", restoreStatus);
    profile_settings.setValue("roster/statustext", m_generalSettingsWidget->GetStatustextStatus());
    emit UpdateClientsSettings();
}

QList<QMenu*> MRIMPluginSystem::getAccountStatusMenu()
{
    QList<QMenu*> menuList;
    foreach (MRIMClient* client, m_clients)
    {
        menuList.append(client->AccountMenu());
    }
    return menuList;
}

QList<AccountStructure> MRIMPluginSystem::getAccountList()
{
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName, "mrimsettings");
    QStringList accountList = settings.value("accounts/list").toStringList();
    QList<AccountStructure> accounts;
    foreach(QString account_from_list, accountList)
    {
        AccountStructure account;
        account.account_name = account_from_list;
        account.protocol_icon = *m_protoIcon;
        account.protocol_name = "MRIM";
        accounts.append(account);
    }
    return accounts;
}

QList<AccountStructure> MRIMPluginSystem::getAccountStatuses()
{
    QList<AccountStructure> statuses;
    AccountStructure acc;
    foreach (MRIMClient* client, m_clients)
    {
        acc = client->GetAccountInfo();
        statuses.append(acc);
    }
    return statuses;
}

void MRIMPluginSystem::setAutoAway()
{
    foreach (MRIMClient* client, m_clients)
    {
        client->SetAutoAway();
    }
}

void MRIMPluginSystem::setStatusAfterAutoAway()
{
    foreach (MRIMClient* client, m_clients)
    {
        client->RestoreFromAutoAway();
    }
}

void MRIMPluginSystem::itemActivated(const QString &account_name, const QString &contact_name)
{
    //not yet needed
}

void MRIMPluginSystem::itemContextMenu(const QList<QAction*> &action_list,
                                       const QString &account_name, const QString &contact_name, int item_type, const QPoint &menu_point)
{
    if (item_type == EContact)
    {
        TreeModelItem itemData;
        itemData.m_account_name = account_name;
        itemData.m_item_type = item_type;
        itemData.m_item_name = contact_name;
        MRIMClient* client = FindClientInstance(account_name);
        if (client)
        {
            client->ShowCntContextPopup(action_list,itemData,menu_point);
        }
    }
}

void MRIMPluginSystem::sendMessageTo(const QString &account_name, const QString &contact_name, int item_type, const QString& message, int message_icon_position)
{
    MRIMClient* client = FindClientInstance(account_name);
    if (client && item_type == EContact)
    {
        client->SendMessageToContact(contact_name,message,message_icon_position);
    }
}

MRIMClient* MRIMPluginSystem::FindClientInstance(QString aAccName) const
{
    MRIMClient* client = NULL;
    QHash<QString, MRIMClient*>::const_iterator iter = m_clients.find(aAccName);
    if (iter != m_clients.end())
    {
        client = iter.value();
    }
    return client;
}

QStringList MRIMPluginSystem::getAdditionalInfoAboutContact(const QString &account_name, const QString &item_name, int item_type ) const
{
    MRIMClient* client = FindClientInstance(account_name);
    QStringList additionalInfo;

    if (client)
    {
        if (account_name == item_name)
        {
            MRIMUserInfo accInfo = client->GetUserInfo();

            if (accInfo.userNickname != "")
            {
                additionalInfo.append(accInfo.userNickname);
            }
            else
            {
                additionalInfo.append(item_name);
            }
//            if (accInfo.AvatarPath != "")
//            {
//                additionalInfo.append(accInfo.AvatarPath);
//            }
        }
        else
        {
            ContactAdditionalInfo info = client->GetContactAdditionalInfo(item_name);

            if (info.Nick != "")
            {
                additionalInfo.append(info.Nick);
            }
            else
            {
                additionalInfo.append(item_name);
            }

            if (info.AvatarPath != "")
            {
                additionalInfo.append(info.AvatarPath);
            }

            if (info.ClientName != "")
            {
                additionalInfo.append(info.ClientName);
            }

            if (info.OtherInfo != "")
            {
                additionalInfo.append(info.OtherInfo);
            }
        }
    }
    return additionalInfo;
}

void MRIMPluginSystem::showContactInformation(const QString &account_name, const QString &item_name, int item_type )
{
    MRIMClient* client = FindClientInstance(account_name);
    if (client && item_type == EContact)
    {
        client->ShowContactDetails(item_name);
    }
}

void MRIMPluginSystem::sendImageTo(const QString &account_name, const QString &item_name, int item_type, const QByteArray &image_raw )
{
    //TODO
}

void MRIMPluginSystem::sendFileTo(const QString &account_name, const QString &item_name, int item_type, const QStringList &file_names)
{
    if (item_type != EContact) return;
    FindClientInstance(account_name)->SendFileTo(item_name,file_names);
}

void MRIMPluginSystem::sendTypingNotification(const QString &account_name, const QString &item_name, int item_type, int notification_type)
{
    MRIMClient* client = FindClientInstance(account_name);
    if (client && client->Protocol() && item_type == EContact && notification_type != 0)
    {
        client->Protocol()->SendTypingToContact(item_name);
    }
}

void MRIMPluginSystem::moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item)
{
    MRIMClient* client = FindClientInstance(old_item.m_account_name);
    if (client && old_item.m_item_type == EContact)
    {
        client->MoveContact(old_item.m_item_name,new_item.m_parent_name);
    }

}

QString MRIMPluginSystem::getItemToolTip(const QString &account_name, const QString &contact_name)
{
    QString toolTip;
    MRIMClient* client = FindClientInstance(account_name);
    if (client)
    {
        toolTip = client->GetItemToolTip(contact_name);
    }
    return toolTip;
}

void MRIMPluginSystem::deleteItemSignalFromCL(const QString &account_name, const QString &item_name, int type)
{
    MRIMClient* client = FindClientInstance(account_name);
    if (client && type == EContact)
    {
        client->RemoveContactFromCL(item_name);
    }
}

void MRIMPluginSystem::chatWindowOpened(const QString &account_name, const QString &item_name)
{
    //not needed yet
}

void MRIMPluginSystem::sendMessageToConference(const QString &conference_name, const QString &account_name,
                                               const QString &message)
{//conference is not yet implemented
}

void MRIMPluginSystem::leaveConference(const QString &conference_name, const QString &account_name)
{//conference is not yet implemented
}

void MRIMPluginSystem::release()
{

}

QString MRIMPluginSystem::name()
{
    QString name = "MRIM";
    return name;
}

QString MRIMPluginSystem::description()
{
    QString desc = "MRIM qutIM protocol plugin";
    return desc;
}

QIcon *MRIMPluginSystem::icon()
{
    return m_protoIcon;
}

void MRIMPluginSystem::setProfileName(const QString &profile_name)
{
    m_profileName = profile_name;
    m_protoIcon = new QIcon(Icon("mrim",IconInfo::Protocol));
    delete m_settingsWidget;
}

void MRIMPluginSystem::addAccount(QString accountName)
{
    MRIMClient* client = new MRIMClient(accountName,m_profileName,m_plugin_system,m_protoButtonLayout);
    client->CreateAccountButton();
    connect(this,SIGNAL(UpdateClientsSettings()),client,SLOT(UpdateSettings()));
    m_clients.insert(accountName,client);
}

void MRIMPluginSystem::conferenceItemActivated(const QString &conference_name, const QString &account_name, 
                                               const QString &nickname)
{//conference is not yet implemented
}

void MRIMPluginSystem::conferenceItemContextMenu(const QList<QAction*> &action_list, const QString &conference_name,
                                                 const QString &account_name, const QString &nickname, const QPoint &menu_point)
{//conference is not yet implemented
}

QString MRIMPluginSystem::getConferenceItemToolTip(const QString &conference_name, const QString &account_name, const QString &nickname)
{//conference is not yet implemented
    return QString();
}

void MRIMPluginSystem::showConferenceContactInformation(const QString &conference_name, const QString &account_name, const QString &nickname)
{ //conference is not yet implemented
}

void MRIMPluginSystem::showConferenceTopicConfig(const QString &conference_name, const QString &account_name)
{

}

void MRIMPluginSystem::showConferenceMenu(const QString &conference_name, const QString &account_name, 
                                          const QPoint &menu_point)
{

}

void MRIMPluginSystem::getMessageFromPlugins(const QList<void *> &event)
{//not implemented yet

}

void MRIMPluginSystem::editAccount(const QString &account_name)
{
    FindClientInstance(account_name)->ShowEditAccountWindow();
}

void MRIMPluginSystem::chatWindowAboutToBeOpened(const QString &account_name, const QString &item_name)
{//not implemented yet


}

void MRIMPluginSystem::chatWindowClosed(const QString &account_name, const QString &item_name)
{//not implemented yet


}

Q_EXPORT_PLUGIN2(mrimrotoimpl, MRIMPluginSystem);
