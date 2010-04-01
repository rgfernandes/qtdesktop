#ifndef TWAVATARMANAGEMENT_H
#define TWAVATARMANAGEMENT_H

#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class twAvatarManagement : public QObject
{
    Q_OBJECT
public:
	twAvatarManagement(const QString &account_name,
                      const QString &profile_name, QObject *parent = 0);
	~twAvatarManagement();
    void requestForAvatar(const QString &buddy_id, const QString &avatar_url);

private slots:
    void replyFinished(QNetworkReply*);

signals:
	void avatarDownloaded(const QString &, const QString &);

private:
    QString m_account_name;
    QString m_profile_name;
    QHash<QString,QNetworkAccessManager*> m_request_list;
};

#endif // TWAVATARMANAGEMENT_H
