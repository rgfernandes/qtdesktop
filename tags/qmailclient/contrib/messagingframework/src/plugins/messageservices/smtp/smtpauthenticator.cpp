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

#include "smtpauthenticator.h"

#include "smtpconfiguration.h"

#include <qmailauthenticator.h>


namespace {

QMap<QMailAccountId, QList<QByteArray> > gResponses;

}

QByteArray SmtpAuthenticator::getAuthentication(const QMailAccountConfiguration::ServiceConfiguration &svcCfg, const QStringList &capabilities)
{
    QByteArray result(QMailAuthenticator::getAuthentication(svcCfg, capabilities));
    if (!result.isEmpty())
        return result;

#ifndef QT_NO_OPENSSL
    SmtpConfiguration smtpCfg(svcCfg);
    if (smtpCfg.smtpAuthentication() != SmtpConfiguration::Auth_NONE) {
        QMailAccountId id(smtpCfg.id());
        QByteArray username(smtpCfg.smtpUsername().toAscii());
        QByteArray password(smtpCfg.smtpPassword().toAscii());

        if (smtpCfg.smtpAuthentication() == SmtpConfiguration::Auth_LOGIN) {
            result = QByteArray("LOGIN");
            gResponses[id] = (QList<QByteArray>() << username << password);
        } else if (smtpCfg.smtpAuthentication() == SmtpConfiguration::Auth_PLAIN) {
            result = QByteArray("PLAIN");
            gResponses[id] = (QList<QByteArray>() << QByteArray(username + '\0' + username + '\0' + password));
        }
    }
#endif

    if (!result.isEmpty()) {
        result.prepend("AUTH ");
    }
    return result;
}

QByteArray SmtpAuthenticator::getResponse(const QMailAccountConfiguration::ServiceConfiguration &svcCfg, const QByteArray &challenge)
{
    QByteArray result;

    QMap<QMailAccountId, QList<QByteArray> >::iterator it = gResponses.find(svcCfg.id());
    if (it != gResponses.end()) {
        QList<QByteArray> &responses = it.value();
        result = responses.takeFirst();

        if (responses.isEmpty())
            gResponses.erase(it);
    } else {
        result = QMailAuthenticator::getResponse(svcCfg, challenge);
    }

    return result;
}

