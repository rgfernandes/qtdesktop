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

#include "floaties.h"
#include <qutim/iconmanagerinterface.h>
#include <QDebug>

#if defined(FORCE_UREF)
inline QDebug &operator<<(QDebug debug, const Event &event)
#else
inline QDebug operator<<(QDebug debug, const Event &event)
#endif
{
    debug.nospace() << "Event( " << event.id << ", " << event.args.toList() << ")";
    return debug.space();
}

#if defined(FORCE_UREF)
inline QDebug &operator<<(QDebug debug, const TreeModelItem &item)
#else
inline QDebug operator<<(QDebug debug, const TreeModelItem &item)
#endif
{
    debug.nospace() << "TreeModelItem( " << item.m_protocol_name << ", " << item.m_account_name << ", " << item.m_item_name
            << ", " << item.m_parent_name << ", " << item.m_item_history << ", " << item.m_item_type << " )";
    return debug.space();
}

 inline bool operator==(const TreeModelItem &t1, const TreeModelItem &t2)
 {
     return t1.m_protocol_name == t2.m_protocol_name
             && t1.m_account_name == t2.m_account_name
             && t1.m_item_name == t2.m_account_name;
 }

/*
    This function is based on Peter J. Weinberger's hash function
    (from the Dragon Book). The constant 24 in the original function
    was replaced with 23 to produce fewer collisions on input such as
    "a", "aa", "aaa", "aaaa", ...
*/

static void qutim_hash(const QChar *p, int n, uint &h)
{
    uint g;

    while (n--) {
        h = (h << 4) + (*p++).unicode();
        if ((g = (h & 0xf0000000)) != 0)
            h ^= g >> 23;
        h &= ~g;
    }
}

inline uint qHash(const TreeModelItem &item)
{
    uint h = 0;
    qutim_hash(item.m_protocol_name.unicode(), item.m_protocol_name.size(), h);
    qutim_hash(item.m_account_name.unicode(), item.m_account_name.size(), h);
    qutim_hash(item.m_item_name.unicode(), item.m_item_name.size(), h);
    return h;
}

#define FUNC(Func) static_cast<EventHandlerFunc>(Func)

bool FloatiesPlugin::init(PluginSystem *plugin_system)
{
	m_icon = 0;
	IconManager *icon_manager = plugin_system->getIconManager();
	qDebug() << icon_manager;
	m_plugin_system = plugin_system;
	m_menu_action = new QAction(tr("Show floaties"), this);
	m_menu_action->setCheckable(true);
	connect(m_menu_action, SIGNAL(triggered(bool)), this, SLOT(onAction(bool)));
	plugin_system->registerContactMenuAction(m_menu_action);
	m_contact_context = plugin_system->registerEventHandler("Core/ContactList/ContactContext", this);
	m_item_added = plugin_system->registerEventHandler("Core/ContactList/ItemAdded", this );
	m_item_removed = plugin_system->registerEventHandler("Core/ContactList/ItemRemoved", this);
	m_item_moved = plugin_system->registerEventHandler("Core/ContactList/ItemMoved", this);
	m_item_icon_changed = plugin_system->registerEventHandler("Core/ContactList/ItemIconChanged", this);
	m_item_changed_name = plugin_system->registerEventHandler("Core/ContactList/ItemChangedName", this);
	m_item_changed_status = plugin_system->registerEventHandler("Core/ContactList/ItemChangedStatus", this);
	m_item_menu = plugin_system->registerEventHandler("Core/ContactList/ContextMenu");
	return true;
}

void FloatiesPlugin::release()
{
    delete m_menu_action;
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "floatiessettings");
    settings.clear();
    QStringList contacts;
    QList<uint> keys = m_floaties.keys();
    foreach(uint hash, keys)
    {
        ContactWidget *contact_widget = m_floaties.take(hash);
        const TreeModelItem &item = contact_widget->getItem();
        QString hash_s = QString::number(hash);
        contacts.append(hash_s);
        settings.beginGroup(hash_s);
        settings.setValue("protocol", item.m_protocol_name);
        settings.setValue("account", item.m_account_name);
        settings.setValue("name", item.m_item_name);
        settings.setValue("position", contact_widget->pos());
        settings.setValue("opacity", contact_widget->windowOpacity());
        settings.endGroup();
    }
    qDeleteAll(m_floaties.values());
    m_floaties.clear();
    settings.setValue("contacts", contacts);
}

void FloatiesPlugin::processEvent(Event &event)
{
    if(event.args.size()<1)
        return;
    TreeModelItem &item = event.at<TreeModelItem>(0);
    if(item.m_item_type)
        return;
    uint hash = qHash(item);
//    qDebug() << event;
//    qDebug() << item << qHash(item);
    if(event.id == m_contact_context)
    {
        m_current_menu = item;
        m_current_hash = hash;
        m_menu_action->setChecked(m_floaties.contains(hash));
        return;
    }
    if(!m_floaties.contains(hash))
    {
        if(event.id == m_item_changed_status)
            m_statuses.insert(hash, event.at<QString>(2));
        return;
    }
    ContactWidget *contact_widget = m_floaties[hash];
    if(event.id == m_item_added)
    {
        if(contact_widget)
        {
            contact_widget->setNewItem(event.at<TreeModelItem>(0));
            contact_widget->setName(event.at<QString>(1));
			TreeModelItem item = event.at<TreeModelItem>(0);
			const TreeModelItem item2 = item;
			qutim_sdk_0_3::TreeModelItem item3 = item;
			const qutim_sdk_0_3::TreeModelItem item4 = item2;
			item = item3;
			const TreeModelItem item5 = item4;
			const qutim_sdk_0_3::TreeModelItem item6 = item4;
            return;
        }
        contact_widget = new ContactWidget(item, event.at<QString>(1), this);
        m_floaties[hash] = contact_widget;
        contact_widget->show();
    }
    else if(!contact_widget)
        return;
    else if(event.id == m_item_removed)
    {
        m_statuses.remove(hash);
        delete m_floaties.take(hash);
    }
    else if(event.id == m_item_moved)
    {
        TreeModelItem &new_item = event.at<TreeModelItem>(1);
        uint new_hash = qHash(new_item);
        m_floaties.remove(hash);
        m_floaties[new_hash] = contact_widget;
        contact_widget->setNewItem(new_item);
    }
    else if(event.id == m_item_icon_changed)
    {
        switch(event.at<int>(2))
        {
        case 0:
            contact_widget->setStatusIcon(event.at<QIcon>(1));
            break;
        case 1:
            contact_widget->setAvatar(event.at<QIcon>(1));
            break;
        default:
            return;
        }
    }
    else if(event.id == m_item_changed_name)
    {
        contact_widget->setName(event.at<QString>(1));
    }
    else if(event.id == m_item_changed_status)
    {
        contact_widget->setStatusIcon(event.at<QIcon>(1));
    }
}

void FloatiesPlugin::setProfileName(const QString &profile_name)
{
    m_profile_name = profile_name;
    m_menu_action->setText(tr("Show floaties"));
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "floatiessettings");
    QStringList keys = settings.value("contacts").toStringList();
    foreach(const QString &key, keys)
    {
        TreeModelItem item;
        settings.beginGroup(key);
        if(settings.contains("protocol") && settings.contains("account")
            && settings.contains("name"))
        {
            item.m_protocol_name = settings.value("protocol").toString();
            item.m_account_name = settings.value("account").toString();
            item.m_item_name = settings.value("name").toString();
            ContactWidget *contact_widget = new ContactWidget(item, "", this);
            contact_widget->move(settings.value("position", QPoint(0,0)).toPoint());
            bool ok = true;
            double opacity = settings.value("opacity", 0.8).toDouble(&ok);
            contact_widget->setWindowOpacity(ok?opacity:0.8);
            m_floaties[qHash(item)] = contact_widget;
            contact_widget->show();
        }
        settings.endGroup();
    }
}

QString FloatiesPlugin::name()
{
    return "Floaties";
}

QString FloatiesPlugin::description()
{
    return "Implemenation of floaty contacts";
}

QString FloatiesPlugin::type()
{
    return "simple";
}

QIcon *FloatiesPlugin::icon()
{
    return m_icon;
}

void FloatiesPlugin::onAction(bool checked)
{
    if(checked)
    {
        ContactWidget *contact_widget = new ContactWidget(m_current_menu, "", this);
        contact_widget->setStatusIcon(m_plugin_system->getStatusIcon(m_statuses.value(m_current_hash,"offline"),m_current_menu.m_protocol_name.toLower()));
        QStringList info = m_plugin_system->getAdditionalInfoAboutContact(m_current_menu);
        if(info.size()>0)
            contact_widget->setName(info[0]);
        if(info.size()>1)
        {
            QIcon ava(info[1]);
            contact_widget->setAvatar(ava);
        }
        m_floaties[m_current_hash] = contact_widget;
        contact_widget->show();
    }
    else
    {
        delete m_floaties.take(m_current_hash);
    }
}

void FloatiesPlugin::contextMenu(const TreeModelItem &item, const QPoint &point)
{
    Event event(m_item_menu, 2, &item, &point);
    m_plugin_system->sendEvent(event);
}

void FloatiesPlugin::createChat(const TreeModelItem &item)
{
    m_plugin_system->createChat(item);
}


Q_EXPORT_PLUGIN2(floaties, FloatiesPlugin);
