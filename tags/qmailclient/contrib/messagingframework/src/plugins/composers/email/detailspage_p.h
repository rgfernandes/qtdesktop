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

#ifndef DETAILSPAGE_P_H
#define DETAILSPAGE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Extended API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qwidget.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qmailmessage.h>

class QLineEdit;
class QHBoxLayout;
class QComboBox;
class QToolButton;
class QLabel;
class QCheckBox;
class QMailAccount;
class QMailAccountId;

class PLUGIN_EXPORT DetailsPage : public QWidget
{
    Q_OBJECT

public:

    DetailsPage( QWidget *parent, const char *name = 0 );

    void setDefaultAccount( const QMailAccountId& defaultId );

    void setTo( const QString &a_to );
    QString to() const;

    void setBcc( const QString &a_bcc );
    QString bcc() const;

    void setCc( const QString &a_cc );
    QString cc() const;

    QString subject() const;
    void setSubject( const QString &sub );

    QString from() const;
    void setFrom( const QString &from );
    void setFrom( const QMailAccountId &id );

    QMailAccount fromAccount() const;

    void setType( int t );

    void setDetails( const QMailMessage &mail );
    void getDetails( QMailMessage &mail ) const;

    bool isDetailsOnlyMode() const;
    void setDetailsOnlyMode(bool val);

public slots:
    void clear();

signals:
    void changed();
    void sendMessage();
    void cancel();
    void editMessage();

private slots:
    void editRecipients();
    void copy();
    void paste();

private:
    bool m_allowPhoneNumbers, m_allowEmails;
    bool m_ignoreFocus;
    int m_type;
    QCheckBox *m_readReplyField;
    QCheckBox *m_deliveryReportField;
    QLineEdit *m_subjectField;
    QLabel *m_subjectFieldLabel, *m_fromFieldLabel;
    QComboBox *m_fromField;
    QLabel *m_toFieldLabel, *m_ccFieldLabel, *m_bccFieldLabel;
    QLineEdit *m_ccField, *m_bccField, *m_toField;
    QHBoxLayout *m_toBox, *m_ccBox, *m_bccBox;
    QAction *m_previousAction;
};

#endif
