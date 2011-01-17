/*
		twPluginSystem

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

#include "twpluginsystem.h"



twPluginSystem::twPluginSystem()
{
}

twPluginSystem &twPluginSystem::instance()
{
				static twPluginSystem twps;
				return twps;
}

void twPluginSystem::setPluginSystemPointer(PluginSystemInterface *ps)
{
    m_plugin_system = ps;
}

QIcon twPluginSystem::getSystemIcon(const QString &icon_name)
{
    return m_plugin_system->getIcon(icon_name);
}

void twPluginSystem::updateStatusIcons()
{
	m_plugin_system->updateStatusIcons();
}

bool twPluginSystem::removeItemFromContactList(TreeModelItem Item)
{
	return m_plugin_system->removeItemFromContactList(Item);
}

bool twPluginSystem::moveItemInContactList(TreeModelItem OldItem, TreeModelItem NewItem)
{
	return m_plugin_system->moveItemInContactList(OldItem, NewItem);
}

bool twPluginSystem::setContactItemStatus(TreeModelItem Item, QIcon icon, QString text, int mass)
{
	return m_plugin_system->setContactItemStatus(Item, icon, text, mass);
}

void twPluginSystem::sendSystemMessage(const QString &account_name, const QString &message)
{
    TreeModelItem item;
		item.m_protocol_name = "Twitter";
    item.m_account_name = account_name;
    item.m_item_name = account_name;
    item.m_item_type = 2;
    m_plugin_system->systemNotifiacation(item ,message);
}

void twPluginSystem::setAccountIsOnline(const QString &account_name, bool online)
{
    TreeModelItem item;
		item.m_protocol_name = "Twitter";
    item.m_account_name = account_name;
    item.m_item_name = account_name;
    item.m_item_type = 2;
    m_plugin_system->setAccountIsOnline(item, online);
}

bool twPluginSystem::addItemToContactList(TreeModelItem Item, QString name)
{
	return m_plugin_system->addItemToContactList(Item, name);
}

bool twPluginSystem::setContactItemIcon(TreeModelItem Item, QIcon icon, int position)
{
    return m_plugin_system->setContactItemIcon(Item, icon, position);
}

bool twPluginSystem::setContactItemRow(TreeModelItem Item, QList<QVariant> row, int position)
{
	return m_plugin_system->setContactItemRow(Item, row, position);
}

void twPluginSystem::addMessageFromContact(const TreeModelItem &item, const QString &message,
		const QDateTime &message_date)
{
	m_plugin_system->addMessageFromContact(item, message, message_date);
}
