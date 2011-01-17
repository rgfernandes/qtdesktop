/*****************************************************************************
    MRIMUtils

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

#include "MRIMUtils.h"
#include <QString>
#include <QDataStream>
#include <QBuffer>
#include <qendian.h>
#include "rtf/rtfimport.h"
#include <QApplication>
#include <QDesktopWidget>


LPString::LPString(QString aStr, bool bUnicode) : m_RawData(NULL), m_bUnicode(bUnicode)
{
    m_String = new QString(aStr);
}

LPString::LPString(QByteArray& aStr, bool bUnicode) : m_RawData(NULL), m_String(NULL), m_bUnicode(bUnicode)
{
    ReadFromByteArray(aStr);
}

LPString::LPString(const char* aStr, bool bUnicode) : m_RawData(NULL), m_String(NULL), m_bUnicode(bUnicode)
{
    QByteArray arr(aStr);
    ReadFromByteArray(arr);
}

void LPString::ReadFromByteArray(QByteArray& aArr)
{
    QString codepage = (m_bUnicode) ? "UTF-16LE" : "CP1251";
    QTextCodec* codec = QTextCodec::codecForName(codepage.toLocal8Bit());

    if (codec != NULL)
    {
        delete m_String;
        QTextCodec::ConverterState convState(QTextCodec::IgnoreHeader);
        m_String = new QString(codec->toUnicode(aArr.constData(),aArr.length(),&convState));
    }
}

QByteArray& LPString::ToRaw()
{
    if (!m_RawData)
    {
        m_RawData = new QByteArray;
    }

    QString codepage = (m_bUnicode) ? "UTF-16LE" : "CP1251";
    QTextCodec* codec = QTextCodec::codecForName(codepage.toLocal8Bit());
    
    if (codec != NULL)
    {
        QByteArray d;
        QTextCodec::ConverterState convState(QTextCodec::IgnoreHeader);

        if (m_String->size() > 0)
        {
        d.append(codec->fromUnicode(m_String->data(),m_String->size(),&convState));
        }
        m_RawData->append(ByteUtils::ConvertULToArray(d.length()));
        m_RawData->append(d);
    }
    return *m_RawData;
}

QString& LPString::String()
{
    return *m_String;
}

LPString::~LPString()
{
    delete m_RawData;
    delete m_String;
}


QByteArray ByteUtils::ConvertULToArray(const quint32 UL)
{
    QByteArray packet;
    packet[3] = (UL / 0x1000000);
    packet[2] = (UL / 0x10000);
    packet[1] = (UL / 0x100);
    packet[0] = (UL % 0x100);
    return packet;
}

quint32 ByteUtils::ConvertArrayToUL(const QByteArray& arr)
{
    bool ok;
    quint32 res = arr.toHex().toULong(&ok,16);
    res = qToBigEndian(res);
    return res;
}

quint32 ByteUtils::ReadToUL(QBuffer& aBuff)
{
    return ConvertArrayToUL(aBuff.read(4));
}

quint32 ByteUtils::ReadToUL(QByteArray& aArr, quint32 aPosInArray)
{
    return ConvertArrayToUL(aArr.mid(aPosInArray,4));
}

LPString* ByteUtils::ReadToLPS(QBuffer& aBuff, bool bUnicode)
{
    quint32 len = ReadToUL(aBuff);
    QByteArray str;
    str.append(aBuff.read(len));
    LPString* lps = new LPString(str,bUnicode);
    return lps;
}

LPString* ByteUtils::ReadToLPS(QByteArray& aArr, quint32 aPosInArray, bool bUnicode)
{
    quint32 len = ReadToUL(aArr,aPosInArray);
    QByteArray str;
    str.append(aArr.mid(aPosInArray+sizeof(len),len));
    LPString* lps = new LPString(str,bUnicode);
    return lps;
}


QString ByteUtils::ReadToString(QBuffer& aBuff, bool bUnicode)
{	
    LPString* lps = ReadToLPS(aBuff,bUnicode);
    QString res(lps->String());
    delete lps;
    return res;
}

QString ByteUtils::ReadToString( QByteArray& aArr, quint32 aPosInArray, bool bUnicode /*= false*/ )
{
    LPString* lps = ReadToLPS(aArr,aPosInArray,bUnicode);
    QString res(lps->String());
    delete lps;
    return res;
}

//MRIMCommonUtils

QString MRIMCommonUtils::ConvertToPlainText(QString aRtfMsg)
{
    QByteArray unbased = QByteArray::fromBase64(aRtfMsg.toAscii());
    QByteArray arr;
    quint32 beLen = qToBigEndian(unbased.length()*10);
    arr.append(ByteUtils::ConvertULToArray(beLen));
    arr.append(unbased);
    QByteArray uncompressed = qUncompress(arr);
#ifdef DEBUG_LEVEL_DEV
    qDebug()<<"Unpacked length: "<<uncompressed.count();
#endif
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);
    buf.write(uncompressed);
    buf.seek(0);
    quint32 numLps = ByteUtils::ReadToUL(buf);
#ifdef DEBUG_LEVEL_DEV
    qDebug()<<"Number of string-params in message: "<<numLps;
#endif
    QString plainText;

    if (numLps > 1)
    {
        QString rtfMsg = ByteUtils::ReadToString(buf,false);
#ifdef DEBUG_LEVEL_DEV
    qDebug()<<"Unbased and unzipped rtf message: "<<rtfMsg;
#endif
        QString color = ByteUtils::ReadToString(buf);//not used now
        Q_UNUSED(color);
        RTFImport rtfConverter;
        plainText = rtfConverter.convert(rtfMsg);
    }
    return plainText;
}

QPoint MRIMCommonUtils::DesktopCenter(QSize aWidgetSize)
{
    QDesktopWidget &desktop = *QApplication::desktop();
    return QPoint(desktop.width() / 2 - aWidgetSize.width() / 2, desktop.height() / 2 - aWidgetSize.height() / 2);
}

QString MRIMCommonUtils::GetFileSize(quint64 aSize)
{
    quint64 bytes = aSize % 1024;
    quint32 kBytes = aSize % (1024 * 1024) / 1024;
    quint32 mBytes = aSize % (1024 * 1024 * 1024) / (1024 * 1024);
    quint32 gBytes = aSize / (1024 * 1024 * 1024);

    QString fileSize;

    if ( bytes && !kBytes && !mBytes && !gBytes )
            fileSize.append(QString::number(bytes) + tr(" B"));
    else if (kBytes && !mBytes && !gBytes )
            fileSize.append(QString::number(kBytes) + "," + QString::number(bytes) + tr(" KB"));
    else if (mBytes && !gBytes )
            fileSize.append(QString::number(mBytes) + "," + QString::number(kBytes) + tr(" MB"));
    else if (gBytes )
            fileSize.append(QString::number(gBytes) + "," + QString::number(mBytes) + tr(" GB"));

    return fileSize;
}
