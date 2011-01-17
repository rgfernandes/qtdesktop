/*

    Copyright (c) 2008      by Tiago Salem Herrmann <tiagosh@gmail.com>
    Kopete    (c) 2002-2003 by the Kopete developers <kopete-devel@kde.org>

    MSNProtocol (c) 2009 by Rustam Chakin <qutim.develop@gmail.com>

    *************************************************************************
    *                                                                       *
    * This library is free software; you can redistribute it and/or         *
    * modify it under the terms of the GNU General Public                   *
    * License as published by the Free Software Foundation; either          *
    * version 2 of the License, or (at your option) any later version.      *
    *                                                                       *
    *************************************************************************
*/

#ifndef MSNSOCKET_H
#define MSNSOCKET_H


#include <QSslSocket>

#include <msn/msn.h>

class QTimer;

class MSNSocket : public QSslSocket
{
  Q_OBJECT
public:
    explicit MSNSocket( MSN::NotificationServerConnection * mainConnection, bool isSSL = false);
    ~MSNSocket();

    bool isSSL() const { return mIsSSL; }

public slots:
    void incomingData();
    void connectionReady();
    void connectionFinished();
    void connectionEncryptedReady();

private slots:
    void resetPing();
    void pingTimeout();

private:
    void initPingTimer();

    MSN::NotificationServerConnection * mMainConnection;
    bool mIsSSL;
    QTimer* mPingTimer;
};


#endif // MSNSOCKET_H
