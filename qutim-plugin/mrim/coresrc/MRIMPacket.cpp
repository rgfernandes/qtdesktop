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

#include "MRIMPacket.h"
#include "MRIMUtils.h"
#include <QDataStream>
#include <QBuffer>
#include <QTcpSocket>
#include <qendian.h>

MRIMPacket::MRIMPacket() : m_Header(NULL), m_Body(NULL), m_currBodyPos(0)
{
    InitializeHeader();
}

MRIMPacket::~MRIMPacket()
{
}

void MRIMPacket::InitializeHeader()
{
    if (m_Header == NULL)
    {
        m_Header = new mrim_packet_header_t();
        m_Header->magic = CS_MAGIC;
        m_Header->from = 0;
        m_Header->dlen = 0;
        m_Header->fromport = 0;
        m_Header->proto = PROTO_VERSION;
        m_Header->seq = 0;
        m_Header->msg = 0;
        m_Header->reserved.fill(0,16);
    }
}

void MRIMPacket::SetHeader(QByteArray& aHeader)
{
#ifdef DEBUG_LEVEL_DEV
    qDebug("Input array size = %d",aHeader.count());
#endif
    QDataStream in(aHeader);    
    in.setByteOrder(QDataStream::LittleEndian);
    InitializeHeader();   
    in >> m_Header->magic;
    in >> m_Header->proto;
    in >> m_Header->seq;
    in >> m_Header->msg;
    in >> m_Header->dlen;
    in >> m_Header->from;
    in >> m_Header->fromport;
    in >> m_Header->reserved;      
}

void MRIMPacket::SetHeader(mrim_packet_header_t& aHeader)
{
    m_Header = new mrim_packet_header_t(aHeader);    
}

void MRIMPacket::SetBody(QByteArray& aBody)
{
    if (m_Body != NULL)
        delete m_Body;

    m_Body = new QByteArray(aBody);
    m_Header->dlen=m_Body->length();
}


void MRIMPacket::SetBody(QString& aBody)
{
    if (m_Body != NULL)
        delete m_Body;    
    m_Body = new QByteArray(aBody.toAscii());
    m_Header->dlen=m_Body->length();
}

void MRIMPacket::SetBody(const char aBody[])
{
    QString str(aBody);
    SetBody(str);
}

void MRIMPacket::SetMsgType(quint32 aMsgType)
{
    m_Header->msg = aMsgType;
}

void MRIMPacket::SetFrom(quint32 aFrom)
{
    m_Header->from = aFrom;
}

void MRIMPacket::SetFromPort(quint32 aFromPort)
{
    m_Header->fromport = aFromPort;
}

void MRIMPacket::SetSequence(quint32 aSeq)
{
    m_Header->seq = aSeq;
}

QByteArray* MRIMPacket::ConvertToByteArray()
{
    QBuffer* buffer = new QBuffer(this);
    QDataStream stream(buffer);
    stream.setByteOrder(QDataStream::LittleEndian);
    buffer->open(QIODevice::ReadWrite);
    stream << m_Header->magic;
    stream << m_Header->proto;
    stream << m_Header->seq;
    stream << m_Header->msg;
    stream << m_Header->dlen;
    stream << m_Header->from;
    stream << m_Header->fromport;    
    QByteArray* temp = new QByteArray(buffer->data());
    temp->append(m_Header->reserved);    
    temp->append(*m_Body);    
    return temp;
}

TPacketErrorCodes MRIMPacket::TryMakeFromRawData(QBuffer& aRawPacket, MRIMPacket*& aOutPacket)
{
    MRIMPacket* packet = new MRIMPacket();
    QByteArray cont = aRawPacket.read(HEADER_SIZE);
    packet->SetHeader(cont);
    quint64 estimBodySize = aRawPacket.size()-aRawPacket.pos();	

    if ( packet->IsHeaderCorrect() && estimBodySize >= packet->DataLenght() )
    {
        bool isSeekOk = true;
        if (isSeekOk)
        {	
            cont = aRawPacket.read(packet->DataLenght());
            packet->SetBody(cont);
        }
        else
        {
            delete packet;
            packet = NULL;
        }
    }
    else
    {
        if (packet->IsHeaderCorrect())
        {
            delete packet;
            packet = NULL;
            aRawPacket.seek(aRawPacket.pos()-HEADER_SIZE);
            return ENotEnoughBytes;
        }
        delete packet;
        packet = NULL;
        return EHeaderCorrupted;
    }
    aOutPacket = packet;
    return ENoError;
}

bool MRIMPacket::IsHeaderCorrect()
{
    if (m_Header->magic != CS_MAGIC)
    {
        return false;
    }
    return true;
}

void MRIMPacket::Append( const QString &aStr, bool bUnicode )
{
    LPString str(aStr,bUnicode);
    Append(str);
}

void MRIMPacket::Append( LPString &aStr )
{
    if (!m_Body)
    {
        m_Body = new QByteArray();
    }

    m_Body->append(aStr.ToRaw());
    m_Header->dlen = m_Body->length();
}

void MRIMPacket::Append( const quint32 &aNum )
{
    if (!m_Body)
    {
        m_Body = new QByteArray();
    }

    m_Body->append(ByteUtils::ConvertULToArray(aNum));
    m_Header->dlen = m_Body->length();
}

qint64 MRIMPacket::Send( QTcpSocket *aSocket )
{
    qint64 nRes = -1;

    if (!aSocket)
    {
        return nRes;
    }

    QByteArray *data = ConvertToByteArray();
    nRes = aSocket->write(*data);
    delete data;
    return nRes;
}

MRIMPacket& MRIMPacket::operator<<( LPString &aStr )
{
    Append(aStr);
    return *this;
}

MRIMPacket& MRIMPacket::operator<<( const quint32 &aNum )
{
    Append(aNum);
    return *this;
}

qint32 MRIMPacket::Read( QString *pStr, bool bUnicode )
{
    if (!pStr) return -1;
    *pStr = ByteUtils::ReadToString(*Data(),m_currBodyPos,bUnicode);
    m_currBodyPos += sizeof(quint32);
    m_currBodyPos += pStr->size() * ((bUnicode)?2:1);
    return pStr->size();
}

qint32 MRIMPacket::Read( quint32 &aNum )
{
    aNum = ByteUtils::ReadToUL(*Data(),m_currBodyPos);
    m_currBodyPos += sizeof(aNum);
    return sizeof(aNum);
}

