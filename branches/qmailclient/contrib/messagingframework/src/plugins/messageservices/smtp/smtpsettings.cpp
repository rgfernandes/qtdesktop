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

#include "smtpsettings.h"
#include "smtpconfiguration.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QValidator>
#include <qmailaccount.h>
#include <qmailaccountconfiguration.h>
#include <qmailtransport.h>
#include <QDialog>

namespace {

const QString serviceKey("smtp");

class SigEntry : public QDialog
{
    Q_OBJECT

public:
    SigEntry(QWidget *parent, const char* name, Qt::WFlags fl = 0);

    void setEntry(QString sig);
    QString entry() const;

protected:
    void closeEvent(QCloseEvent *event);

private:
    QTextEdit *input;
};

SigEntry::SigEntry(QWidget *parent, const char *name, Qt::WFlags fl)
    : QDialog(parent, fl)
{
    setObjectName(name);
    setWindowTitle(tr("Signature"));

    QGridLayout *grid = new QGridLayout(this);
    input = new QTextEdit(this);
    grid->addWidget(input, 0, 0);
}

void SigEntry::setEntry(QString sig)
{
    input->insertPlainText(sig);
}

QString SigEntry::entry() const
{
    return input->toPlainText();
}

void SigEntry::closeEvent(QCloseEvent *)
{
    accept();
}


class PortValidator : public QValidator
{
public:
    PortValidator(QWidget *parent = 0, const char *name = 0);

    QValidator::State validate(QString &str, int &) const;
};

PortValidator::PortValidator(QWidget *parent, const char *name)
    : QValidator(parent)
{
    setObjectName(name);
}

QValidator::State PortValidator::validate(QString &str, int &) const
{
    // allow empty strings, as it's a bit awkward to edit otherwise
    if ( str.isEmpty() )
        return QValidator::Acceptable;

    bool ok = false;
    int i = str.toInt(&ok);
    if ( !ok )
        return QValidator::Invalid;

    if ( i <= 0 || i >= 65536 )
        return QValidator::Invalid;

    return QValidator::Acceptable;
}


const SmtpConfiguration::AuthType authenticationType[] = {
    SmtpConfiguration::Auth_NONE,
#ifndef QT_NO_OPENSSL
    SmtpConfiguration::Auth_LOGIN,
    SmtpConfiguration::Auth_PLAIN,
#endif
    SmtpConfiguration::Auth_INCOMING
};

#ifndef QT_NO_OPENSSL
int authenticationIndex(int type)
{
    const int numTypes = sizeof(authenticationType)/sizeof(SmtpConfiguration::AuthType);
    for (int i = 0; i < numTypes; ++i)
        if (type == authenticationType[i])
            return i;

    return 0;
}
#endif

}


SmtpSettings::SmtpSettings()
    : QMailMessageServiceEditor(),
      addressModified(false)
{
    setupUi(this);
    setLayoutDirection(qApp->layoutDirection());

    connect(setSignatureButton, SIGNAL(clicked()), this, SLOT(sigPressed()));
    connect(authentication, SIGNAL(currentIndexChanged(int)), this, SLOT(authChanged(int)));
    connect(emailInput, SIGNAL(textChanged(QString)), this, SLOT(emailModified()));
    connect(sigCheckBox,SIGNAL(clicked(bool)),setSignatureButton,SLOT(setEnabled(bool)));

    const QString uncapitalised("email noautocapitalization");

    // These fields should not be autocapitalised

    smtpPortInput->setValidator(new PortValidator(this));

    smtpPasswordInput->setEchoMode(QLineEdit::PasswordEchoOnEdit);

#ifdef QT_NO_OPENSSL
    encryption->hide();
    lblEncryption->hide();
    authentication->hide();
    lblAuthentication->hide();
    smtpUsernameInput->hide();
    lblSmtpUsername->hide();
    smtpPasswordInput->hide();
    lblSmtpPassword->hide();
#else
    smtpPasswordInput->setEchoMode(QLineEdit::PasswordEchoOnEdit);
#endif
}

void SmtpSettings::sigPressed()
{
    if (sigCheckBox->isChecked()) {
        QString sigText;
        if (signature.isEmpty())
            sigText = QLatin1String("~~\n") + nameInput->text();
        else
            sigText = signature;

        SigEntry sigEntry(this, "sigEntry", static_cast<Qt::WFlags>(1));
        sigEntry.setEntry(sigText);
        if (sigEntry.exec() == QDialog::Accepted)
            signature = sigEntry.entry();
    }
}

void SmtpSettings::emailModified()
{
    addressModified = true;
}

void SmtpSettings::authChanged(int index)
{
#ifndef QT_NO_OPENSSL
    SmtpConfiguration::AuthType type = authenticationType[index];
    bool enableCredentials = (type == SmtpConfiguration::Auth_LOGIN || type == SmtpConfiguration::Auth_PLAIN);

    smtpUsernameInput->setEnabled(enableCredentials);
    lblSmtpUsername->setEnabled(enableCredentials);
    smtpPasswordInput->setEnabled(enableCredentials);
    lblSmtpPassword->setEnabled(enableCredentials);

    if (!enableCredentials) {
        smtpUsernameInput->clear();
        smtpPasswordInput->clear();
    }
#else
    Q_UNUSED(index);
#endif
}

void SmtpSettings::displayConfiguration(const QMailAccount &account, const QMailAccountConfiguration &config)
{
#ifndef QT_NO_OPENSSL
    // Any reason to re-enable this facility?
    //authentication->setItemText(3, tr("Incoming");
#endif

    if (!config.services().contains(serviceKey)) {
        // New account
        emailInput->setText("");
        smtpServerInput->setText("");
        smtpPortInput->setText("25");
#ifndef QT_NO_OPENSSL
        smtpUsernameInput->setText("");
        smtpPasswordInput->setText("");
        encryption->setCurrentIndex(0);
        authentication->setCurrentIndex(0);
        smtpUsernameInput->setEnabled(false);
        lblSmtpUsername->setEnabled(false);
        smtpPasswordInput->setEnabled(false);
        lblSmtpPassword->setEnabled(false);
#endif
        signature = QString();
    } else {
        SmtpConfiguration smtpConfig(config);

        emailInput->setText(smtpConfig.emailAddress());
        smtpServerInput->setText(smtpConfig.smtpServer());
        smtpPortInput->setText(QString::number(smtpConfig.smtpPort()));
#ifndef QT_NO_OPENSSL
        smtpUsernameInput->setText(smtpConfig.smtpUsername());
        smtpPasswordInput->setText(smtpConfig.smtpPassword());
        authentication->setCurrentIndex(authenticationIndex(smtpConfig.smtpAuthentication()));
        encryption->setCurrentIndex(static_cast<int>(smtpConfig.smtpEncryption()));

        SmtpConfiguration::AuthType type = authenticationType[authentication->currentIndex()];
        const bool enableCredentials(type == SmtpConfiguration::Auth_LOGIN || type == SmtpConfiguration::Auth_PLAIN);
        smtpUsernameInput->setEnabled(enableCredentials);
        lblSmtpUsername->setEnabled(enableCredentials);
        smtpPasswordInput->setEnabled(enableCredentials);
        lblSmtpPassword->setEnabled(enableCredentials);
#endif
        defaultMailCheckBox->setChecked(account.status() & QMailAccount::PreferredSender);
        sigCheckBox->setChecked(account.status() & QMailAccount::AppendSignature);
        setSignatureButton->setEnabled(sigCheckBox->isChecked());
        signature = account.signature();
    }
}

bool SmtpSettings::updateAccount(QMailAccount *account, QMailAccountConfiguration *config)
{
    QString username(nameInput->text());
    QString address(emailInput->text());

    if (!username.isEmpty() || !address.isEmpty()) {
        account->setFromAddress(QMailAddress(username, address)); 
    }

    bool result;
    int port = smtpPortInput->text().toInt(&result);
    if ( (!result) ) {
        // should only happen when the string is empty, since we use a validator.
        port = 25;
    }

    if (!config->services().contains(serviceKey))
        config->addServiceConfiguration(serviceKey);

    SmtpConfigurationEditor smtpConfig(config);

    smtpConfig.setVersion(100);
    smtpConfig.setType(QMailServiceConfiguration::Sink);

    if ((!addressModified) && (address.isEmpty())) {
        // Try to guess email address
        QString server(smtpConfig.smtpServer());
        if (server.count('.')) {
            address = username + "@" + server.mid(server.indexOf('.') + 1, server.length());
        } else if (server.count('.') == 1) {
            address = username + "@" + server;
        }
    }
    smtpConfig.setUserName(username);
    smtpConfig.setEmailAddress(address);
    smtpConfig.setSmtpServer(smtpServerInput->text());
    smtpConfig.setSmtpPort(port);
#ifndef QT_NO_OPENSSL
    smtpConfig.setSmtpUsername(smtpUsernameInput->text());
    smtpConfig.setSmtpPassword(smtpPasswordInput->text());
    smtpConfig.setSmtpAuthentication(authenticationType[authentication->currentIndex()]);
    smtpConfig.setSmtpEncryption(static_cast<QMailTransport::EncryptType>(encryption->currentIndex()));
#endif

    account->setStatus(QMailAccount::PreferredSender, defaultMailCheckBox->isChecked());
    account->setStatus(QMailAccount::AppendSignature, sigCheckBox->isChecked());
    account->setSignature(signature);

    // Do we have a configuration we can use?
    if (!smtpConfig.smtpServer().isEmpty() && !smtpConfig.emailAddress().isEmpty())
        account->setStatus(QMailAccount::CanTransmit, true);

    return true;
}


#include "smtpsettings.moc"

