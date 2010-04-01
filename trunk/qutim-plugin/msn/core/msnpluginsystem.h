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



#ifndef MSNPLUGINSYSTEM_H
#define MSNPLUGINSYSTEM_H

#include <qutim/protocolinterface.h>
using namespace qutim_sdk_0_2;

class MSNPluginSystem
{
public:
    MSNPluginSystem();
    static MSNPluginSystem &instance();
    void setPluginSystemPointer(PluginSystemInterface *ps);
    void updateStatusIcons();
    bool setStatusMessage(QString &status_message, bool &dshow);
    bool addItemToContactList(const TreeModelItem &item, const QString &name=QString());
    bool removeItemFromContactList(const TreeModelItem &item);
    bool setContactItemName(const TreeModelItem &item, const QString &name);
    bool setContactItemStatus(const TreeModelItem &item, QIcon icon, const QString &text, int mass);
    bool moveItemInContactList(const TreeModelItem &OldItem, const TreeModelItem &NewItem);
    void setAccountIsOnline(const TreeModelItem &item, bool online);
    void addMessageFromContact(const TreeModelItem &item, const QString &message,
		    const QDateTime &message_date);
private:
    PluginSystemInterface *m_plugin_system;
};

#endif // MSNPLUGINSYSTEM_H
