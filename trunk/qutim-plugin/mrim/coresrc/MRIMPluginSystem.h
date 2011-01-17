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

#ifndef MRIMPROTOIMPL_H_
#define MRIMPROTOIMPL_H_

#include "../uisrc/mrimloginwidget.h"
#include "../uisrc/settingswidget.h"
#include "../uisrc/generalsettings.h"
#include "MRIMClient.h"
#include <qutim/protocolinterface.h>
#include <QtGui>
#include <QtCore>

using namespace qutim_sdk_0_2;
class LoginForm;
class MRIMEventHandlerClass;

class MRIMPluginSystem: public QObject, public ProtocolInterface
{
    Q_INTERFACES(qutim_sdk_0_2::PluginInterface)
        Q_OBJECT
public:
    MRIMPluginSystem();
    virtual ~MRIMPluginSystem();	
    virtual bool init(PluginSystemInterface *plugin_system);
    inline QString Profile() { return m_profileName; }
    virtual void removeAccount(const QString &account_name);
    virtual QWidget *loginWidget();
    virtual void removeLoginWidget();
    virtual void applySettingsPressed();
    virtual QList<QMenu*> getAccountStatusMenu();
    virtual void addAccountButtonsToLayout(QHBoxLayout *);
    virtual void saveLoginDataFromLoginWidget();
    virtual QList<SettingsStructure> getSettingsList();
    virtual void removeProtocolSettings();
    virtual QList<AccountStructure> getAccountList();	
    virtual QList<AccountStructure> getAccountStatuses();
    virtual void setAutoAway();
    virtual void setStatusAfterAutoAway();	
    virtual void itemActivated(const QString &account_name, const QString &contact_name);
    virtual void itemContextMenu(const QList<QAction*> &action_list,
        const QString &account_name, const QString &contact_name, int item_type, const QPoint &menu_point);
    virtual void sendMessageTo(const QString &account_name, const QString &contact_name, int item_type, const QString& message, int message_icon_position);
    virtual QStringList getAdditionalInfoAboutContact(const QString &account_name, const QString &item_name, int item_type ) const;
    virtual void showContactInformation(const QString &account_name, const QString &item_name, int item_type );
    virtual void sendImageTo(const QString &account_name, const QString &item_name, int item_type, const QByteArray &image_raw );
    virtual void sendFileTo(const QString &account_name, const QString &item_name, int item_type, const QStringList &file_names);
    virtual void sendTypingNotification(const QString &account_name, const QString &item_name, int item_type, int notification_type);
    virtual void moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item);
    virtual QString getItemToolTip(const QString &account_name, const QString &contact_name);
    virtual void deleteItemSignalFromCL(const QString &account_name, const QString &item_name, int type);
    virtual void chatWindowOpened(const QString &account_name, const QString &item_name);
    virtual void sendMessageToConference(const QString &conference_name, const QString &account_name,
        const QString &message);
    virtual void leaveConference(const QString &conference_name, const QString &account_name);
    virtual void release();
    virtual QString name();
    virtual QString description();
    virtual QIcon *icon();
    virtual void setProfileName(const QString &profile_name);
    virtual void conferenceItemActivated(const QString &conference_name, const QString &account_name, 
        const QString &nickname);
    virtual void conferenceItemContextMenu(const QList<QAction*> &action_list, const QString &conference_name,
        const QString &account_name, const QString &nickname, const QPoint &menu_point);
    virtual QString getConferenceItemToolTip(const QString &conference_name, const QString &account_name, const QString &nickname);
    virtual void showConferenceContactInformation(const QString &conference_name, const QString &account_name, const QString &nickname);
    virtual void showConferenceTopicConfig(const QString &conference_name, const QString &account_name);
    virtual void showConferenceMenu(const QString &conference_name, const QString &account_name, 
        const QPoint &menu_point);
    virtual void getMessageFromPlugins(const QList<void *> &event);
    virtual void editAccount(const QString &account_name);
    virtual void chatWindowAboutToBeOpened(const QString &account_name, const QString &item_name);
    virtual void chatWindowClosed(const QString &account_name, const QString &item_name);
    //my funcs
    void addAccount(QString accountName);
    void removeProfileDir(const QString &path);
    MRIMClient* FindClientInstance(QString aAccName) const;
    inline MRIMEventHandlerClass* GetEventHandler() const { return m_event_handler; }
    inline static PluginSystemInterface* PluginSystem() { return m_static_plugin_system; }
    inline static MRIMPluginSystem* ImplPointer() { return m_selfPointer; }
signals:
    void UpdateClientsSettings();
private:
    static PluginSystemInterface* m_static_plugin_system;
    static MRIMPluginSystem* m_selfPointer;

    QHash<QString,MRIMClient*> m_clients;
    LoginForm* m_loginWidget;
    SettingsWidget* m_settingsWidget;
    GeneralSettings* m_generalSettingsWidget;

    QTreeWidgetItem* m_generalSettItem;
    QTreeWidgetItem* m_connectionSettItem;

    QHBoxLayout *m_protoButtonLayout;

    QString m_profileName;
    QString m_host;
    quint32 m_port;
    bool m_useProxy;
    QString m_proxyHost;
    quint32 m_proxyPort;
    QNetworkProxy::ProxyType m_proxyType;
    QString m_proxyUserName;
    QString m_proxyPass;
    QNetworkProxy m_sharedProxy;
    QIcon *m_protoIcon;
    MRIMEventHandlerClass* m_event_handler;
};

inline MRIMPluginSystem* PluginSystemImpl()
{
    return MRIMPluginSystem::ImplPointer();
}

#endif /*MRIMPROTOIMPL_H_*/
