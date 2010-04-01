/*****************************************************************************
    MRIMPacket

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

#ifndef MRIMPACKET_H_
#define MRIMPACKET_H_

#include <QObject>
#include <QString>
#include <QByteArray>
#include "proto.h"

class QBuffer;
class LPString;
class QTcpSocket;

enum TPacketErrorCodes
{
    ENoError = 0,
    ENotEnoughBytes,
    EHeaderCorrupted,
    EUnknownError
};

class MRIMPacket : QObject
{
    Q_OBJECT
public:	

    MRIMPacket();
    virtual ~MRIMPacket();

    static TPacketErrorCodes TryMakeFromRawData(QBuffer& aRawPacket, MRIMPacket*& aOutPacket);
    void SetHeader(QByteArray& aHeader);
    void SetHeader(mrim_packet_header_t& aHeader);
    void SetBody(QByteArray& aBody);
    void SetBody(QString& aBody);
    void SetBody(const char aBody[]);
    void SetSequence(quint32 aSeq);
    void SetMsgType(quint32 aMsgType);
    void SetFrom(quint32 aFrom);
    void SetFromPort(quint32 aFromPort);

    void Append(const QString &aStr, bool bUnicode = false);
    void Append(LPString &aStr);
    void Append(const quint32 &aNum);

    qint32 Read(QString *pStr,bool bUnicode = false);
    qint32 Read(quint32 &aNum);

    MRIMPacket& operator<<(LPString &aStr);
    MRIMPacket& operator<<(const quint32 &aNum);

    bool IsHeaderCorrect();
    inline quint32 DataLenght() { return m_Header->dlen; }
    inline quint32 MsgType() { return m_Header->msg; } 
    inline quint32 Sequence() { return m_Header->seq; }
    inline QByteArray* Data() { return m_Body; } 

    void InitializeHeader();
    QByteArray* ConvertToByteArray();
    qint64 Send(QTcpSocket *aSocket);
private:
    mrim_packet_header_t* m_Header;
    mrim_connection_params_t* m_ConnectionParams;
    QByteArray* m_Body;

    quint32 m_currBodyPos;
};

#endif /*MRIMPACKET_H_*/
