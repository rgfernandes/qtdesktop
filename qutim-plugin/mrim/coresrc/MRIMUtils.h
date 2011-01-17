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

#ifndef MRIMUTILS_H_
#define MRIMUTILS_H_

#include <QByteArray>
#include <QString>
#include <QBuffer>
#include <QTextCodec>
#include <QIcon>
#include "proto.h"
#include "mrimdefs.h"
#include "MRIMContactList.h"
#include "MRIMPluginSystem.h"

class LPString
{
public:
    LPString(QString aStr, bool bUnicode = false);
    LPString(QByteArray& aStr, bool bUnicode = false);
    LPString(const char* aStr, bool bUnicode = false);
    virtual ~LPString();
    void ReadFromByteArray(QByteArray& aArr);
    static LPString* ReadFromRaw(QBuffer& aBuffer);
    QByteArray& ToRaw();
    QString& String();
private:
    QString* m_String;
    QByteArray* m_RawData;
    bool m_bUnicode;
};

class ByteUtils
{
public:
    static QByteArray ConvertULToArray(const quint32 UL);
    static quint32 ConvertArrayToUL(const QByteArray& arr);
    static quint32 ReadToUL(QBuffer& aBuff);
    static quint32 ReadToUL(QByteArray& aArr, quint32 aPosInArray = 0);
    static LPString* ReadToLPS(QBuffer& aBuff, bool bUnicode = false);
    static LPString* ReadToLPS(QByteArray& aArr, quint32 aPosInArray = 0, bool bUnicode = false);
    static QString ReadToString(QBuffer& aBuff, bool bUnicode = false);
    static QString ReadToString(QByteArray& aArr, quint32 aPosInArray = 0, bool bUnicode = false);
};

class MRIMCommonUtils : public QObject
{
Q_OBJECT
public:
    static QString ConvertToPlainText(QString aRtfMsg);
    static QPoint DesktopCenter(QSize aWidgetSize);
    static QString GetFileSize(quint64 aSize);
};


#endif /*MRIMUTILS_H_*/
