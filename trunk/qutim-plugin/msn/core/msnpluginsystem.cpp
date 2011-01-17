/*
    MSNPluginSystem

    Copyright (c) 2009 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "msnpluginsystem.h"

MSNPluginSystem::MSNPluginSystem()
{
}

MSNPluginSystem &MSNPluginSystem::instance()
{
	static MSNPluginSystem mps;
	return mps;
}

void MSNPluginSystem::setPluginSystemPointer(PluginSystemInterface *ps)
{
    m_plugin_system = ps;
}

void MSNPluginSystem::updateStatusIcons()
{
    m_plugin_system->updateStatusIcons();
}

bool MSNPluginSystem::setStatusMessage(QString &status_message, bool &dshow)
{
    return m_plugin_system->setStatusMessage(status_message, dshow);
}

bool MSNPluginSystem::addItemToContactList(const TreeModelItem &item, const QString &name)
{
    return m_plugin_system->addItemToContactList(item, name);
}

bool MSNPluginSystem::removeItemFromContactList(const TreeModelItem &item)
{
    return m_plugin_system->removeItemFromContactList(item);
}

bool MSNPluginSystem::setContactItemName(const TreeModelItem &item, const QString &name)
{
    return m_plugin_system->setContactItemName(item, name);
}

bool MSNPluginSystem::setContactItemStatus(const TreeModelItem &item, QIcon icon, const QString &text, int mass)
{
    return m_plugin_system->setContactItemStatus(item, icon, text, mass);
}

bool MSNPluginSystem::moveItemInContactList(const TreeModelItem &OldItem, const TreeModelItem &NewItem)
{
    return m_plugin_system->moveItemInContactList(OldItem, NewItem);
}

void MSNPluginSystem::setAccountIsOnline(const TreeModelItem &item, bool online)
{
    m_plugin_system->setAccountIsOnline(item, online);
}

void MSNPluginSystem::addMessageFromContact(const TreeModelItem &item, const QString &message,
		const QDateTime &message_date)
{
	m_plugin_system->addMessageFromContact(item, message, message_date);
}
