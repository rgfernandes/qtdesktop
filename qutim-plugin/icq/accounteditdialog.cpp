/*
    AccountEditDialog

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

#include "accounteditdialog.h"
#include "icqpluginsystem.h"
#include <QSettings>

AccountEditDialog::AccountEditDialog(const QString &uin, const QString &profile_name,
                                     contactListTree *cl, QWidget *parent) :
    QWidget(parent),
    m_uin(uin),
    m_profile_name(profile_name),
    m_cl(cl)
{
    m_ui.setupUi(this);
    move(desktopCenter());
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);

    IcqPluginSystem &ips = IcqPluginSystem::instance();
    m_ui.okButton->setIcon(ips.getIcon("apply"));
    m_ui.applyButton->setIcon(ips.getIcon("apply"));
    m_ui.cancelButton->setIcon(ips.getIcon("cancel"));
    m_ui.tabWidget->setTabIcon(0, QIcon(":/icons/icqprotocol.png"));
    m_ui.tabWidget->setTabIcon(1, ips.getIcon("network"));
    m_ui.tabWidget->setTabIcon(2, ips.getIcon("proxy"));
    setWindowTitle(tr("Editing %1").arg(m_uin));
    setWindowIcon(ips.getIcon("edituser"));
    connect( m_ui.typeBox, SIGNAL(currentIndexChanged(int)),
                                this, SLOT(proxyTypeChanged(int)));
    loadSettings();
}

AccountEditDialog::~AccountEditDialog()
{

}

void AccountEditDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

QPoint AccountEditDialog::desktopCenter()
{
		QDesktopWidget &desktop = *QApplication::desktop();
        return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void AccountEditDialog::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+m_uin, "accountsettings");
    const char crypter[] = {0x10,0x67, 0x56, 0x78, 0x85, 0x14, 0x87, 0x11, 0x45,0x45,0x45,0x45,0x45,0x45 };
    QByteArray tmpPass = settings.value("main/password").toByteArray();
    QByteArray roastedPass;
    for ( int i = 0; i < tmpPass.length(); i++ )
        roastedPass[i] = tmpPass.at(i) ^ crypter[i];
    QString password = m_cl->convertPassToCodePage(roastedPass);
    m_ui.passwordEdit->setText(password);

    m_ui.savePassBox->setChecked( settings.value("main/savepass", false).toBool());

	 bool autoC = settings.value("connection/auto", true).toBool();
	m_ui.autoBox->setChecked(autoC);
	if ( autoC )
		m_ui.saveStatusOnExitBox->setChecked(settings.value("connection/statonexit", true).toBool());

    m_ui.idEdit->setText(settings.value("AOL/id", "ICQ Client").toString());
    m_ui.majorBox->setValue(settings.value("AOL/major", 20).toUInt());
    m_ui.minorBox->setValue(settings.value("AOL/minor", 52).toUInt());
    m_ui.lesserBox->setValue(settings.value("AOL/lesser", 1).toUInt());

    m_ui.buildBox->setValue(settings.value("AOL/build", 3916).toUInt());
    m_ui.idBox->setValue(settings.value("AOL/cid", 266).toUInt());
    m_ui.distributionBox->setValue(settings.value("AOL/distr", 85).toUInt());
    m_ui.seqBox->setValue(settings.value("AOL/seq", 0).toUInt());

    m_ui.secureBox->setChecked(settings.value("connection/md5", true).toBool());
    m_ui.hostEdit->setText(settings.value("connection/host", "login.icq.com").toString());
    m_ui.portBox->setValue(settings.value("connection/port", 5190).toInt());
    m_ui.typeBox->setCurrentIndex(settings.value("proxy/proxyType", 0).toInt());
    m_ui.proxyHostEdit->setText(settings.value("proxy/host", "").toString());
    m_ui.proxyPortBox->setValue(settings.value("proxy/port", 1).toInt());
    m_ui.authBox->setChecked(settings.value("proxy/auth", false).toBool());
        if ( m_ui.authBox->isChecked() )
        {
                m_ui.userNameEdit->setEnabled(true);
                m_ui.userPasswordEdit->setEnabled(true);
        }
        m_ui.userNameEdit->setText(settings.value("proxy/user", "").toString());
        m_ui.userPasswordEdit->setText(settings.value("proxy/pass", "").toString());
        m_ui.keepAliveBox->setChecked(settings.value("connection/alive", true).toBool());
        m_ui.useProxyBox->setChecked(settings.value("connection/useproxy", false).toBool());
        m_ui.listenPortBox->setValue(settings.value("connection/listen", 5191).toUInt());
}

void AccountEditDialog::on_okButton_clicked()
{
    saveSettings();
    close();
}

void AccountEditDialog::on_applyButton_clicked()
{
    saveSettings();
}

void AccountEditDialog::on_cancelButton_clicked()
{
    close();
}

void AccountEditDialog::saveSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+m_uin, "accountsettings");
    QString account_password = m_ui.passwordEdit->text();
    account_password.truncate(8);
    const char crypter[] = {0x10,0x67, 0x56, 0x78, 0x85, 0x14, 0x87, 0x11, 0x45,0x45,0x45,0x45,0x45,0x45 };
    QByteArray roasted_pass;
    for ( int i = 0; i < account_password.length(); i++ )
        roasted_pass[i] = account_password.at(i).unicode() ^ crypter[i];

    settings.setValue("main/password",roasted_pass);
    settings.setValue("main/savepass", m_ui.savePassBox->isChecked());

	 bool autoC = m_ui.autoBox->isChecked();
	settings.setValue("connection/auto", autoC);
	if ( autoC )
		settings.setValue("connection/statonexit", m_ui.saveStatusOnExitBox->isChecked());
	else
		settings.remove("connection/statonexit");

    settings.setValue("AOL/id",m_ui.idEdit->text());
    settings.setValue("AOL/major", m_ui.majorBox->value());
    settings.setValue("AOL/minor", m_ui.minorBox->value());
    settings.setValue("AOL/lesser", m_ui.lesserBox->value());
    settings.setValue("AOL/build", m_ui.buildBox->value());
    settings.setValue("AOL/cid", m_ui.idBox->value());
    settings.setValue("AOL/distr", m_ui.distributionBox->value());
    settings.setValue("AOL/seq", m_ui.seqBox->value());

        settings.setValue("connection/md5", m_ui.secureBox->isChecked());
        if ( m_ui.hostEdit->text().trimmed() != "" )
                                settings.setValue("connection/host", m_ui.hostEdit->text());
        else
                settings.remove("connection/host");
        settings.setValue("connection/port", m_ui.portBox->value());
        settings.setValue("proxy/proxyType", m_ui.typeBox->currentIndex());

        if ( m_ui.proxyHostEdit->isEnabled() && m_ui.proxyHostEdit->text().trimmed() != "" )
                settings.setValue("proxy/host", m_ui.proxyHostEdit->text());
        else
                settings.remove("proxy/host");

        if ( m_ui.proxyPortBox->isEnabled() )
                settings.setValue("proxy/port", m_ui.proxyPortBox->value());
        else
                settings.remove("proxy/port");
        if ( m_ui.authBox->isChecked() )
        {
                settings.setValue("proxy/auth", true);
                settings.setValue("proxy/user", m_ui.userNameEdit->text());
                settings.setValue("proxy/pass", m_ui.userPasswordEdit->text());
        } else {
                settings.remove("proxy/auth");
                settings.remove("proxy/user");
                settings.remove("proxy/pass");
        }

        settings.setValue("connection/alive", m_ui.keepAliveBox->isChecked());
        settings.setValue("connection/useproxy", m_ui.useProxyBox->isChecked());
        settings.setValue("connection/listen", m_ui.listenPortBox->value());

}


void AccountEditDialog::proxyTypeChanged(int index)
{
        if ( index )
        {
                m_ui.proxyHostEdit->setEnabled(true);
                m_ui.proxyPortBox->setEnabled(true);
                m_ui.authBox->setEnabled(true);
        } else {
                m_ui.proxyHostEdit->setEnabled(false);
                m_ui.proxyPortBox->setEnabled(false);
                m_ui.authBox->setEnabled(false);
                m_ui.authBox->setChecked(false);
        }
}
