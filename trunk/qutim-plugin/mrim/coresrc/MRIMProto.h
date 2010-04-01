/*****************************************************************************
    MRIMProto

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

#ifndef MRIMPROTO_H
#define MRIMPROTO_H

#include <QTcpSocket>
#include <QtGui/QWidget>
#include <QString>
#include <QBuffer>
#include <QQueue>
#include <QMap>
#include "proto.h"
#include <QTimer>
#include "mrimdefs.h"
#include <QDateTime>
#include "MRIMContactList.h"
#include <QNetworkProxy>
#include "Status.h"
#include "useragent.h"

class MRIMPacket;

struct TypingStruct
{
    MRIMContact* typingContact;
    int secsLeft;
};

struct MsgIdsLink
{
    quint32 qutimKernelNum;
    quint32 mrimMsgSequence;
    QString contactEmail;
    quint32 groupId;
};


class MRIMProto : public QObject
{
    Q_OBJECT

public:
    struct MRIMOfflineMessage
    {
        QString From;
        QDateTime DateTime;
        QString Subject; //for future use
        quint32 Flags;
        QString Message;
    };

    enum TModifyFlags
    {
        ENoFlags = 0,
        EKeepOldValues
    };

    enum CurrentCLOperation
    {
        ENoOperation,
        EDelete,
        EModify,
        EAdd
    };

    MRIMProto(QString aProfileName, QNetworkProxy aProxy);
    ~MRIMProto();
    void Connect(QString aLogin, QString aPass, QString aHost, quint32 aPort,  const Status &aStatus = Status(STATUS_ONLINE));
    void SendTypingToContact(QString aEmail);
    void SendMessageToContact(QString aEmail, QString aMessage, quint32 aKernelMsgId, bool aIsAuth = false, bool aIsTyping = false);
    void SendAuthorizationTo(QString aEmail);
    void AddContact(QString aEmail, QString aName, quint32 aGroupId, bool aIsAuthed = false, bool aIsAuthedMe = false);
    void AddGroup(QString aName, quint32 aId = -1);
    QList<MRIMGroup*> GetAllGroups();
    bool IsInList(QString aEmail);
    bool IsContactAuthedByMe(QString aEmail);
    bool IsContactAuthedMe(QString aEmail);
    bool ChangeStatus(const Status& aNewStatus);
    void SetProxy(QNetworkProxy aProxy);
    QList<MRIMCLItem*>* GetAllCL();
    void DisconnectFromIM();
    MRIMContact* GetContactByEmail(QString aEmail);
    void RemoveUserFromCL(QString aEmail);
    void StartSearch(MRIMSearchParams aParams);
    void RequestCntInfo(QString aEmail);
    MRIMContact* GetCnt(QString aEmail);
    void SendModifyContact(QString aEmail, QString aNewName, quint32 aNewGroupId, quint32 aFlags = 0, TModifyFlags aSpecialFlags = ENoFlags);
    void SendSMS(QString aPhoneNumber, QString aText);
    void DeclineFileTransfer(quint32 aUniqueId);
    void FileTransferCompleted(quint32 aUniqueId);
    void SendFileTransferRequest(const FileTransferRequest& aReq);
    bool IsOnline();
    static bool IsOnline(const Status& aStatus);
    inline Status& CurrentStatus() { return m_currentStatus; }
    inline Status& PreviousStatus() { return m_prevStatus; }
    inline MRIMContactList *GetContactList() { return m_clParser; }
signals:
    void NotifyUI(QString aMessage);
    void ProtoStatusChanged(StatusData aStatusData);
    void AddItemToUI(CLItemType aType, QString aParentId, QString aID, QString aName, StatusData aData, bool aAuthed = true, bool isNew = true);
    void RemoveItemFromUI(CLItemType aType, QString aParentId, QString aID);
    void AccountInfoRecieved(MRIMUserInfo aInfo);
    void ContactTyping(QString aContactEmail, QString aGroupId);
    void ContactTypingStopped(QString aContactEmail, QString aGroupId);
    void MessageRecieved(QString aContactEmail, QString aGroupId, QString aMessage, QDateTime aDate, bool aIsRtf, bool aIsAuth);
    void MessageDelivered(QString aContactEmail, QString aGroupId, quint32 aKernelMessageId);
    void AuthorizeResponseReceived(QString aContactEmail, QString aGroupId);
    void LogoutReceived(LogoutReason aReason);
    void MailboxStatusChanged(quint32 aUnreadMessages);
    void MPOPKeyReceived(QString aMPOPKey);
    void CLOperationFailed(CLOperationError aError);
    void SearchFinished(QList<MRIMSearchParams*> aFoundList);
    void ContactFound(MRIMSearchParams aFoundContact);
    void ContactInfoRecieved(MRIMSearchParams aInfo);
    void NewCLReceived();
    void FileTransferRequested(FileTransferRequest req);
private slots:
    void receiveGoodServer();
    void connectedToSrvRequestServer();
    void connectedToIMServer();
    void disconnectedFromIMServer();
    void disconnectedFromSrvRequestServer();
    void readDataFromSocket();
    void SendPINGPacket();
    void SendLOGINPacket();
    bool HandleMRIMPacket(MRIMPacket* aPacket);
    void StartPing();
    void StopPing();
    void TypingTimerStep();
    void RequestMPOPKey();
    void HandleChangeOfStatus();
private:
    //handlers
    void HandleContactList(MRIMPacket* aPacket);
    void HandleUserStatusChanged(MRIMPacket* aPacket);
    void HandleUserInfo(MRIMPacket* aPacket);
    void HandleMessageAck(MRIMPacket* aPacket);
    void HandleAddContactAck(MRIMPacket* aPacket);
    void HandleMessageStatusPacket(MRIMPacket* aPacket);
    void HandleAuthorizeAckPacket(MRIMPacket* aPacket);
    void HandleMPOPSessionAck(MRIMPacket* aPacket);
    void HandleModifyContactAck(MRIMPacket* aPacket);
    void HandleAnketaInfo(MRIMPacket* aPacket);
    void HandleFileTransferRequest(MRIMPacket* aPacket);
    void HandleOfflineMessageAck(MRIMPacket* aPacket);
    void HandleNewMail(MRIMPacket* aPacket);
    //senders
    void SendStatusChangePacket(const Status& aNewStatus);
    void SendDeliveryReport(QString aFrom, quint32 aMsgId);
    void SetAllContactsOffline();
    void SendFileTransferAck(quint32 aUniqueId, quint32 aErrorCode, QString aIPsList = QString() );
    bool IsUnicodeAnketaField(const QString& aFieldName);

    //utils
    CLOperationError ConvertCLErrorFromNative(quint32 aNativeErrorCode);
    quint32 ConvertCLErrorToNative(CLOperationError aLocalErrorCode);
    MRIMSearchParams* ParseForm(QHash<QString,QString>& aUnparsedForm);
    bool ParseOfflineMessage(QString aRawMsg, MRIMOfflineMessage& aMsg);
    inline quint32 ProtoFeatures() { return MY_MRIM_PROTO_FEATURES; }

    //members
    MRIMContactList* m_clParser;
    QBuffer* m_RecvBuffer;
    Status m_firstStatus;
    Status m_currentStatus;
    Status m_prevStatus;
    QTimer* m_PingTimer;
    quint32 m_PingPeriod;

    MRIMContact* m_addingContact;
    MRIMGroup* m_addingGroup;
    MRIMContact* m_modifingContact;
    CurrentCLOperation m_currentClOp;

    QNetworkProxy m_proxy;
    QString m_profileName;

    QTcpSocket* m_IMSocket;
    QTcpSocket* m_SrvRequestSocket;

    MRIMUserInfo m_UserInfo;
    bool m_SaveBufPos;
    QString* m_IMServerHost;
    ulong m_IMServerPort;
    QString m_IMServerSelectorHost;
    ulong m_IMServerSelectorPort;
    QString m_login;
    QString m_pass;

    QTimer* m_typingTimer;	
    QTimer* m_mpopTimer;
    QList<TypingStruct>* m_typersList;


    quint32 m_msgSequenceNum;

    QQueue<MsgIdsLink> m_msgIdsLinks;
    QHash<quint32,FileTransferRequest*> m_fileReqsHash;

    bool m_doNotReconnect;
    bool m_cntInfoRequested;
    quint32 m_unreadMsgs;
    quint32 m_protoFeatures;
    UserAgent m_userAgent;
};

#endif // MRIMPROTO_H
