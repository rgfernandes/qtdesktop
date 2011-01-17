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

#ifndef MRIMCLIENT_H_
#define MRIMCLIENT_H_

#include <QtCore>
#include <QtGui>
#include <QMap>
#include "MRIMProto.h"
#include <qutim/protocolinterface.h>
#include <qsettings.h>
#include <QDateTime>
#include <QActionGroup>
#include <QNetworkProxy>
#include "mrimdefs.h"

using namespace qutim_sdk_0_2;

class authwidget;
class MRIMSearchWidget;
class SearchResultsWidget;
class ContactDetails;
class MoveToGroupWidget;
class SMSWidget;
class AddNumberWidget;

class MRIMClient: public QObject
{
    Q_OBJECT

    enum MenuActions
    {
        AddContactAction
    };
public:        
    MRIMClient(QString accountName, QString profileName, PluginSystemInterface *pluginSystem, QHBoxLayout *protoButtonLayout);
    const TreeModelItem AccountItem();
    void CreateAccountButton();
    void RemoveAccountButton();
    void ClearCL(CLItemType aDepth = EAccount, bool aDeleteFromSettings = false);
    void SendMessageToContact(QString aContactName, QString aMessage, int aMessageIconPosition);
    virtual ~MRIMClient();
    void ShowCntContextPopup(const QList<QAction*> &aDefActList,TreeModelItem aItemData, const QPoint &aMenuPoint);
    ContactAdditionalInfo GetContactAdditionalInfo(QString aEmail);
    MRIMUserInfo GetUserInfo();
    void ShowContactDetails(QString aEmail);
    inline QMenu* AccountMenu() { return m_accountButtonMenu; }
    AccountStructure GetAccountInfo();
    void SetAutoAway();
    void RestoreFromAutoAway();
    void RenameContact(QString aEmail, QString aNewName);
    QString GetItemToolTip(const QString &aCntId);
    void RemoveContactFromCL(QString aCntId);
    inline MRIMProto* Protocol() { return m_protoInstance; }
    inline const QString& ProfileName() const { return m_profileName; }
    inline const QString& AccountName() const { return m_accountName; }
    QString FilterMessage(const QString& aMessage);
    void SendFileTo(QString aTo, QStringList aFiles);        
    void ShowEditAccountWindow();
    void ChangeStatus(const Status &aNewStatus);

public slots:
    void MoveContact(QString aCntId, QString aNewGroup);
    //from search res widget
    void HandleAddContact(QString aEmail = QString(),QString aNick = QString());
    //from protoimpl
    void UpdateSettings();
private slots:
    void SelectXStatusesClicked();
    void DisconnectMenuItemClicked();
    void ChangeStatusClicked(QAction* action);
    void AccountMenuItemClicked(QAction* action);

    void CntContextMenuClicked(QAction* aAction);
    //from proto
    void HandleProtoStatusChanged(StatusData aNewStatusData);

    void UpdateStatusIcon( const Status &newStatus );
    void UpdateStatusIcon( const QIcon &newIcon );
    void HandleItemAdditionToUI(CLItemType aType, QString aParentId, QString aID, QString aName, StatusData aStatusData, bool aAuthed, bool aIsNew);
    void HandleAccountInfoRecieved(MRIMUserInfo aInfo);
    void HandleMessageRecieved(QString aContactEmail, QString aGroupId, QString aMessage, QDateTime aDate, bool aIsRtf, bool aIsAuth);
    void HandleContactTyping(QString aContactEmail, QString aGroupId);
    void HandleContactTypingStopped(QString aContactEmail, QString aGroupId);
    void HandleMessageDelivered(QString aContactEmail, QString aGroupId, quint32 aKernelMessageId);
    void HandleAuthorizeResponseReceived(QString aContactEmail, QString aGroupId);
    void HandleMailboxStatusChanged(quint32 aUnreadMessages);
    void HandleLogoutReceived(LogoutReason aReason);
    void HandleMPOPKeyReceived(QString aMPOPKey);
    void HandleCLOperationFailed(CLOperationError aClErrCode);
    void HandleSearchFinished(QList<MRIMSearchParams*> aFoundList);
    void HandleRemoveItemFromUI(CLItemType aType,QString aParentID,QString aId);
    void HandleNewCLReceived();
    void HandleFileTransferRequest(FileTransferRequest aReq);
    //from proto
    void HandleNotifyUI(QString aMessage);

private:
    void ChangeStatus(qint32 aNewStatus, const QString &aCustomID = QString());
    void LoadSettings();
    void LoadAccountSettings();
    void ConnectAllProtoEvents();
    void DisconnectAllProtoEvents();
    void SaveCLItem(CLItemType aItemType, TreeModelItem aItem, QString aName, bool aisAuthed,bool aIsAuthedMe, QString aPhone);
    void LoadCL();
    void DeleteFromLocalSettings(CLItemType aType, QString aId);
    void FillActionLists();
private:	
    QToolButton* m_accountButton;
    QSettings *m_settings;
    QString m_accountName;
    QString m_profileName;
    MRIMProto* m_protoInstance;
    QHBoxLayout *m_protoButtonLayout;
    PluginSystemInterface *m_pluginSystem;
    QMenu* m_contextCntMenu;
    QMenu *m_accountButtonMenu; //account menu
    QActionGroup *m_statusGroup; //statuses group

    //account menu actions
    QAction* m_actUnreadEmails;
    QAction* m_actExtendedStatus;
    QAction* m_actAddContact;
    QAction* m_actOpenMbox;
    QAction* m_actSearchCnts;
    //item context menu actions
    QWidgetAction* m_menuTitle;
    QLabel* m_menuLabel;
    QAction* m_actRemoveCnt;
    QAction* m_actAuthorizeCnt;
    QAction* m_actRequestAuthFromCnt;
    QAction* m_actRenameCnt;
    QAction* m_actMoveToGroup;
    QAction* m_actAddToList;
    QAction* m_actSendSms;
    QAction* m_actAddNumber;
    QAction* m_separator;

    QList<QAction*> m_contactActionsList;
    QList<QAction*> m_statusActionsList;
    QList<QAction*> m_additionalActionsList;
    QList<QAction*> m_otherActionsList;

    //widgets
    MRIMSearchWidget* m_searchCntsWidget;
    SearchResultsWidget* m_searchResWidget;
    ContactDetails* m_cntDetailsWidget;
    MoveToGroupWidget* m_moveToGroupWidget;
    SMSWidget* m_smsWidget;
    AddNumberWidget* m_addNumberWidget;
    //other members
    MRIMUserInfo m_userInfo;

    QString m_login;
    QString m_pass;
    QString m_host;
    quint32 m_port;
    QNetworkProxy m_proxy;

    bool m_inAutoAway;
    bool m_phoneGroupAdded;
    bool m_isAccountItemAdded;

    bool m_isAddingContact;
    bool m_updateAccSettingsOnNextLogon;

    bool m_showPhoneCnts;
    quint32 m_phoneCntCounter;

    QString m_mpopKey;
};

#endif /*MRIMCLIENT_H_*/
