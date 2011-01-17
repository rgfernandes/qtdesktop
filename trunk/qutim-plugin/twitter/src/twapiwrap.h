/*
		twApiWrap

		Copyright (c) 2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef TWAPIWRAP_H
#define TWAPIWRAP_H

#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QScriptEngine>

enum CurrentConnectionState {Online,Connecting,Offline};

struct Buddy {
	QString id;
	QString name;
	QString screen_name;
	QString location;
	QString description;
	QString profile_image_url;
	QString url;
	quint16 followers_count;
	quint16 friends_count;
	quint16 favourites_count;
	quint16 statuses_count;
	QString status_id;
	QString status_text;
	QString avatar_hash;
};

struct BuddyStatus {
	QString id;
	QString text;
};

struct DirectMessage {
	QString id;
	QString sender;
	QString text;
	QDateTime datetime;
};


class twApiWrap : public QObject
{
    Q_OBJECT
public:
	twApiWrap(const QString &account_name, const QString &profile_name);
	~twApiWrap();

	void sendMessage(const QString &buddy_id, const QString &message);
	void loadSettings();
	void startRequests();
	void stopAll();

public slots:
	void sendRequests();

private slots:
	void getReply(QNetworkReply*);

signals:
	void iMDisconnected();
	void iMConnected();
	void arrivedStatuses(QHash<QString, BuddyStatus>);
	void arrivedBuddies(QList<Buddy>&, QString);
	void arrivedDirectMessages(QList<DirectMessage>&);

private:
	QNetworkAccessManager *m_network_handler;
	QNetworkRequest m_network_request;
	QString m_account_name;
	QString m_profile_name;
	CurrentConnectionState m_current_state;
	void processBuddies(QString, QString);
	void processDirectMessages(QString);
	quint16 m_check_every;
	QString m_last_direct_message_id;

};

#endif // TWAPIWRAP_H
