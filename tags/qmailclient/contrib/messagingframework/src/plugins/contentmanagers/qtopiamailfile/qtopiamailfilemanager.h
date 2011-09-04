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

#ifndef QTOPIAMAILFILEMANAGER_H
#define QTOPIAMAILFILEMANAGER_H

#include <qmailid.h>
#include <qmailcontentmanager.h>
#include <QList>
#include <QMap>


class QMailMessagePart;
class QMailMessagePartContainer;
class QFile;

class QtopiamailfileManager : public QObject, public QMailContentManager
{
    Q_OBJECT

public:
    QtopiamailfileManager();
    ~QtopiamailfileManager();

    QMailStore::ErrorCode add(QMailMessage *message, QMailContentManager::DurabilityRequirement durability);
    QMailStore::ErrorCode update(QMailMessage *message, QMailContentManager::DurabilityRequirement durability);

    QMailStore::ErrorCode ensureDurability();

    QMailStore::ErrorCode remove(const QString &identifier);
    QMailStore::ErrorCode load(const QString &identifier, QMailMessage *message);

    bool init();
    void clearContent();

    static const QString &messagesBodyPath(const QMailAccountId &accountId);
    static QString messageFilePath(const QString &fileName, const QMailAccountId &accountId);
    static QString messagePartDirectory(const QString &fileName);
    static QString messagePartFilePath(const QMailMessagePart &part, const QString &fileName);

protected slots:
    void clearAccountPath(const QMailAccountIdList&);

private:
    QMailStore::ErrorCode addOrRename(QMailMessage *message, const QString &existingIdentifier, bool durable);

    bool addOrRenameParts(QMailMessage *message, const QString &fileName, const QString &existing, bool durable);
    bool removeParts(const QString &fileName);

    QList<QFile*> _openFiles;
};


class QtopiamailfileManagerPlugin : public QMailContentManagerPlugin
{
    Q_OBJECT

public:
    QtopiamailfileManagerPlugin();

    virtual QString key() const;
    QMailContentManager *create();
};

#endif

