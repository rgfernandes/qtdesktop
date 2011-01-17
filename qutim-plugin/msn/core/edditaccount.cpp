/*
    EdditAccount

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

#include "edditaccount.h"
#include "ui_edditaccount.h"
#include <qutim/iconmanagerinterface.h>
using namespace qutim_sdk_0_2;

EdditAccount::EdditAccount(const QString &account, const QString &profile_name,QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::EdditAccount),
    m_profile_name(profile_name),
    m_account_name(account)
{
    m_ui->setupUi(this);
    move(desktopCenter());
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);

    setWindowTitle(tr("Editing %1").arg(account));
    setWindowIcon(Icon("edituser"));
    m_ui->okButton->setIcon(Icon("apply"));
    m_ui->applyButton->setIcon(Icon("apply"));
    m_ui->cancelButton->setIcon(Icon("cancel"));
    m_ui->tabWidget->setTabIcon(0,QIcon(":/icons/msn.png"));
    m_ui->tabWidget->setTabIcon(1,Icon("statuses"));
    loadSettings();
}

EdditAccount::~EdditAccount()
{
    delete m_ui;
}

void EdditAccount::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QPoint EdditAccount::desktopCenter()
{
	QDesktopWidget &desktop = *QApplication::desktop();
        return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void EdditAccount::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "accountsettings");
    m_ui->passEdit->setText(settings.value("main/password","").toString());
    m_ui->autoBox->setChecked(settings.value("main/autoconnect",false).toBool());

    StatusContainer container;
    container.m_dshow_dialog = settings.value("autoreply/onlinedshow",false).toBool();
    container.m_message = settings.value("autoreply/onlinemsg","").toString();
    m_status_container_list.append(container);

    container.m_dshow_dialog = settings.value("autoreply/busydshow",false).toBool();
    container.m_message = settings.value("autoreply/busymsg","").toString();
    m_status_container_list.append(container);

    container.m_dshow_dialog = settings.value("autoreply/idledshow",false).toBool();
    container.m_message = settings.value("autoreply/idlemsg","").toString();
    m_status_container_list.append(container);

    container.m_dshow_dialog = settings.value("autoreply/rightbackdshow",false).toBool();
    container.m_message = settings.value("autoreply/rightbackmsg","").toString();
    m_status_container_list.append(container);

    container.m_dshow_dialog = settings.value("autoreply/awaydshow",false).toBool();
    container.m_message = settings.value("autoreply/awaymsg","").toString();
    m_status_container_list.append(container);

    container.m_dshow_dialog = settings.value("autoreply/onphonedshow",false).toBool();
    container.m_message = settings.value("autoreply/onphonemsg","").toString();
    m_status_container_list.append(container);

    container.m_dshow_dialog = settings.value("autoreply/lunchdshow",false).toBool();
    container.m_message = settings.value("autoreply/lunchmsg","").toString();
    m_status_container_list.append(container);

    on_statusBox_currentIndexChanged(0);
}

void EdditAccount::saveSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/MSN."+m_account_name, "accountsettings");
    settings.setValue("main/password",m_ui->passEdit->text());
    settings.setValue("main/autoconnect",m_ui->autoBox->isChecked());

    settings.setValue("autoreply/onlinedshow",m_status_container_list[0].m_dshow_dialog);
    settings.setValue("autoreply/onlinemsg",m_status_container_list[0].m_message);

    settings.setValue("autoreply/busydshow",m_status_container_list[1].m_dshow_dialog);
    settings.setValue("autoreply/busymsg",m_status_container_list[1].m_message);

    settings.setValue("autoreply/idledshow",m_status_container_list[2].m_dshow_dialog);
    settings.setValue("autoreply/idlemsg",m_status_container_list[2].m_message);

    settings.setValue("autoreply/rightbackdshow",m_status_container_list[3].m_dshow_dialog);
    settings.setValue("autoreply/rightbackmsg",m_status_container_list[3].m_message);

    settings.setValue("autoreply/awaydshow",m_status_container_list[4].m_dshow_dialog);
    settings.setValue("autoreply/awaymsg",m_status_container_list[4].m_message);

    settings.setValue("autoreply/onphonedshow",m_status_container_list[5].m_dshow_dialog);
    settings.setValue("autoreply/onphonemsg",m_status_container_list[5].m_message);

    settings.setValue("autoreply/lunchdshow",m_status_container_list[6].m_dshow_dialog);
    settings.setValue("autoreply/lunchmsg",m_status_container_list[6].m_message);
}

void EdditAccount::on_cancelButton_clicked()
{
    close();
}

void EdditAccount::on_applyButton_clicked()
{
    saveSettings();
}

void EdditAccount::on_okButton_clicked()
{
    saveSettings();
    close();
}

void EdditAccount::on_statusBox_currentIndexChanged(int index)
{
    StatusContainer tmp_container = m_status_container_list.at(index);
    m_ui->replyBox->setChecked(tmp_container.m_dshow_dialog);
    m_ui->statusEdit->setPlainText(tmp_container.m_message);
}

void EdditAccount::on_replyBox_stateChanged(int state)
{
    m_status_container_list[(m_ui->statusBox->currentIndex())].m_dshow_dialog = state;
}

void EdditAccount::on_statusEdit_textChanged()
{
    m_status_container_list[(m_ui->statusBox->currentIndex())].m_message = m_ui->statusEdit->toPlainText();
}
