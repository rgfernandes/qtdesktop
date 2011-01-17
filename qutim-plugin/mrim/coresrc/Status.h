/*****************************************************************************
Status

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

#ifndef STATUS_H
#define STATUS_H

#include <QIcon>
#include <QString>
#include <QObject>
#include "proto.h"

struct StatusData
{
    StatusData(quint32 aStatus, const QString &aTitle, const QString &aDescription, const QString &aCustomID);
    StatusData(quint32 aStatus);

    friend bool operator==(const StatusData& aStatusData, const StatusData& aOtherStatusData);
    friend bool operator!=(const StatusData& aStatusData, const StatusData& aOtherStatusData);
    friend bool operator==(const StatusData& aStatusData, quint32 aOtherStatus);
    friend bool operator!=(const StatusData& aStatusData, quint32 aOtherStatus);

    quint32 m_status;
    QString m_customStatusID;
    QString m_title;
    QString m_descr;
};

class Status : public QObject
{
Q_OBJECT
public:
    Status(quint32 aStatus = STATUS_UNDETERMINATED, const QString &aTitle = QString(), const QString &aDescription = QString(), const QString &aCustomID = QString());
    Status(const StatusData &aStatusData);
    ~Status();

    StatusData GetData() const;
    inline bool IsOnline() const { return (m_statusData.m_status != STATUS_OFFLINE && m_statusData.m_status != STATUS_UNDETERMINATED); }
    quint32 GetMass();
    static quint32 GetMass(quint32 aNumStatus, const QString &aCustomID = QString());
    quint32 Get() const;
    QString GetTitle() const;
    QString GetDescription() const;
    
    void Set(quint32 nNewStatus, const QString &aCustomID = QString());
    void SetTitle(const QString& aTitle);
    void SetDescription(const QString& aDescription);
    void SetCustomID(const QString &aCustomID);
    static quint32 FromString(const QString& aString);
    QString Stringify() const;
    static QString Stringify(quint32 nStatus, const QString &aCustomStatusID = QString());
    QIcon GetIcon() const;
    static QIcon GetIcon(quint32 nStatus, const QString &aCustomStatusID = QString());
    static QIcon GetIcon(const QString &aStatusName);
    QString GetIconPath() const;
    static QString GetIconPath(quint32 nStatus, const QString &aCustomStatusID = QString());

    void Clear();
    
    void Clone( const Status &aNewStatus, bool aNotifyChange = false );
    Status& operator=(const StatusData &aNewStatusData);
    Status& operator=(quint32 aNewStatus);
    friend bool operator==(const Status& aStatus, const Status& aOtherStatus);
    friend bool operator!=(const Status& aStatus, const Status& aOtherStatus);
    friend bool operator==(const Status& aStatus, quint32 aOtherStatus);
    friend bool operator!=(const Status& aStatus, quint32 aOtherStatus);

signals:
    void Changed();
private:
    StatusData m_statusData;
};

#endif
