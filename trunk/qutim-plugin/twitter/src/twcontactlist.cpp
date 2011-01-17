#include "twcontactlist.h"
#include <QSettings>

twContactList::twContactList(const QString &account_name, const QString &profile_name,
		 twApiWrap *api_wrap)
: m_account_name(account_name),
m_profile_name(profile_name),
m_api_wrap(api_wrap),
m_plugin_system(twPluginSystem::instance())
{
	m_online_icon = QIcon(":/icons/twitter-online.png");
	m_offline_icon = QIcon(":/icons/twitter-offline.png");

	TreeModelItem item;
	item.m_protocol_name = "Twitter";
	item.m_account_name = m_account_name;
	item.m_item_name = m_account_name;
	item.m_item_type = 2;
	m_plugin_system.addItemToContactList(item, m_account_name);

	m_avatar_management = new twAvatarManagement(m_account_name, m_profile_name);

	connect(m_api_wrap, SIGNAL(arrivedBuddies(QList<Buddy>&, QString)), this, SLOT(processBuddies(QList<Buddy>&, QString)));
	connect(m_api_wrap, SIGNAL(arrivedDirectMessages(QList<DirectMessage>&)), this, SLOT(processDirectMessages(QList<DirectMessage>&)));
	connect(m_api_wrap, SIGNAL(iMDisconnected()), this, SLOT(removeAll()));
}

twContactList::~twContactList()
{

}

void twContactList::removeAll()
{
	foreach (QString group, m_groups) {
		TreeModelItem item;
		item.m_protocol_name = "Twitter";
		item.m_account_name = m_account_name;
		foreach (QString name, m_group_contacts[group]) {
			item.m_parent_name = "."+group;
			item.m_item_name = group;
			item.m_item_type = 0;
			m_plugin_system.removeItemFromContactList(item);
		}
		item.m_parent_name = m_account_name;
		item.m_item_name = "."+group;
		item.m_item_type = 1;
		m_plugin_system.removeItemFromContactList(item);
		m_group_contacts.clear();
		m_groups.clear();
		m_contact_tooltips.clear();
	}
}

void twContactList::removeCL()
{
	TreeModelItem item;
	item.m_protocol_name = "Twitter";
	item.m_account_name = m_account_name;
	item.m_item_name = m_account_name;
	item.m_item_type = 2;
	m_plugin_system.removeItemFromContactList(item);
}

QString twContactList::getToolTip(const QString &buddy_name)
{
	if (m_contact_tooltips.contains(buddy_name)) return m_contact_tooltips[buddy_name];
	else return NULL;

}

void twContactList::processBuddies(QList<Buddy> &buddies, QString callback)
{

	if (!m_groups.contains(callback)) {
		TreeModelItem item;
		item.m_protocol_name = "Twitter";
		item.m_account_name = m_account_name;
		item.m_parent_name = m_account_name;
		item.m_item_name = "."+callback;
		item.m_item_type = 1;

		QHash<QString, QString> groupname;
		groupname["friends"] = tr("Friends");
		groupname["followers"] = tr("Followers");

		m_plugin_system.addItemToContactList(item, groupname[callback]);
		m_groups.append(callback);
	}

	foreach (Buddy buddy, buddies) {
		if (!m_group_contacts[callback].contains(buddy.screen_name)) {

			TreeModelItem item;
			item.m_protocol_name = "Twitter";
			item.m_account_name = m_account_name;
			item.m_parent_name = "."+callback;
			item.m_item_name = buddy.screen_name;
			item.m_item_type = 0;

			m_plugin_system.addItemToContactList(item, buddy.screen_name);
			m_plugin_system.setContactItemStatus(item, m_online_icon, "online", 0);

			QString tooltip;
			tooltip.append("<table><tr><td>");
			tooltip.append("<img src=':/icons/twitter-online.png'> <b>" + buddy.screen_name + "</b><br>");
			tooltip.append("<font size='2'><b>"+tr("Name:")+"</b> "+buddy.name+"</font><br>");
			tooltip.append("<font size='2'><b>"+tr("Location:")+"</b> "+buddy.location+"</font><br>");
			tooltip.append("<font size='2'><b>"+tr("Description:")+"</b> "+buddy.description+"</font><br>");
			tooltip.append("<font size='2'><b>"+tr("Followers count:")+"</b> "+QString::number(buddy.followers_count)+"</font><br>");
			tooltip.append("<font size='2'><b>"+tr("Friends count:")+"</b> "+QString::number(buddy.friends_count)+"</font><br>");
			tooltip.append("<font size='2'><b>"+tr("Favourites count:")+"</b> "+QString::number(buddy.favourites_count)+"</font><br>");
			tooltip.append("<font size='2'><b>"+tr("Statuses count:")+"</b> "+QString::number(buddy.statuses_count)+"</font><br>");
			tooltip.append("<font size='2'><b>"+tr("Last status text:")+"</b> "+buddy.status_text+"</font><br>");

			QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/twitter."+m_account_name, "accountsettings");
			QDir dir(settings.fileName().section('/', 0, -3) + "/twittericons");
			QByteArray filename = QCryptographicHash::hash(buddy.profile_image_url.toUtf8(), QCryptographicHash::Md5).toHex();
			QFile file(dir.path() + "/" + filename);
			if (!file.exists())	{
				m_avatar_management->requestForAvatar(buddy.id, buddy.profile_image_url);
			}
			tooltip.append("</td><td><img src='"+dir.path()+"/"+filename+"' align='right'>");
			tooltip.append("</td></tr></table>");
			m_contact_tooltips[buddy.screen_name] = tooltip;
			m_group_contacts[callback].append(buddy.screen_name);
		}

	}
}

void twContactList::processDirectMessages(QList<DirectMessage> &direct_messages)
{
	foreach(DirectMessage direct_message, direct_messages) {
		TreeModelItem item;
		item.m_protocol_name = "Twitter";
		item.m_account_name = m_account_name;
		if (m_group_contacts["friends"].contains(direct_message.sender)) item.m_parent_name = ".friends";
		else if (m_group_contacts["followers"].contains(direct_message.sender)) item.m_parent_name = ".followers";
		item.m_item_name = direct_message.sender;
		item.m_item_type = 0;

		m_plugin_system.addMessageFromContact(item, direct_message.text, direct_message.datetime);
	}
}
