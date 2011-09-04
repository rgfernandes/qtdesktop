/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Messaging Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PopClient_H
#define PopClient_H

#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qlist.h>
#include <qtimer.h>
#include <qmailaccountconfiguration.h>
#include <qmailmessage.h>
#include <qmailmessageclassifier.h>
#include <qmailmessageserver.h>
#include <qmailtransport.h>

class LongStream;
class QMailTransport;
class QMailAccount;

typedef QMap<QString, QMailMessageId> SelectionMap;

class PopClient : public QObject
{
    Q_OBJECT

public:
    PopClient(QObject* parent);
    ~PopClient();

    QMailMessage::MessageType messageType() const;

    void setAccount(const QMailAccountId &accountId);
    QMailAccountId account() const;

    void newConnection();
    void closeConnection();
    void setOperation(QMailRetrievalAction::RetrievalSpecification spec);
    void setDeleteOperation();
    void setSelectedMails(const SelectionMap& data);
    void checkForNewMessages();
    void cancelTransfer();

signals:
    void errorOccurred(int, const QString &);
    void errorOccurred(QMailServiceAction::Status::ErrorCode, const QString &);
    void updateStatus(const QString &);

    void messageActionCompleted(const QString &uid);

    void progressChanged(uint, uint);
    void retrievalCompleted();

    void allMessagesReceived();

protected slots:
    void connected(QMailTransport::EncryptType encryptType);
    void transportError(int, QString msg);

    void connectionInactive();
    void incomingData();

private:
    void deactivateConnection();
    int nextMsgServerPos();
    int msgPosFromUidl(QString uidl);
    uint getSize(int pos);
    void uidlIntegrityCheck();
    void createMail();
    void sendCommand(const char *data, int len = -1);
    void sendCommand(const QString& cmd);
    void sendCommand(const QByteArray& cmd);
    QString readResponse();
    void processResponse(const QString &response);
    void nextAction();
    void retrieveOperationCompleted();
    void messageProcessed(const QString &uid);

    void operationFailed(int code, const QString &text);
    void operationFailed(QMailServiceAction::Status::ErrorCode code, const QString &text);

private:
    enum TransferStatus
    {
        Init, CapabilityTest, Capabilities, 
        StartTLS, TLS, Connected, Auth, 
        RequestUids, Uidl, UidList,
        RequestSizes, List, SizeList,
        RequestMessage, Retr, MessageData,
        DeleteMessage, Dele,
        Done, Quit, Exit
    };

    QMailAccountConfiguration config;
    QTimer inactiveTimer;
    TransferStatus status;
    int messageCount;
    bool selected;
    bool deleting;
    QString message;
    SelectionMap selectionMap;
    SelectionMap::ConstIterator selectionItr;
    uint mailSize;
    uint headerLimit;

    QMap<QString, int> serverUidNumber;
    QMap<int, QString> serverUid;
    QMap<int, uint> serverSize;

    QString messageUid;
    QStringList newUids;
    QStringList obsoleteUids;
    LongStream *dataStream;

    QMailTransport *transport;

    QString retrieveUid;

    SelectionMap completionList;

    uint progress;
    uint total;

    // RetrievalMap maps uid -> ((units, bytes) to be retrieved, percentage retrieved)
    typedef QMap<QString, QPair< QPair<uint, uint>, uint> > RetrievalMap;
    RetrievalMap retrievalSize;
    uint progressRetrievalSize;
    uint totalRetrievalSize;

    QMailMessageClassifier classifier;

    QStringList capabilities;
    QList<QByteArray> authCommands;
};

#endif
