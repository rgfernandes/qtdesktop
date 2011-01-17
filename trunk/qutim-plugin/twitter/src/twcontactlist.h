#ifndef TWCONTACTLIST_H
#define TWCONTACTLIST_H

#include <QtGui>
#include "twapiwrap.h"
#include "twpluginsystem.h"
#include "twavatarmanagement.h"

class twContactList : public QObject
{
    Q_OBJECT
public:
	twContactList(const QString &account_name, const QString &profile_name,
	 twApiWrap *api_wrap);
	~twContactList();
	QString getToolTip(const QString &);
	void removeCL();

private:
    QString m_account_name;
    QString m_profile_name;
	twPluginSystem &m_plugin_system;
	twApiWrap *m_api_wrap;
	twAvatarManagement *m_avatar_management;
    QIcon m_online_icon;
	QIcon m_offline_icon;\
	QStringList m_groups;
	QHash<QString, QStringList> m_group_contacts;
	QHash<QString, QString> m_contact_tooltips;


private slots:
	void processBuddies(QList<Buddy>&, QString);
	void processDirectMessages(QList<DirectMessage>&);
	void removeAll();

};

#endif // TWCONTACTLIST_H
