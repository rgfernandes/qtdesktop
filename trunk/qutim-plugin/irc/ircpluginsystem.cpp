/*
    ircPluginSystem.cpp

    Copyright (c) 2008 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "ircpluginsystem.h"

ircPluginSystem &ircPluginSystem::instance()
{
	static ircPluginSystem ips;
	return ips;
}

void ircPluginSystem::setIrcLayer(ircLayer *irc_layer)
{
	m_parent_layer = irc_layer;
}

void ircPluginSystem::updateStatusIcons()
{
	m_parent_layer->getMainPluginSystemPointer()->updateStatusIcons();	
}

bool ircPluginSystem::addItemToContactList(TreeModelItem Item, QString name)
{
	return false;
}

bool ircPluginSystem::removeItemFromContactList(TreeModelItem Item)
{
	return m_parent_layer->getMainPluginSystemPointer()->removeItemFromContactList(Item);
}

bool ircPluginSystem::moveItemInContactList(TreeModelItem OldItem, TreeModelItem NewItem)
{
	return m_parent_layer->getMainPluginSystemPointer()->moveItemInContactList(OldItem, NewItem);
}

bool ircPluginSystem::setContactItemName(TreeModelItem Item, QString name)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemName(Item, name);
}

bool ircPluginSystem::setContactItemIcon(TreeModelItem Item, QIcon icon, int position)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemIcon(Item, icon, position);
}

bool ircPluginSystem::setContactItemRow(TreeModelItem Item, QList<QVariant> row, int position)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemRow(Item, row, position);
}

bool ircPluginSystem::setContactItemStatus(TreeModelItem Item, QIcon icon, QString text, int mass)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemStatus(Item, icon, text, mass);
}

bool ircPluginSystem::setStatusMessage(QString &status_message, bool &dshow)
{
	return m_parent_layer->getMainPluginSystemPointer()->setStatusMessage(status_message, dshow);
}

void ircPluginSystem::addMessageFromContact(const TreeModelItem &item, const QString &message, const QDateTime &message_date)
{
	m_parent_layer->getMainPluginSystemPointer()->addMessageFromContact(item, message, message_date);
}

void ircPluginSystem::addServiceMessage(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->addServiceMessage(item, message);
}

void ircPluginSystem::addImage(const TreeModelItem &item, const QByteArray &image_raw)
{
	m_parent_layer->getMainPluginSystemPointer()->addImage(item, image_raw);
}

void ircPluginSystem::contactTyping(const TreeModelItem &item, bool typing)
{
	m_parent_layer->getMainPluginSystemPointer()->contactTyping(item, typing);
}

void ircPluginSystem::messageDelievered(const TreeModelItem &item, int position)
{
	m_parent_layer->getMainPluginSystemPointer()->messageDelievered(item, position);
}

bool ircPluginSystem::checkForMessageValidation(const TreeModelItem &item, const QString &message, int message_type, bool special_status)
{
	return m_parent_layer->getMainPluginSystemPointer()->checkForMessageValidation(item, message, message_type, special_status);
}

QString ircPluginSystem::getIconFileName(const QString &icon_name)
{
	return m_parent_layer->getMainPluginSystemPointer()->getIconFileName(icon_name);
}

QIcon ircPluginSystem::getIcon(const QString &icon_name)
{
	return m_parent_layer->getMainPluginSystemPointer()->getIcon(icon_name);
}

QString ircPluginSystem::getStatusIconFileName(const QString &icon_name, const QString &default_path)
{
	return m_parent_layer->getMainPluginSystemPointer()->getStatusIconFileName(icon_name, default_path);
}

QIcon ircPluginSystem::getStatusIcon(const QString & icon_name, const QString & default_path)
{
	return m_parent_layer->getMainPluginSystemPointer()->getStatusIcon(icon_name, default_path);	
}

void ircPluginSystem::setAccountIsOnline(const TreeModelItem &Item, bool online)
{
	m_parent_layer->getMainPluginSystemPointer()->setAccountIsOnline(Item, online);	
}

void ircPluginSystem::createChat(const TreeModelItem &item)
{
	m_parent_layer->getMainPluginSystemPointer()->createChat(item);	
}

void ircPluginSystem::notifyAboutBirthDay(const TreeModelItem &item)
{
	m_parent_layer->getMainPluginSystemPointer()->notifyAboutBirthDay(item);	
}

void ircPluginSystem::systemNotifiacation(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->systemNotifiacation(item, message);	
}

void ircPluginSystem::customNotifiacation(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->customNotifiacation(item, message);	
}

void ircPluginSystem::getQutimVersion(quint8 &major, quint8 &minor, quint8 &secminor, quint16 &svn)
{
	m_parent_layer->getMainPluginSystemPointer()->getQutimVersion(major, minor, secminor, svn);	
}

void ircPluginSystem::createConference(const QString &protocol_name, const QString &conference_name, const QString &account_name)
{
	m_parent_layer->getMainPluginSystemPointer()->createConference(protocol_name, conference_name, account_name);	
}

void ircPluginSystem::addMessageToConference(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &from, const QString &message, const QDateTime &date, bool history)
{
	m_parent_layer->getMainPluginSystemPointer()->addMessageToConference(protocol_name, conference_name, account_name, from, message, date, history);	
}

void ircPluginSystem::changeOwnConferenceNickName(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
	m_parent_layer->getMainPluginSystemPointer()->changeOwnConferenceNickName(protocol_name, conference_name, account_name, nickname);	
}

void ircPluginSystem::addConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
	m_parent_layer->getMainPluginSystemPointer()->addConferenceItem(protocol_name, conference_name, account_name, nickname);	
}

void ircPluginSystem::removeConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
	m_parent_layer->getMainPluginSystemPointer()->removeConferenceItem(protocol_name, conference_name, account_name, nickname);	
}

void ircPluginSystem::renameConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QString &new_nickname)
{
	m_parent_layer->getMainPluginSystemPointer()->renameConferenceItem(protocol_name, conference_name, account_name, nickname, new_nickname);	
}

void ircPluginSystem::setConferenceItemStatus(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &status, int mass)
{
	m_parent_layer->getMainPluginSystemPointer()->setConferenceItemStatus(protocol_name, conference_name, account_name, nickname, icon, status, mass);	
}

void ircPluginSystem::setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, int position)
{
	m_parent_layer->getMainPluginSystemPointer()->setConferenceItemIcon(protocol_name, conference_name, account_name, nickname, icon, position);	
}

void ircPluginSystem::setConferenceItemRole(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &role, int mass)
{
	m_parent_layer->getMainPluginSystemPointer()->setConferenceItemRole(protocol_name, conference_name, account_name, nickname, icon, role, mass);	
}

void ircPluginSystem::setItemVisible(const TreeModelItem &item, bool visible)
{
	m_parent_layer->getMainPluginSystemPointer()->setItemVisible(item, visible);	
}

void ircPluginSystem::setItemInvisible(const TreeModelItem &item, bool invisible)
{
	m_parent_layer->getMainPluginSystemPointer()->setItemInvisible(item, invisible);	
}

void ircPluginSystem::setConferenceTopic(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &topic)
{
	m_parent_layer->getMainPluginSystemPointer()->setConferenceTopic(protocol_name, conference_name, account_name, topic);
}

void ircPluginSystem::addSystemMessageToConference(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &message, const QDateTime &date, bool history)
{
	m_parent_layer->getMainPluginSystemPointer()->addSystemMessageToConference(protocol_name, conference_name, account_name, message, date, history);
}

QStringList ircPluginSystem::getConferenceItemsList(const QString &protocol_name, const QString &conference_name, const QString &account_name) {
//	m_parent_layer->getMainPluginSystemPointer()->getConferenceItemsList(protocol_name, conference_name, account_name);
	return QStringList();
}

QIcon ircPluginSystem::getStatusIcon(int presence)
{
	return QIcon();
}

QString ircPluginSystem::getStatusIconName(int presence)
{
	return QString();
}
