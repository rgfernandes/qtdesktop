/*
    MSNStatusIcons

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

#include "msnstatusicons.h"

MSNStatusIcons::MSNStatusIcons()
{
    m_online_icon = Icon("online", IconInfo::Status, "msn");
    if(m_online_icon.isNull()) m_online_icon = QIcon(":/icons/online.png");

    m_offline_icon = Icon("offline", IconInfo::Status, "msn");
    if(m_offline_icon.isNull()) m_offline_icon = QIcon(":/icons/offline.png");

    m_connecting_icon = Icon("connecting", IconInfo::Status, "msn");
    if(m_connecting_icon.isNull()) m_connecting_icon = QIcon(":/icons/connecting.png");

    m_busy_icon = Icon("busy", IconInfo::Status, "msn");
    if(m_busy_icon.isNull()) m_busy_icon = QIcon(":/icons/busy.png");

    m_idle_icon = Icon("idle", IconInfo::Status, "msn");
    if(m_idle_icon.isNull()) m_idle_icon = QIcon(":/icons/idle.png");

    m_rightback_icon = Icon("rightback", IconInfo::Status, "msn");
    if(m_rightback_icon.isNull()) m_rightback_icon = QIcon(":/icons/rightback.png");

    m_away_icon = Icon("away", IconInfo::Status, "msn");
    if(m_away_icon.isNull()) m_away_icon = QIcon(":/icons/away.png");

    m_onphone_icon = Icon("onphone", IconInfo::Status, "msn");
    if(m_onphone_icon.isNull()) m_onphone_icon = QIcon(":/icons/onphone.png");

    m_lunch_icon = Icon("lunch", IconInfo::Status, "msn");
    if(m_lunch_icon.isNull()) m_lunch_icon = QIcon(":/icons/lunch.png");

    m_invisible_icon = Icon("invisible", IconInfo::Status, "msn");
    if(m_invisible_icon.isNull()) m_invisible_icon = QIcon(":/icons/invisible.png");
}

MSNStatusIcons &MSNStatusIcons::instance()
{
	static MSNStatusIcons msi;
	return msi;
}
