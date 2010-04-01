/*
    Сopyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "jAccountSettings.h"
#include "jProtocol.h"

jAccountSettings::jAccountSettings(const QString &profile_name, const QString &account_name, jAccount *account, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	m_account = account;
	m_profile_name = profile_name;
	m_account_name = account_name;
	loadSettings();
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(ui.paswdEdit, SIGNAL(textChanged(QString)), this, SLOT(stateChange()));
	connect(ui.jidEdit, SIGNAL(textChanged(QString)), this, SLOT(stateChange()));
	connect(ui.autologinCheck, SIGNAL(stateChanged(int)), this, SLOT(stateChange()));
	connect(ui.previousStatusCheck, SIGNAL(stateChanged(int)), this, SLOT(stateChange()));
	connect(ui.localBookmarkCheck, SIGNAL(stateChanged(int)), this, SLOT(stateChange()));
	connect(ui.resourceEdit, SIGNAL(textChanged(QString)), this, SLOT(stateChange()));
	connect(ui.prioritySpinBox, SIGNAL(valueChanged(int)), this, SLOT(stateChange()));
	connect(ui.priorityCheck, SIGNAL(stateChanged(int)), this, SLOT(stateChange()));
	connect(ui.tlsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(stateChange()));
	connect(ui.compressCheckBox, SIGNAL(stateChanged(int)), this, SLOT(stateChange()));
	connect(ui.manualCheckBox, SIGNAL(stateChanged(int)), this, SLOT(stateChange()));
	connect(ui.hostEdit, SIGNAL(textChanged(QString)), this, SLOT(stateChange()));
	connect(ui.portSpinBox, SIGNAL(valueChanged(int)), this, SLOT(stateChange()));
	connect(ui.hostEdit, SIGNAL(textChanged(QString)), this, SLOT(stateChange()));
	connect(ui.portSpinBox, SIGNAL(valueChanged(int)), this, SLOT(stateChange()));
	connect(ui.proxyTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(stateChange()));
	connect(ui.proxyHostEdit, SIGNAL(textChanged(QString)), this, SLOT(stateChange()));
	connect(ui.proxyPortSpinBox, SIGNAL(valueChanged(int)), this, SLOT(stateChange()));
	connect(ui.proxyAuthBox, SIGNAL(stateChanged(int)), this, SLOT(stateChange()));
	connect(ui.proxyUserEdit, SIGNAL(textChanged(QString)), this, SLOT(stateChange()));
	connect(ui.proxyPasswordEdit, SIGNAL(textChanged(QString)), this, SLOT(stateChange()));
	ui.applyButton->setEnabled(false);
}

jAccountSettings::~jAccountSettings()
{

}

void jAccountSettings::loadSettings()
{
	QSettings profile_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	QString default_resource = profile_settings.value("main/defaultresource", "qutIM").toString();
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
	settings.beginGroup("main");
	ui.paswdEdit->setText(settings.value("password","").toString());
	ui.jidEdit->setText(m_account_name);
	ui.autologinCheck->setChecked(settings.value("autoconnect", true).toBool());
	ui.previousStatusCheck->setChecked(settings.value("keepstatus", true).toBool());
	ui.previousStatusCheck->setEnabled(ui.autologinCheck->isChecked());
	ui.localBookmarkCheck->setChecked(settings.value("localbookmark", false).toBool());
	ui.resourceEdit->setText(settings.value("resource",default_resource).toString());
	ui.prioritySpinBox->setValue(settings.value("priority",30).toInt());
	ui.priorityCheck->setChecked(settings.value("autopriority",true).toBool());
	ui.prioritySpinBox->setEnabled(!ui.priorityCheck->isChecked());
	ui.tlsComboBox->setCurrentIndex(settings.value("tlspolicy",1).toInt());
	ui.compressCheckBox->setChecked(settings.value("compress",true).toBool());
	ui.manualCheckBox->setChecked(!settings.value("usedns",true).toBool());
	ui.hostEdit->setEnabled(ui.manualCheckBox->isChecked());
	ui.portSpinBox->setEnabled(ui.manualCheckBox->isChecked());
	QString server = m_account_name.section("@",1);
	if(server=="qutim.org")
		server = "jabber.qutim.org";
	ui.hostEdit->setText(settings.value("server",server).toString());
	ui.portSpinBox->setValue(settings.value("port",5222).toInt());
	settings.endGroup();
	settings.beginGroup("proxy");
	ui.proxyTypeComboBox->setCurrentIndex(settings.value("type",3).toInt());
	ui.proxyHostEdit->setText(settings.value("server","").toString());
	ui.proxyPortSpinBox->setValue(settings.value("port",3128).toInt());
	ui.proxyAuthBox->setChecked(settings.value("auth",false).toBool());
	ui.proxyUserEdit->setText(settings.value("user","").toString());
	ui.proxyPasswordEdit->setText(settings.value("password","").toString());
	changeEnabled(ui.proxyTypeComboBox->currentIndex());
	connect(ui.proxyTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeEnabled(int)));
	settings.endGroup();
	ui.okButton->setIcon(m_account->getPluginSystem().getIcon("apply"));
	ui.applyButton->setIcon(m_account->getPluginSystem().getIcon("apply"));
	ui.cancelButton->setIcon(m_account->getPluginSystem().getIcon("cancel"));
	ui.tabWidget->setTabIcon(0, QIcon(":/icons/protocol.png"));
	ui.tabWidget->setTabIcon(1, m_account->getPluginSystem().getIcon("network"));
	ui.tabWidget->setTabIcon(2, m_account->getPluginSystem().getIcon("proxy"));
	setWindowTitle(tr("Editing %1").arg(m_account_name));
	setWindowIcon(m_account->getPluginSystem().getIcon("edituser"));
}

void jAccountSettings::saveSettings()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
	settings.beginGroup("main");
	settings.setValue("password", ui.paswdEdit->text());
	settings.setValue("autoconnect", ui.autologinCheck->isChecked());
	if (ui.autologinCheck->isChecked())
		settings.setValue("keepstatus", ui.previousStatusCheck->isChecked());
	else
		settings.setValue("keepstatus", false);
	settings.setValue("localbookmark", ui.localBookmarkCheck->isChecked());
	settings.setValue("resource", ui.resourceEdit->text());
	settings.setValue("autopriority", ui.priorityCheck->isChecked());
	settings.setValue("priority", ui.prioritySpinBox->value());
	ui.prioritySpinBox->setDisabled(ui.priorityCheck->isChecked());
	settings.setValue("tlspolicy", ui.tlsComboBox->currentIndex());
	settings.setValue("compress", ui.compressCheckBox->isChecked());
	settings.setValue("usedns", !ui.manualCheckBox->isChecked());
	settings.setValue("server", ui.hostEdit->text());
	settings.setValue("port", ui.portSpinBox->value());
	settings.endGroup();
	settings.beginGroup("proxy");
	settings.setValue("type", ui.proxyTypeComboBox->currentIndex());
	settings.setValue("server", ui.proxyHostEdit->text());
	settings.setValue("port", ui.proxyPortSpinBox->value());
	settings.setValue("auth", ui.proxyAuthBox->isChecked());
	settings.setValue("user", ui.proxyUserEdit->text());
	settings.setValue("password", ui.proxyPasswordEdit->text());
	settings.endGroup();
	emit settingsSaved();
	m_account->getProtocol()->loadSettings();
}

void jAccountSettings::changeEnabled(int pos)
{
	if (pos == 0 || pos == 3)
	{
		ui.proxyHostEdit->setEnabled(false);
		ui.proxyPortSpinBox->setEnabled(false);
		ui.proxyAuthBox->setEnabled(false);
		ui.proxyAuthBox->setChecked(false);
		ui.proxyUserEdit->setEnabled(false);
		ui.proxyPasswordEdit->setEnabled(false);
	}
	else
	{
		ui.proxyHostEdit->setEnabled(true);
		ui.proxyPortSpinBox->setEnabled(true);
		ui.proxyAuthBox->setEnabled(true);
		if (ui.proxyAuthBox->isChecked())
		{
			ui.proxyUserEdit->setEnabled(true);
			ui.proxyPasswordEdit->setEnabled(true);
		}
	}
}

void jAccountSettings::applyButtonClicked()
{
	if (!ui.applyButton->isEnabled())
		return;
	ui.applyButton->setEnabled(false);

	if (ui.paswdEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("You must enter a password"));
		return;
	}

	saveSettings();
}

void jAccountSettings::okButtonClicked()
{
	applyButtonClicked();
	if (!ui.paswdEdit->text().isEmpty())
		close();
}
