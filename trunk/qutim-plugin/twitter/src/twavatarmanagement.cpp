#include "twavatarmanagement.h"
#include <QFile>
#include <QDir>

twAvatarManagement::twAvatarManagement(const QString &account_name,
                                     const QString &profile_name,
        QObject *parent) : QObject(parent),
m_account_name(account_name),
m_profile_name(profile_name)
{

}

twAvatarManagement::~twAvatarManagement()
{

}

void twAvatarManagement::requestForAvatar(const QString &buddy_id, const QString &avatar_url)
{
	if ( !m_request_list.contains(buddy_id) )
    {
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
	 this, SLOT(replyFinished(QNetworkReply*)));
	manager->setProperty("avatar_url",avatar_url);
	manager->setProperty("buddy_id",buddy_id);
	manager->get(QNetworkRequest(QUrl(avatar_url)));

	m_request_list.insert(buddy_id, manager);
    }
}

void twAvatarManagement::replyFinished(QNetworkReply *reply)
{
	QNetworkAccessManager *manager = reply->manager();
	if (m_request_list.contains(manager->property("buddy_id").toString())) {
		QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/twitter."+m_account_name, "accountsettings");
		QDir dir(settings.fileName().section('/', 0, -3) + "/twittericons");
		if (!dir.exists()) dir.mkdir(dir.path());
		QByteArray filename = QCryptographicHash::hash(manager->property("avatar_url").toByteArray(), QCryptographicHash::Md5).toHex();
		QFile file(dir.path() + "/" + filename);
		if (!file.exists())	{
			if (!file.open(QIODevice::WriteOnly)) return;
			file.write(reply->readAll());
			file.close();
		}

//		emit avatarDownloaded(manager->property("buddy_id").toString(), filename);

		m_request_list.remove(manager->property("buddy_id").toString());
		//delete manager;
	}
}
