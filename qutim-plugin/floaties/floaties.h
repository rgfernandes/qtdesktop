/*
    Floaties

    Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef FLOATIES_H
#define FLOATIES_H

#include "contactwidget.h"
#include <QObject>

using namespace qutIM;

class FloatiesPlugin : public QObject, public Plugin, public EventHandler
{
    Q_OBJECT
    Q_INTERFACES(qutim_sdk_0_2::PluginInterface)
public:
	virtual bool init(PluginSystem *plugin_system);
    virtual void release();
    virtual void processEvent(Event &event);
    virtual void setProfileName(const QString &profile_name);
    virtual QString name();
    virtual QString description();
    virtual QString type();
    virtual QIcon *icon();
    void contextMenu(const TreeModelItem &item, const QPoint &point);
	void createChat(const TreeModelItem &item);
public slots:
	void onAction(bool checked = false);
private:
    TreeModelItem m_current_menu;
    uint m_current_hash;
    QIcon *m_icon;
    QAction *m_menu_action;
    QString m_profile_name;
    quint16 m_contact_context;
    quint16 m_item_added;
    quint16 m_item_removed;
    quint16 m_item_moved;
    quint16 m_item_icon_changed;
    quint16 m_item_changed_name;
    quint16 m_item_changed_status;
    quint16 m_item_menu;
    QHash<uint, ContactWidget *> m_floaties;
    QHash<uint, QString> m_statuses;
};

#endif // FLOATIES_H
