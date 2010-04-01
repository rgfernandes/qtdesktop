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

#ifndef MSNSTATUSICONS_H
#define MSNSTATUSICONS_H
#include <qutim/iconmanagerinterface.h>

using namespace qutim_sdk_0_2;

class MSNStatusIcons
{
public:
    MSNStatusIcons();
    static MSNStatusIcons &instance();
    QIcon m_online_icon;
    QIcon m_offline_icon;
    QIcon m_connecting_icon;
    QIcon m_busy_icon;
    QIcon m_idle_icon;
    QIcon m_rightback_icon;
    QIcon m_away_icon;
    QIcon m_onphone_icon;
    QIcon m_lunch_icon;
    QIcon m_invisible_icon;
};

#endif // MSNSTATUSICONS_H
