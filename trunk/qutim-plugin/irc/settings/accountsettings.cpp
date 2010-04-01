/*
		ircAccountSettings

		Copyright (c) 2008-2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "accountsettings.h"

ircAccountSettings::ircAccountSettings(QSettings *account_settings, QWidget *parent, Qt::WFlags f) 
	: QWidget(parent, f)
{
	setupUi(this);
	settings = account_settings;

	loadSettings();

	QDesktopWidget &desktop = *QApplication::desktop();
	move(desktop.width()/2 - width()/2, desktop.height()/2 - height()/2);

	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
}

void ircAccountSettings::loadSettings()
{
	editNick->setText(settings->value("main/nick").toString());
	editNick2->setText(settings->value("main/nick2").toString());
	editRealName->setText(settings->value("main/realname").toString());

	editServer->setText(settings->value("main/server").toString());
	editPort->setText(settings->value("main/port").toString());

	int codepageIndex = cbCodepage->findText(settings->value("main/codepage", "Windows-1251").toString());
	if (codepageIndex > -1)	cbCodepage->setCurrentIndex(codepageIndex);
	else cbCodepage->setCurrentIndex(47);

	if (editNick2->text().trimmed().length()<1 && editNick->text().trimmed().length()>0) editNick2->setText(editNick->text().trimmed() + "_");

	editAutoCmds->setText(settings->value("advanced/autocmds").toString());
	editMsgPart->setText(settings->value("advanced/msgpart").toString());
	editMsgQuit->setText(settings->value("advanced/msgquit").toString());

	cbAutologin->hide(); // hide because not worked
	cbAutologin->setChecked(settings->value("main/autologin").toBool());

	cbAge->setValue(settings->value("identify/age").toInt());

	int cb_idx = cbGender->findText(settings->value("identify/gender").toString());
	if (cb_idx>0)	cbGender->setCurrentIndex(cb_idx);

	editLocation->setText(settings->value("identify/location").toString());
	editLanguages->setText(settings->value("identify/languages").toString());
	editAvatar->setText(settings->value("identify/avatar").toString());
	editOther->setText(settings->value("identify/other").toString());

}

void ircAccountSettings::saveSettings()
{
	settings->setValue("main/nick", editNick->text());
	settings->setValue("main/nick2", editNick2->text());
	settings->setValue("main/realname", editRealName->text());
	settings->setValue("main/server", editServer->text());
	settings->setValue("main/port", editPort->text());
	settings->setValue("main/codepage", cbCodepage->currentText());
	settings->setValue("advanced/autocmds", editAutoCmds->toPlainText());
	settings->setValue("advanced/msgpart", editMsgPart->text());
	settings->setValue("advanced/msgquit", editMsgQuit->text());
	settings->setValue("main/autologin", cbAutologin->isChecked());


	if (cbAge->value()>0) settings->setValue("identify/age", QString::number(cbAge->value()));
	if (cbGender->currentIndex()>0) settings->setValue("identify/gender", cbGender->currentText());
	if (!editLocation->text().trimmed().isEmpty()) settings->setValue("identify/location", editLocation->text());
	if (!editLanguages->text().trimmed().isEmpty()) settings->setValue("identify/languages", editLanguages->text());
	if (!editAvatar->text().trimmed().isEmpty()) settings->setValue("identify/avatar", editAvatar->text());
	if (!editOther->text().trimmed().isEmpty()) settings->setValue("identify/other", editOther->text());
}

void ircAccountSettings::on_btnApply_clicked()
{
	saveSettings();
}

void ircAccountSettings::on_btnOK_clicked()
{
	saveSettings();
	this->close();
}

void ircAccountSettings::on_btnCancel_clicked()
{
	this->close();
}

