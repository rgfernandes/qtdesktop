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

#include "MRIMProto.h"
#include <qendian.h>
#include "MRIMUtils.h"
#include <QBuffer>
#include <QTimer>
#include <QDateTime>
#include <QByteArray>
#include <QTextCodec>
#include "mrimeventhandler.h"
#include "MRIMContactList.h"
#include "MRIMPacket.h"
#include <QSettings>
#include <QMessageBox>
#include <QLocale>

const qint32 KMpopRequestPeriod = 1000*60*30; //30 mins

MRIMProto::MRIMProto(QString aProfileName, QNetworkProxy aProxy)
: m_clParser(NULL), m_RecvBuffer(NULL), m_PingTimer(NULL), m_addingContact(NULL), m_addingGroup(NULL), m_modifingContact(NULL), m_currentClOp(ENoOperation), m_proxy(aProxy),m_profileName(aProfileName), m_IMSocket(NULL), m_SrvRequestSocket(NULL), m_protoFeatures(0), m_currentStatus(STATUS_OFFLINE)
{
    m_typersList = new QList<TypingStruct>;
    m_typingTimer = new QTimer;
    m_mpopTimer = new QTimer;
    connect(m_typingTimer,SIGNAL(timeout()),this,SLOT(TypingTimerStep()));
    m_doNotReconnect = true; //TODO: make it as a setting
    m_cntInfoRequested = false;
    connect(&m_currentStatus,SIGNAL(Changed()),this,SLOT(HandleChangeOfStatus()));
    quint8 dummy = 0;
    quint16 svnVer = 0;
    MRIMPluginSystem::PluginSystem()->getQutimVersion(dummy,dummy,dummy,svnVer);
    UserAgent qutimAgent(QApplication::applicationName(),QApplication::applicationVersion(),QString("%1").arg(svnVer),PROTO_VERSION_MAJOR,PROTO_VERSION_MINOR);
    m_userAgent.Set(qutimAgent);
}

MRIMProto::~MRIMProto()
{
    delete m_typingTimer;
    delete m_PingTimer;
    delete m_mpopTimer;
    delete m_clParser;
    delete m_typersList;
    delete m_RecvBuffer;
    delete m_IMSocket;
    delete m_SrvRequestSocket;
}

void MRIMProto::Connect(QString aLogin, QString aPass, QString aHost, quint32 aPort, const Status &aStatus)
{
    m_unreadMsgs = 0;
    m_msgSequenceNum = 0;
    m_login = aLogin;
    m_pass = aPass;
    m_IMServerSelectorHost = aHost;
    m_IMServerSelectorPort = aPort;
    m_firstStatus.Clone(aStatus);
    m_currentStatus = StatusData(STATUS_OFFLINE);    
    m_prevStatus = StatusData(STATUS_OFFLINE);    
    receiveGoodServer();
}

void MRIMProto::receiveGoodServer()
{
    m_SrvRequestSocket = new QTcpSocket(this);
    qDebug()<<"Using proxy: "<<m_proxy.hostName()<<":"<<m_proxy.port()<<" proxy type="<<(quint32)m_proxy.type()<<" user="<<m_proxy.user()<<" pass="<<m_proxy.password();
    m_SrvRequestSocket->setProxy(m_proxy);
    QObject::connect(m_SrvRequestSocket, SIGNAL(connected()), this, SLOT( connectedToSrvRequestServer() ));
    QObject::connect(m_SrvRequestSocket, SIGNAL(disconnected()), this, SLOT( disconnectedFromSrvRequestServer() ));
    m_SrvRequestSocket->connectToHost(m_IMServerSelectorHost, m_IMServerSelectorPort);
}

void MRIMProto::connectedToSrvRequestServer()
{//catched connected signal
    QBuffer* buffer = new QBuffer(this);
    QTextStream stream(buffer);
    buffer->open(QIODevice::ReadWrite);
    QTcpSocket::SocketState state = m_SrvRequestSocket->state();
    bool connected = false;

    switch (state)
    {
    case QAbstractSocket::ConnectedState:
        connected = true;
        break;
    case QAbstractSocket::ConnectionRefusedError: //TODO
        break;
    case QAbstractSocket::ClosingState: //TODO
        break;
    default:
        break;
    }

    if (connected)
    {
        if (!m_SrvRequestSocket->waitForReadyRead())
            throw 1;
        quint64 bytesToRead = m_SrvRequestSocket->bytesAvailable();
        if (bytesToRead)
        {
            bool ok;
            QByteArray bytes = m_SrvRequestSocket->readAll();
            m_SrvRequestSocket->disconnectFromHost();
            buffer->write(bytes);
            QString str(bytes);
            QStringList list = str.split(":", QString::SkipEmptyParts);
            m_IMServerHost = new QString(list[0]);
            m_IMServerPort = list[1].toULong(&ok, 10);
            m_IMSocket = new QTcpSocket(this);
            m_IMSocket->setProxy(m_proxy);
            connect(m_IMSocket, SIGNAL( connected() ), this, SLOT( connectedToIMServer() ));
            connect(m_IMSocket, SIGNAL( disconnected() ), this, SLOT( disconnectedFromIMServer() ));
            connect(m_IMSocket, SIGNAL( readyRead() ), this, SLOT( readDataFromSocket() ));
#ifdef DEBUG_LEVEL_DEV
            qDebug()<<*m_IMServerHost;
#endif
            m_IMSocket->connectToHost(*m_IMServerHost, m_IMServerPort);
        }
    }
}

void MRIMProto::disconnectedFromSrvRequestServer()
{//catched disconnect event
    qDebug("Changing server...");
}

void MRIMProto::connectedToIMServer()
{//we are at IM server, start communication from HELLO
    MRIMPacket hello;
    hello.SetMsgType(MRIM_CS_HELLO);
    hello.SetBody("");
    hello.Send(m_IMSocket);
}

void MRIMProto::disconnectedFromIMServer()
{
    qDebug("Disconnected from IM server!");
    StopPing();
    m_prevStatus.Clone(m_currentStatus);
    m_currentStatus.Clear();
    m_currentStatus = STATUS_OFFLINE;
    MRIMEventHandler().sendDisconnectedEvent(m_login);
    emit ProtoStatusChanged(m_currentStatus.GetData());
    SetAllContactsOffline();
}

void MRIMProto::SetAllContactsOffline()
{
    if (!m_clParser) return;
    quint32 itemsCount = m_clParser->GetItemsCount();
    for (quint32 i=0; i < itemsCount; i++)
    {
        MRIMCLItem* item = m_clParser->ItemByIndex(i);
        if (item == NULL) continue;

        if (item->Type() == EContact)
        {
            MRIMContact* contact = reinterpret_cast<MRIMContact*>(item);

            if ( contact->Email() != "phone" )
            {
                contact->SetStatus(STATUS_OFFLINE);
            }
        }
    }
}

void MRIMProto::readDataFromSocket()
{//ready to read data
    if (!m_SaveBufPos && m_RecvBuffer != NULL)
    {
        delete m_RecvBuffer;
        m_RecvBuffer = NULL;
    }

    if (m_RecvBuffer == NULL)
    {
        m_RecvBuffer = new QBuffer(this);
        m_RecvBuffer->open(QIODevice::ReadWrite);
        m_SaveBufPos = false;
    }

    quint64 rememberPos = m_RecvBuffer->pos();
    if (m_SaveBufPos)
    {
        m_RecvBuffer->seek(m_RecvBuffer->size());
    }		

    m_RecvBuffer->write(m_IMSocket->readAll());

    if (!m_SaveBufPos)
    {
        m_RecvBuffer->seek(0);
    }
    else
    {
        m_RecvBuffer->seek(rememberPos);
    }

    while (true)
    {
        qint64 totalBuffSize = m_RecvBuffer->size();
        qint64 bytesLeft = totalBuffSize - m_RecvBuffer->pos();       
        if (bytesLeft >= sizeof(mrim_packet_header_t))
        {
            MRIMPacket* packet = NULL;
            TPacketErrorCodes errCode = MRIMPacket::TryMakeFromRawData(*m_RecvBuffer,packet);

            if (errCode == ENotEnoughBytes)
            {
                m_SaveBufPos = true;
                break;
            }

            if (packet != NULL)
            {
                HandleMRIMPacket(packet);
            }
            else
            {                            
                qDebug("Error occured while reading packet. Possibly packet is corrupted or internal error.");
                break;
            }
        } 
        else 
        {
            if (bytesLeft == 0)
            {
                m_SaveBufPos = false;
            }
            else if (bytesLeft > 0)
            {
                m_SaveBufPos = true;
            }
            break;
        }
    }	
}

bool MRIMProto::HandleMRIMPacket(MRIMPacket* aPacket)
{
    if (aPacket == NULL) return false;
    QDataStream out(aPacket->Data(),QIODevice::ReadOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    quint32 logoutReason = 0;
    quint32 unreadMsgs = 0;
    switch (aPacket->MsgType())
    {
    case MRIM_CS_HELLO_ACK:
#ifdef DEBUG_LEVEL_DEV
        qDebug("HELLO_ACK packet!");
#endif
        out >> m_PingPeriod;
        SendLOGINPacket();
        break;
    case MRIM_CS_LOGIN_ACK:
#ifdef DEBUG_LEVEL_DEV
        qDebug("LOGIN_ACK packet! starting ping!");
#endif
        StartPing();
        m_prevStatus.Clear();
        m_prevStatus = STATUS_OFFLINE;
        m_currentStatus.Clone(m_firstStatus); 
        RequestMPOPKey();
        MRIMEventHandler().sendConnectedEvent(m_login,m_currentStatus);
        emit ProtoStatusChanged(m_currentStatus.GetData());
        break;
    case MRIM_CS_LOGIN_REJ:
#ifdef DEBUG_LEVEL_DEV
        qDebug("LOGIN_REJ packet! disconnecting!");
#endif
        m_prevStatus.Clear();
        m_currentStatus.Clear();
        m_prevStatus = STATUS_OFFLINE;
        m_currentStatus = STATUS_OFFLINE;
        emit LogoutReceived(EAuthenticationFailed);
        break;
    case MRIM_CS_MPOP_SESSION:
#ifdef DEBUG_LEVEL_DEV
        qDebug("MPOP key packet!");
#endif
        HandleMPOPSessionAck(aPacket);
        break;
    case MRIM_CS_CONTACT_LIST2:
#ifdef DEBUG_LEVEL_DEV
        qDebug("CONTACT_LIST2 packet!");
#endif
        HandleContactList(aPacket);
        break;
    case MRIM_CS_USER_STATUS:
#ifdef DEBUG_LEVEL_DEV
        qDebug("USER_STATUS packet!");
#endif
        HandleUserStatusChanged(aPacket);
        break;
    case MRIM_CS_USER_INFO:
#ifdef DEBUG_LEVEL_DEV
        qDebug("USER_INFO packet!");
#endif
        HandleUserInfo(aPacket);
        break;
    case MRIM_CS_MESSAGE_ACK:
#ifdef DEBUG_LEVEL_DEV
        qDebug("MESSAGE_ACK packet!");
#endif
        HandleMessageAck(aPacket);
        break;
    case MRIM_CS_OFFLINE_MESSAGE_ACK:
#ifdef DEBUG_LEVEL_DEV
        qDebug("OFFLINE_MESSAGE_ACK packet!");
#endif
        HandleOfflineMessageAck(aPacket);
        break;
    case MRIM_CS_ADD_CONTACT_ACK:
#ifdef DEBUG_LEVEL_DEV
        qDebug("ADD_CONTACT_ACK packet!");
#endif
        HandleAddContactAck(aPacket);
        break;
    case MRIM_CS_MESSAGE_STATUS:
#ifdef DEBUG_LEVEL_DEV
        qDebug("MESSAGE_STATUS packet!");
#endif
        HandleMessageStatusPacket(aPacket);
        break;
    case MRIM_CS_AUTHORIZE_ACK:
#ifdef DEBUG_LEVEL_DEV
        qDebug("AUTHORIZE_ACK packet!");
#endif
        HandleAuthorizeAckPacket(aPacket);
        break;
    case MRIM_CS_LOGOUT:		
#ifdef DEBUG_LEVEL_DEV
        qDebug("LOGOUT packet!");
#endif
        out >> logoutReason;

        if (logoutReason & LOGOUT_NO_RELOGIN_FLAG)
        {
            m_doNotReconnect = true;
            emit LogoutReceived(EAnotherClientConnected);
        }
        else
        {
            //m_doNotReconnect = false;
            emit LogoutReceived(EUnknownReason);
        }
        break;
    case MRIM_CS_MAILBOX_STATUS:
#ifdef DEBUG_LEVEL_DEV
        qDebug("MAILBOX_STATUS packet!");
#endif
        out >> unreadMsgs;
        m_unreadMsgs = unreadMsgs;
        emit MailboxStatusChanged(unreadMsgs);
        break;
    case MRIM_CS_CONNECTION_PARAMS:
#ifdef DEBUG_LEVEL_DEV
        qDebug("CONNECTION_PARAMS packet!");
#endif
        //ignore it as not used
        break;
    case MRIM_CS_GET_MPOP_SESSION:
#ifdef DEBUG_LEVEL_DEV
        qDebug("MPOP_SESSION_ACK packet!");
#endif
        HandleMPOPSessionAck(aPacket);  
        break;
    case MRIM_CS_MODIFY_CONTACT_ACK:
#ifdef DEBUG_LEVEL_DEV
        qDebug("MODIFY_CONTACT_ACK packet!");
#endif
        HandleModifyContactAck(aPacket);
        break;
    case MRIM_CS_ANKETA_INFO:
#ifdef DEBUG_LEVEL_DEV
        qDebug("ANKETA_INFO packet!");
#endif
        HandleAnketaInfo(aPacket);
        break;
    case MRIM_CS_FILE_TRANSFER:
#ifdef DEBUG_LEVEL_DEV
        qDebug("FILE_TRANSFER request packet!");
#endif
        HandleFileTransferRequest(aPacket);
        break;
    case MRIM_CS_SMS_ACK:
        {
            qint32 status = 0;
            out >> status;
            qDebug()<<"SMS send status: "<<status;

            if (status == 1)
            {//message sent ok
                emit NotifyUI("SMS message sent OK!");
            }
        }
        break;
    case MRIM_CS_NEW_MAIL:
        HandleNewMail(aPacket);
        break;
    default: //not yet supported
#ifdef DEBUG_LEVEL_DEV
        qDebug("Unsupported packet! id: 0x%x",aPacket->MsgType());
#endif
        break;
    }
    return true;
}

void MRIMProto::StartPing()
{
    m_PingTimer = new QTimer(this);
    QObject::connect(m_PingTimer, SIGNAL(timeout()), this, SLOT(SendPINGPacket()));
    m_PingTimer->start(m_PingPeriod*1000);
}

void MRIMProto::StopPing()
{
    if (!m_PingTimer) return;

    if (m_PingTimer->isActive())
    {
        m_PingTimer->stop();
    }
}

void MRIMProto::SendPINGPacket()
{
    MRIMPacket packet;
    packet.SetMsgType(MRIM_CS_PING);
    packet.SetBody("");
    packet.Send(m_IMSocket);
}

void MRIMProto::SendLOGINPacket()
{
    MRIMPacket loginPacket;
    loginPacket.SetMsgType(MRIM_CS_LOGIN2);
    loginPacket.Append(m_login);
    loginPacket.Append(m_pass);
    loginPacket.Append(m_firstStatus.Get());
    loginPacket.Append(m_firstStatus.Stringify());
    loginPacket.Append(m_firstStatus.GetTitle(),true);
    loginPacket.Append(m_firstStatus.GetDescription(),true);
    loginPacket.Append(ProtoFeatures());
    loginPacket.Append(m_userAgent.Stringify());
    loginPacket.Append("ru"); //TEMP !!
#if PROTO_VERSION_MINOR >= 20
    loginPacket.Append(0); //NULL
    loginPacket.Append(0); //NULL
#endif
    loginPacket.Append(QString("%1 %2;").arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));
#ifdef DEBUG_LEVEL_DEV
    qDebug()<<"Sending LOGIN request...";
#endif
    loginPacket.Send(m_IMSocket);
}

void MRIMProto::HandleContactList(MRIMPacket* aPacket)
{	
    emit NewCLReceived();

    if (m_clParser)
    {
        delete m_clParser;            
    }
    m_clParser = new MRIMContactList(m_login,*aPacket->Data());
    m_clParser->Parse();
}

void MRIMProto::HandleUserStatusChanged(MRIMPacket* aPacket)
{
    if (!m_clParser || !aPacket) return;

    quint32 numStatus = STATUS_UNDETERMINATED, comSupport;
    QString statusName, statusTitle, statusDescr, email, userAgent;

    aPacket->Read(numStatus);
    aPacket->Read(&statusName);
    aPacket->Read(&statusTitle,true);
    aPacket->Read(&statusDescr,true);
    aPacket->Read(&email);
    aPacket->Read(comSupport);
    aPacket->Read(&userAgent);

    qDebug()<<"User "<<email<<" changed status to "<<statusName<<" with title="<<statusTitle<<" and descr="<<statusDescr<<", userAgent="<<userAgent; 
    MRIMContact* foundContact = m_clParser->CntByEmail(email);

    if (foundContact)
    {
        Status newStatus(numStatus,statusTitle,statusDescr,statusName);
        foundContact->SetStatus(newStatus);
        UserAgent* contactAgent = UserAgent::Parse(userAgent);
        foundContact->SetUserAgent(*contactAgent);
        delete contactAgent;
    }
}

void MRIMProto::HandleUserInfo(MRIMPacket* aPacket)
{
    QMap<QString,QString>* userInfoArray = new QMap<QString,QString>();
    QBuffer* buff = new QBuffer(this);
    buff->open(QIODevice::ReadWrite);
    qint64 written = buff->write(*aPacket->Data());
    Q_UNUSED(written);
    buff->seek(0);
    while(buff->pos() < buff->size())
    {
        LPString* name = ByteUtils::ReadToLPS(*buff);
        LPString* value = ByteUtils::ReadToLPS(*buff,true);
#ifdef DEBUG_LEVEL_DEV
        qDebug()<<"Param read. Name="<<name->String()<<", Value="<<value->String();
#endif
        userInfoArray->insert(name->String(),value->String());
    }

    MRIMUserInfo userInfo;
    userInfo.messagesTotal = userInfoArray->take("MESSAGES.TOTAL");
    userInfo.messagesUnread = userInfoArray->take("MESSAGES.UNREAD");        
    userInfo.userNickname = userInfoArray->take("MRIM.NICKNAME");
    //userInfo.userHasMyMail = userInfoArray->take("HAS_MYMAIL");
    userInfo.userClientEndpoint = userInfoArray->take("client.endpoint");
    bool ok;
    m_unreadMsgs = userInfo.messagesUnread.toUInt(&ok,10);
    if (!ok) m_unreadMsgs = 0;
    emit AccountInfoRecieved(userInfo);
    delete buff;
}

void MRIMProto::HandleMessageAck(MRIMPacket* aPacket)
{
    QBuffer* buff = new QBuffer(this);
    buff->open(QIODevice::ReadWrite);
    qint64 written = buff->write(*aPacket->Data());
    Q_UNUSED(written);
    buff->seek(0);

    quint32 msgId = ByteUtils::ReadToUL(*buff);
    quint32 flags = ByteUtils::ReadToUL(*buff);
    QString from = ByteUtils::ReadToString(*buff);

    if (flags & MESSAGE_FLAG_NOTIFY)
    {//typing notification
        MRIMContact* cnt = m_clParser->CntByEmail(from);
        if (cnt)
        {
            bool isAlreadyThere = false;
            for (quint32 i=0; i < m_typersList->count(); i++)
            {
                if (m_typersList->at(i).typingContact->Email() == cnt->Email())
                {
                    isAlreadyThere = true;
                    TypingStruct temp;
                    temp.typingContact = cnt;
                    temp.secsLeft = 10;
                    m_typersList->replace(i,temp);
                    break;
                }
            }

            if (!isAlreadyThere)
            {
                TypingStruct typer;
                typer.typingContact=cnt;
                typer.secsLeft = 10;
                m_typersList->append(typer);
                if (!m_typingTimer->isActive())
                {
                    m_typingTimer->setInterval(1000);
                    m_typingTimer->setSingleShot(false);
                    m_typingTimer->start();
                }
                emit ContactTyping(cnt->Email(),QString(cnt->GroupId()));	
            }
        }
        else
        {	//ignore not in list typers
            //emit ContactTyping(from->String(),"");	
        }
        return;
    }

    bool isAuth = (flags & MESSAGE_FLAG_AUTHORIZE);
    bool isUnicode = !(flags & MESSAGE_FLAG_CP1251);
    QString msg = ByteUtils::ReadToString(*buff,isUnicode);
    bool hasRtf = (flags & MESSAGE_FLAG_RTF);

    if (hasRtf)
    {//RTF processing
        QString rtfMsg = ByteUtils::ReadToString(*buff,false);
        msg = MRIMCommonUtils::ConvertToPlainText(rtfMsg);        
    }

    //	if (!(flags & MESSAGE_FLAG_OFFLINE))
    if (true)
    {
        if (!isAuth && !(flags & MESSAGE_FLAG_NORECV))
        {
            SendDeliveryReport(from,msgId);
        }

        MRIMContact* cnt = m_clParser->CntByEmail(from);

        if (isAuth)
        {
            QByteArray authTextDecoded = QByteArray::fromBase64(msg.toAscii());
            QBuffer buff(&authTextDecoded);            
            quint32 strCount = ByteUtils::ReadToUL(buff);

            if (strCount < 2)
                msg = "";
            else
            {
                QString nick = ByteUtils::ReadToString(buff,isUnicode);
                msg = ByteUtils::ReadToString(buff,isUnicode);
                msg.append(QString(" (%1)").arg(nick));
            }
        }

        if (cnt)
        {			
            emit MessageRecieved(cnt->Email(),QString(cnt->GroupId()),msg,QDateTime::currentDateTime(),hasRtf,isAuth);
        }
        else if (from.contains('@'))
        {
            Status stat(STATUS_UNDETERMINATED);
            UserAgent emptyAgent;
            MRIMContact* newCnt = new MRIMContact(m_login,0,from,from,-1,-1,stat,0,QString(),
                                                  emptyAgent,0,true,true);
            m_clParser->AddItem(newCnt);
            emit MessageRecieved(newCnt->Email(),"-1",msg,QDateTime::currentDateTime(),hasRtf,isAuth);
        }
        else
        {
            emit NotifyUI(msg);
        }
    }
    delete buff;
}

void MRIMProto::HandleOfflineMessageAck(MRIMPacket* aPacket)
{
    if (aPacket == 0) return;    
    QBuffer* buff = new QBuffer(this);
    buff->open(QIODevice::ReadWrite);
    buff->write(*aPacket->Data());
    buff->seek(0);
    quint32 uidl = ByteUtils::ReadToUL(*buff);
    quint32 uidl2 = ByteUtils::ReadToUL(*buff);
    LPString* msg = ByteUtils::ReadToLPS(*buff);
    if (!msg) return;
    MRIMOfflineMessage offMsg;
    bool parsedOk = ParseOfflineMessage(msg->String(),offMsg);
    delete msg;

    if (parsedOk)
    {
        bool isAuth = false;
        bool hasRtf = false;

        if (offMsg.Flags & MESSAGE_FLAG_AUTHORIZE)
        {
            isAuth = true;
        }

        if (offMsg.Flags & MESSAGE_FLAG_RTF)
        {
            hasRtf = true;
            offMsg.Message = MRIMCommonUtils::ConvertToPlainText(offMsg.Message);
        }

        QString msgStr = tr("Offline message ")+"("+offMsg.DateTime.toString(Qt::SystemLocaleShortDate)+")\n"+offMsg.Message;
        MRIMContact* cnt = m_clParser->CntByEmail(offMsg.From);
        if (cnt)
        {
            emit MessageRecieved(cnt->Email(),QString(cnt->GroupId()),msgStr,QDateTime::currentDateTime(),hasRtf,isAuth);
        }
        else if (offMsg.From.contains('@'))
        {            
            Status stat(STATUS_UNDETERMINATED);
            UserAgent emptyAgent;
            MRIMContact* newCnt = new MRIMContact(m_login,0,offMsg.From,offMsg.From,-1,-1,stat,0,QString(),emptyAgent,0,false);

            if (m_clParser->AddItem(newCnt))
            {
                emit AddItemToUI(EContact,QString::number(-1),newCnt->Email(),newCnt->Name(),stat.GetData(),false);
            }
            emit MessageRecieved(newCnt->Email(),"",offMsg.Message,QDateTime::currentDateTime(),hasRtf,isAuth);
        }
        else
        {
            emit NotifyUI(msgStr);
        }
        //respond
        MRIMPacket offMsgRespPacket;
        offMsgRespPacket.SetMsgType(MRIM_CS_DELETE_OFFLINE_MESSAGE);
        QByteArray packet;
        packet.append(ByteUtils::ConvertULToArray(uidl));
        packet.append(ByteUtils::ConvertULToArray(uidl2));
        offMsgRespPacket.SetBody(packet);
        QByteArray* rawPacket = offMsgRespPacket.ConvertToByteArray();
        m_IMSocket->write(*rawPacket);
        delete rawPacket;
    }
    else
    {//do nothing:)

    }
}

bool MRIMProto::ParseOfflineMessage(QString aRawMsg, MRIMOfflineMessage& aMsg)
{
    bool res = false;
    QRegExp charsetRx("charset=([\\w\\d-_]+)\\n");
    QRegExp fromRx("From:\\s([a-zA-Z0-9\\-\\_\\.]+@[a-zA-Z0-9\\-\\_]+\\.+[a-zA-Z]+)\\n");
    QRegExp dateRx("Date:\\s([a-zA-Z0-9, :]+)\\n");
    QRegExp subjectRx("Subject:\\s(\\b[\\w\\s]+\\b)\\n");
    QRegExp mrimFlagsRx("X-MRIM-Flags:\\s([0-9]+)\\n");
    QRegExp boundaryRx("Boundary:\\s(\\b\\w+\\b)\\n");
    QRegExp versionRx("Version:\\s([0-9\\.]+)\\n");
    QRegExp msgRx("\\n\\n(.+)\\n--{boundary}--");
    QDateTime dateTime;
    if (fromRx.indexIn(aRawMsg) == -1) return res;
    aMsg.From = fromRx.cap(1);
    if (dateRx.indexIn(aRawMsg) == -1) return res;
    aMsg.DateTime = QLocale("en").toDateTime(dateRx.cap(1),"ddd, dd MMM yyyy hh:mm:ss");
    if (subjectRx.indexIn(aRawMsg) == -1) return res;
    aMsg.Subject = subjectRx.cap(1);
    if (mrimFlagsRx.indexIn(aRawMsg) == -1) return res;
    bool ok = false;
    aMsg.Flags = 0;
    aMsg.Flags = mrimFlagsRx.cap(1).toULong(&ok,16);
    if (boundaryRx.indexIn(aRawMsg) == -1) return res;
    QString boundary = boundaryRx.cap(1);
    if (versionRx.indexIn(aRawMsg) == -1) return res;
    msgRx.setPattern(msgRx.pattern().replace("{boundary}",boundary));
    if (msgRx.indexIn(aRawMsg) == -1) return res;
    aMsg.Message = msgRx.cap(1);
    res = true;
    return res;
}

void MRIMProto::SendTypingToContact(QString aEmail)
{
    if (IsOnline())
        SendMessageToContact(aEmail," ",99,false,true);
}

void MRIMProto::SendMessageToContact(QString aEmail, QString aMessage, quint32 aKernelMsgId, bool aIsAuth, bool aIsTyping)
{
    MRIMPacket messagePacket;
    messagePacket.SetMsgType(MRIM_CS_MESSAGE);
    messagePacket.SetSequence(m_msgSequenceNum);
    while (m_msgIdsLinks.count() >= MRIM_MSGQUEUE_MAX_LEN)
    {
        m_msgIdsLinks.dequeue();
    }
    MsgIdsLink idsLink;
    idsLink.qutimKernelNum = aKernelMsgId;
    idsLink.mrimMsgSequence = m_msgSequenceNum;
    MRIMContact* contact = m_clParser->CntByEmail(aEmail);
    if (contact != NULL)
    {
        idsLink.contactEmail = contact->Email();
        idsLink.groupId = contact->GroupId();
    }
    else
    {		
        idsLink.contactEmail = aEmail;
        idsLink.groupId = 0;
    }
    m_msgIdsLinks.enqueue(idsLink);
    m_msgSequenceNum++;

    quint32 flags = 0;

    if (aIsAuth)
    {
        flags |= MESSAGE_FLAG_AUTHORIZE;
        flags |= MESSAGE_FLAG_NORECV;
    }

    if (aIsTyping)
    {
        flags |= MESSAGE_FLAG_NOTIFY;
    }

    messagePacket.Append(flags);
    messagePacket.Append(aEmail);
    messagePacket.Append(aMessage,true);
    messagePacket.Append(" ");
    messagePacket.Send(m_IMSocket);
}

void MRIMProto::TypingTimerStep()
{
    if (m_typersList->count() == 0)
    {
        m_typingTimer->stop();
        return;
    }

    int count = m_typersList->count();
    int i = 0;
    while(i < count)
    {
        TypingStruct& typer = (*m_typersList)[i];
        typer.secsLeft--;
        if (typer.secsLeft <= 0)
        {
            emit ContactTypingStopped(typer.typingContact->Email(),QString(typer.typingContact->GroupId()));
            m_typersList->removeAt(i);
            count--;
        }
        i++;
    }
}

void MRIMProto::SendAuthorizationTo(QString aEmail)
{
    MRIMPacket authPacket;
    authPacket.SetMsgType(MRIM_CS_AUTHORIZE);
    authPacket.Append(aEmail);

    MRIMContact* authedCnt = m_clParser->CntByEmail(aEmail);
    if (authedCnt)
    {
        authedCnt->SetAuthed(true);
    }
    authPacket.Send(m_IMSocket);
}

bool MRIMProto::IsInList(QString aEmail)
{
    if (!m_clParser) return false;
    MRIMContact* found = m_clParser->CntByEmail(aEmail);
    return (found != NULL && found->GroupId() >= 0);
}

void MRIMProto::AddContact(QString aEmail, QString aName, quint32 aGroupId, bool aIsAuthed, bool aIsAuthedMe)
{
    if (!m_clParser)
    {
        m_clParser = new MRIMContactList(m_login);
    }
    if (m_addingContact)
        delete m_addingContact;
    UserAgent emptyAgent;
    Status stat(STATUS_UNDETERMINATED);
    m_addingContact = new MRIMContact(m_login,0,aName,aEmail,0,aGroupId,stat,0,QString(),
                                      emptyAgent,0,false);

    if (!IsOnline())
    {//offline addition
        m_addingContact->SetAuthed(aIsAuthed);
        m_addingContact->SetAuthedMe(aIsAuthedMe);
        m_clParser->AddItem(m_addingContact);
        m_addingContact = NULL;
    }
    else
    {
        quint32 flags = 0;
        MRIMPacket addContactPacket;
        addContactPacket.SetMsgType(MRIM_CS_ADD_CONTACT);
        addContactPacket.Append(flags);
        addContactPacket.Append(aGroupId);
        addContactPacket.Append(aEmail);
        addContactPacket.Append(aName,true);
        addContactPacket.Append(QString()); //phones
        QByteArray authMsg;
        authMsg.append(ByteUtils::ConvertULToArray(2));
        LPString nick(m_UserInfo.userNickname,true);
        authMsg.append(nick.ToRaw());
        LPString authText(tr("Pls authorize and add me to your contact list! Thanks!"),true);
        authMsg.append(authText.ToRaw());        
        authMsg = authMsg.toBase64();
        LPString encodedAuthMsg(authMsg);
        addContactPacket.Append(encodedAuthMsg); //auth text
        addContactPacket.Append(ADD_CONTACT_ACTION_FLAG_MYMAIL_INVITE); //actions. invite user to My world on mail.ru
        addContactPacket.Send(m_IMSocket);
    }
}

void MRIMProto::AddGroup(QString aName, quint32 aId)
{
    if (!m_clParser)
    {
        m_clParser = new MRIMContactList(m_login);
    }
    if (m_addingGroup)
        delete m_addingGroup;
    m_addingGroup = new MRIMGroup(m_login,0,QString::number(aId),aName);

    if (!IsOnline())
    {
        m_clParser->AddItem(m_addingGroup);
        m_addingGroup = NULL;
    }
    else
    {
        MRIMPacket addGroupPacket;
        addGroupPacket.SetMsgType(MRIM_CS_ADD_CONTACT);
        addGroupPacket.Append(CONTACT_FLAG_GROUP);
        addGroupPacket.Append(0);
        addGroupPacket.Append(aName);
        addGroupPacket.Send(m_IMSocket);
    }
}

void MRIMProto::HandleAddContactAck(MRIMPacket* aPacket)
{
    quint32 result = ByteUtils::ReadToUL(*aPacket->Data());
    quint32 contId = ByteUtils::ReadToUL(*aPacket->Data(),4);
#ifdef DEBUG_LEVEL_DEV
    qDebug()<<"New user addition to CL, contId="<<contId<<", operation result="<<result;
#endif
    if (result == CONTACT_OPER_SUCCESS)
    {
        if (m_addingContact != NULL)
        {			
            MRIMContact* oldContact = m_clParser->CntByEmail(m_addingContact->Email());
            if (oldContact)
            {
                m_clParser->DeleteEntry(oldContact);
                emit RemoveUserFromCL(m_addingContact->Email());
            }

            m_addingContact->SetId(contId);
            if (m_clParser->AddItem(m_addingContact))
            {
                AddItemToUI(EContact,QString::number(m_addingContact->GroupId()),m_addingContact->Email(),m_addingContact->Name(),m_addingContact->GetStatus().GetData(),false);
            }
            m_addingContact = NULL;
        }
    }
    else
    {
        emit CLOperationFailed(ConvertCLErrorFromNative(result));
    }
}

QList<MRIMGroup*> MRIMProto::GetAllGroups()
{
    QList<MRIMGroup*> grList;
    if (m_clParser == 0) return grList;
    for (qint32 i=0; i < m_clParser->GetItemsCount(); i++)
    {
        MRIMCLItem* item = m_clParser->ItemByIndex(i);
        if (item != 0)
        {
            if (item->Type() == EGroup)
            {
                MRIMGroup* group = reinterpret_cast<MRIMGroup*>(item);
                grList.append(group);
            }
        }
    }

    return grList;
}


QList<MRIMCLItem*>* MRIMProto::GetAllCL()
{
    if (m_clParser == NULL) return 0;
    return m_clParser->GetCL();
}

bool MRIMProto::ChangeStatus(const Status &aNewStatus)
{
    bool res = false;

    if (IsOnline() && IsOnline(aNewStatus))
    {
        SendStatusChangePacket(aNewStatus);
        res = true;
    }
    return res;
}

void MRIMProto::SendStatusChangePacket(const Status& aNewStatus)
{
    if (IsOnline(aNewStatus))
    {
        MRIMPacket changeStatusPacket;
        changeStatusPacket.SetMsgType(MRIM_CS_CHANGE_STATUS);
        changeStatusPacket.Append(aNewStatus.Get());
        changeStatusPacket.Append(aNewStatus.Stringify());
        changeStatusPacket.Append(aNewStatus.GetTitle(),true);
        changeStatusPacket.Append(aNewStatus.GetDescription(),true);
        changeStatusPacket.Append(m_login);
        changeStatusPacket.Append(ProtoFeatures());
        changeStatusPacket.Append(m_userAgent.Stringify());
        qDebug()<<"Sending status change packet...";
        changeStatusPacket.Send(m_IMSocket);       
        m_prevStatus.Clone(m_currentStatus);
        m_currentStatus.Clone(aNewStatus);
        MRIMEventHandler().sendStatusChangedEvent(m_login,m_currentStatus);
        emit ProtoStatusChanged(m_currentStatus.GetData());        
    }
}

void MRIMProto::HandleMessageStatusPacket(MRIMPacket* aPacket)
{
    if (!m_clParser || !aPacket) return;
    quint32 deliveryStatus = ByteUtils::ReadToUL(*aPacket->Data());

    if (deliveryStatus == MESSAGE_DELIVERED)
    {
        qDebug()<<"Message "<<QString::number(aPacket->Sequence())<<" delivered";
    }
    else
    {
        qDebug()<<"Message "<<QString::number(aPacket->Sequence())<<" delivery ERROR!";
    }

    for (qint32 i=0; i < m_msgIdsLinks.count(); i++)
    {
        if (m_msgIdsLinks[i].mrimMsgSequence == aPacket->Sequence())
        {
            emit MessageDelivered(m_msgIdsLinks[i].contactEmail,QString(m_msgIdsLinks[i].groupId),m_msgIdsLinks[i].qutimKernelNum);
            m_msgIdsLinks.removeAt(i);
            break;
        }
    }
}

void MRIMProto::SendDeliveryReport(QString aFrom, quint32 aMsgId)
{
    MRIMPacket deliveryPacket;
    deliveryPacket.SetMsgType(MRIM_CS_MESSAGE_RECV);
    deliveryPacket.Append(aFrom);
    deliveryPacket.Append(aMsgId);
    qDebug()<<"Sending delivery report...";
    deliveryPacket.Send(m_IMSocket);
}

void MRIMProto::HandleAuthorizeAckPacket(MRIMPacket* aPacket)
{
    if (!m_clParser || !aPacket) return;

    LPString* authedEmail = ByteUtils::ReadToLPS(*aPacket->Data());

    MRIMContact* authedContact = m_clParser->CntByEmail(authedEmail->String());
    if (authedContact)
    {
        authedContact->SetAuthedMe(true);
        emit AuthorizeResponseReceived(authedContact->Email(),QString(authedContact->GroupId()));
    }
}

void MRIMProto::SetProxy(QNetworkProxy aProxy)
{
    m_proxy = aProxy;
    if (m_SrvRequestSocket)
    {
        m_SrvRequestSocket->setProxy(m_proxy);
    }
    if (m_IMSocket)
    {
        m_IMSocket->setProxy(m_proxy);
    }
}
void MRIMProto::DisconnectFromIM()
{	
    SetAllContactsOffline();
    if (!m_IMSocket) return;
    if (m_IMSocket->isValid())
    {
        m_IMSocket->disconnectFromHost();
    }
}

void MRIMProto::HandleMPOPSessionAck(MRIMPacket* aPacket)
{
    if (!aPacket) return;

    quint32 opStatus;
    QString mpopKey;
    aPacket->Read(opStatus);
    aPacket->Read(&mpopKey);

    if (opStatus == MRIM_GET_SESSION_SUCCESS)
    {
        emit MPOPKeyReceived(mpopKey);
    }
    m_mpopTimer->singleShot(KMpopRequestPeriod,this,SLOT(RequestMPOPKey()));
}

void MRIMProto::RequestMPOPKey()
{
    if (!IsOnline()) return;

    MRIMPacket mpopReqPacket;
    mpopReqPacket.SetMsgType(MRIM_CS_GET_MPOP_SESSION);
    mpopReqPacket.Append("");
    qDebug()<<"Sending MPOP request...";
    mpopReqPacket.Send(m_IMSocket);
}

bool MRIMProto::IsContactAuthedByMe(QString aEmail)
{
    if (!m_clParser) return true;
    MRIMContact* cnt = m_clParser->CntByEmail(aEmail);
    if (!cnt) return true;
    return cnt->IsAuthed();
}

bool MRIMProto::IsContactAuthedMe(QString aEmail)
{
    if (!m_clParser) return true;
    MRIMContact* cnt = m_clParser->CntByEmail(aEmail);
    if (!cnt) return true;
    return cnt->IsAuthedMe();
}

MRIMContact* MRIMProto::GetContactByEmail(QString aEmail)
{
    if (!m_clParser) return NULL;

    return m_clParser->CntByEmail(aEmail);
}

void MRIMProto::RemoveUserFromCL(QString aEmail)
{
    SendModifyContact(aEmail,"",0,CONTACT_FLAG_REMOVED,EKeepOldValues);
}

void MRIMProto::SendModifyContact(QString aEmail, QString aNewName, quint32 aNewGroupId, quint32 aFlags, TModifyFlags aSpecialFlags)
{
    if (!m_clParser || m_modifingContact) return;

    MRIMContact* cnt = m_clParser->CntByEmail(aEmail);
    if (!cnt) return;

    m_modifingContact = cnt;
    if (aFlags & CONTACT_FLAG_REMOVED)
    {
        m_currentClOp = EDelete;
    }
    else
    {
        m_currentClOp = EModify;
    }

    quint32 id = 0;
    quint32 grId = 0;
    QString name;

    if (aSpecialFlags & EKeepOldValues)
    {
        id = cnt->Id();
        grId = cnt->GroupId();
        name = cnt->Name();
    }
    else
    {
        id = cnt->Id();
        grId = aNewGroupId;
        if (grId == -1) grId = cnt->GroupId();
        name = aNewName;
    }

    MRIMPacket modifyPacket;
    modifyPacket.SetMsgType(MRIM_CS_MODIFY_CONTACT);
    modifyPacket.Append(id);
    modifyPacket.Append(aFlags);
    modifyPacket.Append(grId);
    modifyPacket.Append(aEmail);
    modifyPacket.Append(name,true);

    if (cnt->HasPhone())
    {
        QString phones = cnt->Phone().join(",");
        modifyPacket.Append(phones.remove('+'));
    }
    modifyPacket.Send(m_IMSocket);
}

void MRIMProto::HandleModifyContactAck(MRIMPacket* aPacket)
{
    if (!m_clParser || !aPacket) return;

    quint32 opStatus = ByteUtils::ReadToUL(*aPacket->Data());

    if (m_modifingContact)
    {
        if (opStatus == CONTACT_OPER_SUCCESS)
        {
            if (m_currentClOp == EDelete)
            {
                qDebug()<<"Delete contact operation succeeded!";
                m_clParser->DeleteEntry(m_modifingContact);
            }
            else
            {
                qDebug()<<"Modify contact operation succeeded!";
                //modify
            }

            m_modifingContact = NULL;
            m_currentClOp = ENoOperation;
        }
        else
        {
            emit CLOperationFailed(ConvertCLErrorFromNative(opStatus));
            m_modifingContact = NULL;
            m_currentClOp = ENoOperation;
        }
    }
}

CLOperationError MRIMProto::ConvertCLErrorFromNative(quint32 aNativeErrorCode)
{
    CLOperationError localErrCode = ECLUnknownError;
    switch (aNativeErrorCode)
    {
    case CONTACT_OPER_NO_SUCH_USER:
        localErrCode = ECLNoSuchUser;
        break;
    case CONTACT_OPER_INTERR:
        localErrCode = ECLInternalServerError;
        break;
    case CONTACT_OPER_INVALID_INFO:
        localErrCode = ECLInvalidInfo;
        break;
    case CONTACT_OPER_USER_EXISTS:
        localErrCode = ECLUserAlreadyExists;
        break;
    case CONTACT_OPER_GROUP_LIMIT:
        localErrCode = ECLGroupLimitReached;
        break;
    }
    return localErrCode;
}

quint32 MRIMProto::ConvertCLErrorToNative(CLOperationError aLocalErrorCode)
{
    quint32 nativeErrCode = CONTACT_OPER_ERROR;
    switch (aLocalErrorCode)
    {
    case ECLNoSuchUser:
        nativeErrCode = CONTACT_OPER_NO_SUCH_USER;
        break;
    case ECLInternalServerError:
        nativeErrCode = CONTACT_OPER_INTERR;
        break;
    case ECLInvalidInfo:
        nativeErrCode = CONTACT_OPER_INVALID_INFO;
        break;
    case ECLUserAlreadyExists:
        nativeErrCode = CONTACT_OPER_USER_EXISTS;
        break;
    case ECLGroupLimitReached:
        nativeErrCode = CONTACT_OPER_GROUP_LIMIT;
        break;
    default:
        break;
    }
    return nativeErrCode;
}

void MRIMProto::StartSearch(MRIMSearchParams aParams)
{
    int fieldsCount = 0;
    if (IsOnline())
    {
        MRIMPacket searchPacket;
        searchPacket.SetMsgType(MRIM_CS_WP_REQUEST);

        //append pairs of param-value for search
        if (aParams.EmailAddr.length() > 0 && aParams.EmailDomain.length() > 0)
        {
            searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_USER);
            searchPacket.Append(aParams.EmailAddr);
            searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_DOMAIN);
            searchPacket.Append(aParams.EmailDomain);
            fieldsCount+=2;
        }
        else
        {
            if (aParams.Nick.length() > 0 && aParams.Nick != "*")
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_NICKNAME);
                searchPacket.Append(aParams.Nick);
                fieldsCount++;
            }

            if (aParams.Name.length() > 0 && aParams.Name != "*")
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_FIRSTNAME);
                searchPacket.Append(aParams.Name);
                fieldsCount++;
            }

            if (aParams.Surname.length() > 0 && aParams.Surname != "*")
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_LASTNAME);
                searchPacket.Append(aParams.Surname);
                fieldsCount++;
            }

            if (aParams.Sex != -1)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_SEX);
                searchPacket.Append(QString::number(aParams.Sex));
                fieldsCount++;
            }

            if (aParams.MinAge != -1)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_DATE1);
                searchPacket.Append(QString::number(aParams.MinAge));
                fieldsCount++;
            }

            if (aParams.MaxAge != -1)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_DATE2);
                searchPacket.Append(QString::number(aParams.MaxAge));
                fieldsCount++;
            }

            if (aParams.CityId != -1)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_CITY_ID);
                searchPacket.Append(QString::number(aParams.CityId));
                fieldsCount++;
            }

            if (aParams.CountryId != -1)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_COUNTRY_ID);
                searchPacket.Append(QString::number(aParams.CountryId));
                fieldsCount++;
            }

            if (aParams.ZodiacId != -1)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_ZODIAC);
                searchPacket.Append(QString::number(aParams.ZodiacId));
                fieldsCount++;
            }

            if (aParams.BirthDay != -1)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_BIRTHDAY_DAY);
                searchPacket.Append(QString::number(aParams.BirthDay));
                fieldsCount++;
            }

            if (aParams.BirthdayMonth != -1)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_BIRTHDAY_MONTH);
                searchPacket.Append(QString::number(aParams.BirthdayMonth));
                fieldsCount++;
            }

            if (aParams.OnlineOnly == true)
            {
                searchPacket.Append(MRIM_CS_WP_REQUEST_PARAM_ONLINE);
                searchPacket.Append(QString::number(1));
                fieldsCount++;
            }
        }

        if (fieldsCount > 0)
        {
            searchPacket.Send(m_IMSocket);
        }
    }
}

void MRIMProto::HandleAnketaInfo(MRIMPacket* aPacket)
{
    if (!aPacket) return;

    QBuffer* buff = new QBuffer(this);
    buff->open(QIODevice::ReadWrite);
    qint64 written = buff->write(*aPacket->Data());
    Q_UNUSED(written);
    buff->seek(0);
    QList<QString> fieldNames;
    quint32 opStatus = ByteUtils::ReadToUL(*buff);
    Q_UNUSED(opStatus);
    quint32 fieldsNum = ByteUtils::ReadToUL(*buff);
    quint32 maxRows = ByteUtils::ReadToUL(*buff);
    Q_UNUSED(maxRows);
    quint32 serverTime = ByteUtils::ReadToUL(*buff);
    Q_UNUSED(serverTime);
    QString fieldName;

    for (quint32 i=0; i < fieldsNum; i++)
    {
        fieldName = ByteUtils::ReadToString(*buff);
        fieldNames.append(fieldName);
    }

    quint32 totalFound = 0;
    QList<MRIMSearchParams*> foundList;
    QHash<QString,QString> cntValues;
    QString fieldValue;

    while (!buff->atEnd())
    {                
        for (quint32 i=0; i < fieldsNum; i++)
        {
            fieldValue = ByteUtils::ReadToString(*buff,IsUnicodeAnketaField(fieldNames[i]));
            cntValues.insert(fieldNames[i],fieldValue);
        }	
        MRIMSearchParams* cnt = ParseForm(cntValues);

        if (cnt) foundList.append(cnt);
        totalFound++;
        cntValues.clear();;
    }

    emit SearchFinished(foundList);
}

MRIMSearchParams* MRIMProto::ParseForm(QHash<QString,QString>& aUnparsedForm)
{
    QList<QString> keys = aUnparsedForm.keys();
    MRIMSearchParams* contact = new MRIMSearchParams;

    for (int i=0; i < keys.count(); i++)
    {
        QString key = keys[i];
        QString val = aUnparsedForm.value(keys[i]);
        bool ok = false;
#ifdef DEBUG_LEVEL_DEV
        qDebug()<<key<<"="<<val;
#endif

        if (key == "Username")
        {
            contact->EmailAddr = val;
        }

        if (key == "Domain")
        {
            contact->EmailDomain = val;
        }

        if (key == "FirstName")
        {
            contact->Name = val;
        }

        if (key == "LastName")
        {
            contact->Surname = val;
        }

        if (key == "Nickname")
        {
            contact->Nick = val;
        }

        if (key == "Sex")
        {		
            ok = false;
            contact->Sex = val.toInt(&ok);
            if (!ok)
            {
                contact->Sex = -1;
            }
        }

        if (key == "Country_id")
        {
            ok = false;
            contact->CountryId = val.toInt(&ok);
            if (!ok)
            {
                contact->CountryId = -1;
            }
        }

        if (key == "City_id")
        {
            ok = false;
            contact->CityId = val.toInt(&ok);
            if (!ok)
            {
                contact->CityId = -1;
            }
        }

        if (key == "BDay")
        {
            ok = false;
            contact->BirthDay = val.toInt(&ok);
            if (!ok)
            {
                contact->BirthDay = -1;
            }
        }

        if (key == "BMonth")
        {
            ok = false;
            contact->BirthdayMonth = val.toInt(&ok);
            if (!ok)
            {
                contact->BirthdayMonth = -1;
            }
        }


        if (key == "Birthday")
        {
            ok = false;
            contact->BirthYear = val.left(4).toInt(&ok);
            if (!ok)
            {
                contact->BirthYear = -1;
            }
        }

        if (key == "Zodiac")
        {
            ok = false;
            contact->ZodiacId = val.toInt(&ok);
            if (!ok)
            {
                contact->ZodiacId = -1;
            }
        }

        if (key == "Location")
        {			
            contact->LocationText = val;
        }

        if (key == "mrim_status")
        {
            ok = false;
            contact->Status = val.toInt(&ok,16);
            if (!ok)
            {
                contact->Status = -1;
            }
        }
    }
    //not used values
    contact->MinAge = -1;
    contact->MaxAge = -1;

    return contact;
}

void MRIMProto::RequestCntInfo(QString aEmail)
{
    MRIMSearchParams params;
    QStringList addr = aEmail.split("@");
    params.EmailAddr = addr[0];
    params.EmailDomain = addr[1];
    m_cntInfoRequested = true;
    StartSearch(params);
}

MRIMContact* MRIMProto::GetCnt(QString aEmail)
{
    if (!m_clParser) return NULL;

    return m_clParser->CntByEmail(aEmail);
}

void MRIMProto::HandleFileTransferRequest(MRIMPacket* aPacket)
{
    FileTransferRequest* req = new FileTransferRequest;
    QBuffer* buff = new QBuffer(this);
    buff->open(QIODevice::ReadWrite);
    qint64 written = buff->write(*aPacket->Data());
    Q_UNUSED(written);
    buff->seek(0);
    req->From = ByteUtils::ReadToString(*buff);
    req->UniqueId = ByteUtils::ReadToUL(*buff);
    req->SummarySize = ByteUtils::ReadToUL(*buff);
    ByteUtils::ReadToUL(*buff); //igonre 4 bytes
    QString files = ByteUtils::ReadToString(*buff);
    ByteUtils::ReadToString(*buff); //skip NULL string
    QString IPs = ByteUtils::ReadToString(*buff);

    QRegExp rx("[;:]");
    QStringList fileList = files.split(rx,QString::SkipEmptyParts);
    QStringListIterator filesIterator(fileList);
    bool ok = true;
    while (filesIterator.hasNext())
    {           
        QString key = filesIterator.next();
        if (!filesIterator.hasNext()) { ok = false; break; }
        req->FilesDict.insert(key,filesIterator.next().toUInt());
    }

    QStringList ipList = IPs.split(rx,QString::SkipEmptyParts);
    QStringListIterator IPsIterator(ipList);
    while (IPsIterator.hasNext())
    {
        QString key = IPsIterator.next();
        if (!IPsIterator.hasNext()) { ok = false; break; }
        req->IPsDict.insert(key,IPsIterator.next().toUInt());
    }

    if (!ok)
    {        
        emit NotifyUI(tr("File transfer request from %1 couldn't be processed!").arg(req->From));
        delete req;
        return;
    }

    m_fileReqsHash.insert(req->UniqueId,req);
    emit FileTransferRequested(*req);
}

void MRIMProto::SendSMS(QString aPhoneNumber, QString aText)
{
    MRIMPacket smsPacket;
    smsPacket.SetMsgType(MRIM_CS_SMS);
    smsPacket.Append(0);
    if (!aPhoneNumber.contains('+')) aPhoneNumber.insert(0,'+');
    smsPacket.Append(aPhoneNumber);
    smsPacket.Append(aText,true);
    smsPacket.Send(m_IMSocket);
}

void MRIMProto::DeclineFileTransfer(quint32 aUniqueId)
{
    SendFileTransferAck(aUniqueId,0);  //somehow decline code is zero %)
}

void MRIMProto::FileTransferCompleted(quint32 aUniqueId)
{
    if (!m_fileReqsHash.contains(aUniqueId)) return;
    FileTransferRequest* req = m_fileReqsHash.value(aUniqueId);
    m_fileReqsHash.remove(aUniqueId);
    delete req;
}

void MRIMProto::SendFileTransferAck(quint32 aUniqueId, quint32 aErrorCode, QString aIPsList)
{
    if (!m_fileReqsHash.contains(aUniqueId)) return;

    FileTransferRequest* req = m_fileReqsHash.value(aUniqueId);

    MRIMPacket ftAckPacket;    
    ftAckPacket.SetMsgType(MRIM_CS_FILE_TRANSFER_ACK);
    ftAckPacket.Append(aErrorCode);
    ftAckPacket.Append(req->From);
    ftAckPacket.Append(aUniqueId);
    ftAckPacket.Append(aIPsList);
    ftAckPacket.Send(m_IMSocket);

    m_fileReqsHash.remove(aUniqueId);
    delete req;
}

void MRIMProto::SendFileTransferRequest(const FileTransferRequest& aReq)
{
    FileTransferRequest* req = new FileTransferRequest;
    req->To = aReq.To;
    req->FilesDict = aReq.FilesDict;
    req->FilesInfo = aReq.FilesInfo;
    req->IPsDict = aReq.IPsDict;
    req->UniqueId = aReq.UniqueId;
    req->SummarySize = aReq.SummarySize;

    MRIMPacket ftPacket;
    ftPacket.SetMsgType(MRIM_CS_FILE_TRANSFER);
    ftPacket.Append(req->To);
    ftPacket.Append(req->UniqueId);
    ftPacket.Append(req->SummarySize);

    QString files, IPs;
    QHashIterator<QString,quint32> iter = QHashIterator<QString,quint32>(req->FilesDict);
    iter.toFront();

    while (iter.hasNext())
    {
        QHash<QString,quint32>::const_iterator item = iter.next();        
        files.append(item.key());
        files.append(';');
        files.append(QString::number(item.value()));
        files.append(';');
    }

    iter = QHashIterator<QString,quint32>(req->IPsDict);
    iter.toFront();

    while (iter.hasNext())
    {
        QHash<QString,quint32>::const_iterator item = iter.next();
        IPs.append(item.key());
        IPs.append(';');
        IPs.append(QString::number(item.value()));
        IPs.append(';');
    }

    qint32 stringsLen = files.length() + IPs.length() + 4*3;

    ftPacket.Append(stringsLen);
    ftPacket.Append(files);
    ftPacket.Append("");
    ftPacket.Append(IPs);
    ftPacket.Send(m_IMSocket);

    m_fileReqsHash.insert(req->UniqueId,req);
}

void MRIMProto::HandleChangeOfStatus()
{
    SendStatusChangePacket(m_currentStatus);
}

bool MRIMProto::IsOnline()
{
    return m_currentStatus.IsOnline();
}

bool MRIMProto::IsOnline( const Status& aStatus )
{
    return aStatus.IsOnline();
}

bool MRIMProto::IsUnicodeAnketaField(const QString& aFieldName)
{
    QString lcName = aFieldName.toLower();

    if (lcName == "firstname")
        return true;
    if (lcName == "lastname")
        return true;
    if (lcName == "nickname")
        return true;
    if (lcName == "location")
        return true;
    if (lcName == "status_title")
        return true;
    if (lcName == "status_desc")
        return true;
    return false;
}

void MRIMProto::HandleNewMail(MRIMPacket* aPacket)
{
    quint32 unreadCount,date,uidl;
    QString from,subject;
    aPacket->Read(unreadCount);
    aPacket->Read(&from);
    aPacket->Read(&subject);
    aPacket->Read(date); //ignore
    aPacket->Read(uidl); //ignore
    QString newMailMessage = "New e-mail recieved!<br/>From: %1<br/>Subject: %2<br/>";
    m_unreadMsgs = unreadCount;
    emit MailboxStatusChanged(m_unreadMsgs);
    emit NotifyUI(newMailMessage.arg(from).arg(subject));
}
