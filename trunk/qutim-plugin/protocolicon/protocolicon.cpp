/*****************************************************************************
    Protocol Icon Plugin

	Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "protocolicon.h"
#include <qutim/iconmanagerinterface.h>

bool ProtocolIcon::init(PluginSystemInterface *plugin_system)
{
	SimplePluginInterface::init(plugin_system);
	m_ya = QString::fromUtf8(QByteArray::fromHex("d0af2ed09ed0bdd0bbd0b0d0b9d0bd"));
	m_plugin_system = plugin_system;
	m_event_id = m_plugin_system->registerEventHandler("Core/ContactList/ItemAdded", this);
	m_plugin_icon = QIcon(":/icons/protocolicon.png");
	m_account = false;
	m_contact = false;
	return true;
}
void ProtocolIcon::setProfileName(const QString &profile_name)
{
	m_profile_name = profile_name;
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "protocolicon");
	QString path = settings.value("path",QString()).toString();
	m_account = settings.value("account", true).toBool();
	m_contact = settings.value("contact", true).toBool();
	if(path.isEmpty())
		return;
	QSettings style(path+"/Icons.plist", m_plugin_system->plistFormat());
	QVariantMap map_large = style.value("Interface-Large").toMap();
	QVariantMap map_small = style.value("Interface-Small").toMap();
	if(map_large.isEmpty() && map_small.isEmpty())
		return;
	QDir dir = path;
	QSet<QString> keys;
	keys += QSet<QString>::fromList( map_large.keys() );
	keys += QSet<QString>::fromList( map_small.keys() );
	foreach( const QString &key, keys )
	{
		IconInfo icon;
		QString small = dir.filePath( map_small.value( key ).toString() );
		QString large = dir.filePath( map_large.value( key ).toString() );
		icon.icon.addFile( small );
		icon.icon.addFile( large );
		if(icon.icon.isNull())
			continue;
		icon.name = key.toLower();
		icon.category = IconInfo::Protocol;
		SystemsCity::IconManager()->setIcon( icon, small );
		m_icons.insert( key, icon.icon );
	}
}

QWidget *ProtocolIcon::settingsWidget()
{
	ui = Ui::PluginSettings();
	m_widget = new QWidget();
	ui.setupUi(m_widget);
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "protocolicon");
	QStringList paths = SystemsCity::PluginSystem()->getSharePaths();
	foreach(QString path, paths)
	{
		QDir dir = QDir(path).filePath("protocolicons");
		QStringList themes(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
		foreach(QString dir_name,themes)
			ui.comboBox->addItem(dir_name, dir.filePath(dir_name));
	}
	int index = ui.comboBox->findData(settings.value("path","").toString());
	ui.comboBox->setCurrentIndex(index > -1 ?index:0);
	ui.accountCheckBox->setChecked(settings.value("account", true).toBool());
	ui.contactCheckBox->setChecked(settings.value("contact", true).toBool());
	return m_widget;
}

void ProtocolIcon::removeSettingsWidget()
{
	delete m_widget;
}

void ProtocolIcon::saveSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "protocolicon");
	settings.setValue("path", ui.comboBox->itemData(ui.comboBox->currentIndex()).toString());
	settings.setValue("account", ui.accountCheckBox->isChecked());
	settings.setValue("contact", ui.contactCheckBox->isChecked());
}
	
void ProtocolIcon::processEvent(Event &event)
{
	if(event.id != m_event_id)
		return;
	TreeModelItem &item = event.at<TreeModelItem>(0);
	switch(item.m_item_type)
	{
	case 0:
		if(!m_contact)
			return;
		break;
	case 2:
		if(!m_account)
			return;
		break;
	default:
		return;
	}
	QString name = item.m_protocol_name;
	if(name=="Jabber")
	{
		if(!m_icons.contains(name))
			m_icons.insert(name, Icon("jabber", IconInfo::Protocol));
		if(item.m_item_name.endsWith("@gmail.com") || item.m_item_name.indexOf("@gmail.com/")>-1)
			name="GTalk";
		else if(item.m_item_name.endsWith("@livejournal.com") || item.m_item_name.indexOf("@livejournal.com/")>-1)
			name="LiveJournal";
		else if(item.m_item_name.endsWith("@ya.ru") || item.m_item_name.indexOf("@ya.ru/")>-1)
			name=m_ya;
		if(!m_icons.contains(name))
			m_icons.insert(name,m_icons.value("Jabber"));
	}
	if(!m_icons.contains(name))
		m_icons.insert(name,Icon(name.toLower(), IconInfo::Protocol));
	m_plugin_system->setContactItemIcon(item, m_icons.value(name), item.m_item_type==0?9:0);
}

Q_EXPORT_PLUGIN2(protocolicon, ProtocolIcon);
