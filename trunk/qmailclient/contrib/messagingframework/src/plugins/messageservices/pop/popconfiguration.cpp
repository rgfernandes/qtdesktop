/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Messaging Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "popconfiguration.h"


PopConfiguration::PopConfiguration(const QMailAccountConfiguration &config)
    : QMailServiceConfiguration(config, "pop3")
{
}

PopConfiguration::PopConfiguration(const QMailAccountConfiguration::ServiceConfiguration &svcCfg)
    : QMailServiceConfiguration(svcCfg)
{
}

QString PopConfiguration::mailUserName() const
{
    return value("username");
}

QString PopConfiguration::mailPassword() const
{
    return decodeValue(value("password"));
}

QString PopConfiguration::mailServer() const
{
    return value("server");
}

int PopConfiguration::mailPort() const
{
    return value("port", "143").toInt();
}

int PopConfiguration::mailEncryption() const
{
    return value("encryption", "0").toInt();
}

bool PopConfiguration::canDeleteMail() const
{
    return (value("canDelete", "0").toInt() != 0);
}

bool PopConfiguration::isAutoDownload() const
{
    return (value("autoDownload", "0").toInt() != 0);
}

int PopConfiguration::maxMailSize() const
{
    return value("maxSize", "102400").toInt();
}

int PopConfiguration::checkInterval() const
{
    return value("checkInterval", "-1").toInt();
}

bool PopConfiguration::intervalCheckRoamingEnabled() const
{
    return (value("intervalCheckRoamingEnabled", "0").toInt() != 0);
}


PopConfigurationEditor::PopConfigurationEditor(QMailAccountConfiguration *config)
    : PopConfiguration(*config)
{
}

void PopConfigurationEditor::setMailUserName(const QString &str)
{
    setValue("username", str);
}

void PopConfigurationEditor::setMailPassword(const QString &str)
{
    setValue("password", encodeValue(str));
}

void PopConfigurationEditor::setMailServer(const QString &str)
{
    setValue("server", str);
}

void PopConfigurationEditor::setMailPort(int i)
{
    setValue("port", QString::number(i));
}

#ifndef QT_NO_OPENSSL

void PopConfigurationEditor::setMailEncryption(int t)
{
    setValue("encryption", QString::number(t));
}

#endif

void PopConfigurationEditor::setDeleteMail(bool b)
{
    setValue("canDelete", QString::number(b ? 1 : 0));
}

void PopConfigurationEditor::setAutoDownload(bool b)
{
    setValue("autoDownload", QString::number(b ? 1 : 0));
}

void PopConfigurationEditor::setMaxMailSize(int i)
{
    setValue("maxSize", QString::number(i));
}

void PopConfigurationEditor::setCheckInterval(int i)
{
    setValue("checkInterval", QString::number(i));
}

void PopConfigurationEditor::setIntervalCheckRoamingEnabled(bool b)
{
    setValue("intervalCheckRoamingEnabled", QString::number(b ? 1 : 0));
}

