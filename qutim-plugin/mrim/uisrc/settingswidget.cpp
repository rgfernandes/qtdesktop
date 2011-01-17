/*****************************************************************************
    SettingsWidget

    Copyright (c) 2009 by Rusanov Peter <tazkrut@mail.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "settingswidget.h"
#include <QVariant>
#include <QSettings>

SettingsWidget::SettingsWidget(QString aProfileName, QString aAccountName, QWidget *parent)
: QWidget(parent), m_account(aAccountName), m_profile(aProfileName)
{
    ui.setupUi(this);

    connect( ui.proxyCheckBox, SIGNAL(stateChanged(int)),this, SLOT(widgetStateChanged()));
    connect( ui.proxyTypeComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(widgetStateChanged()));
    connect( ui.hostEdit, SIGNAL(textEdited ( const QString & )),this, SLOT(widgetStateChanged()));
    connect( ui.portEdit, SIGNAL(textEdited ( const QString & )),this, SLOT(widgetStateChanged()));
    connect( ui.proxyHostEdit, SIGNAL(textEdited ( const QString & )),this, SLOT(widgetStateChanged()));
    connect( ui.proxyPortEdit, SIGNAL(textEdited ( const QString & )),this, SLOT(widgetStateChanged()));

    ui.groupBox->setVisible(false);
    QString settingsPath = "qutim/qutim."+m_profile;
    QString settingsFile = "mrimsettings";

    if (!m_account.isEmpty())
    {
        settingsPath += "/mrim."+m_account;
        settingsFile = "accountsettings";
    }
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, settingsPath, settingsFile);
    QString host(settings.value("main/host").toString());

    if (host == "")
    {
            host = "mrim.mail.ru";
    }
    ui.hostEdit->setText(host);

    quint32 port = settings.value("main/port").toUInt();

    if (port == 0)
    {
            port = 2042;
    }
    ui.portEdit->setText(QString::number(port));
    bool useProxy = settings.value("main/useProxy").toBool();
    ui.proxyCheckBox->setCheckState(useProxy ? Qt::Checked : Qt::Unchecked);
    bool convertedOk;
    QNetworkProxy::ProxyType proxyType = (QNetworkProxy::ProxyType)settings.value("main/proxyType").toUInt(&convertedOk);
    if (!convertedOk || proxyType == QNetworkProxy::NoProxy)
    {
            proxyType = QNetworkProxy::DefaultProxy;
    }
    ui.proxyHostEdit->setText(settings.value("main/proxyHost").toString());
    ui.proxyPortEdit->setText(QString::number(settings.value("main/proxyPort").toUInt()));

    ui.proxyTypeComboBox->addItem(tr("Default proxy"), QVariant(QNetworkProxy::DefaultProxy));
    ui.proxyTypeComboBox->addItem("SOCKS", QVariant(QNetworkProxy::Socks5Proxy));
    ui.proxyTypeComboBox->addItem("HTTP(S)", QVariant(QNetworkProxy::HttpProxy));

    int itemIndex = ui.proxyTypeComboBox->findData(QVariant(proxyType));
    if (itemIndex < ui.proxyTypeComboBox->count())
    {
            ui.proxyTypeComboBox->setCurrentIndex(itemIndex);
    }

    ui.proxyUsernameEdit->setText(settings.value("main/proxyUser").toString());
    ui.proxyPassEdit->setText(settings.value("main/proxyPass").toString());

    UpdateControlsAvailablility();
}

SettingsWidget::~SettingsWidget()
{

}


void SettingsWidget::on_groupBox_2_toggled(bool)
{

}

QNetworkProxy::ProxyType SettingsWidget::GetSelectedProxyType()
{
    bool convertedOk;
    QNetworkProxy::ProxyType proxyType = (QNetworkProxy::ProxyType)ui.proxyTypeComboBox->itemData(ui.proxyTypeComboBox->currentIndex()).toUInt(&convertedOk);
    if (!convertedOk)
    {
            proxyType = QNetworkProxy::NoProxy;
    }
    return proxyType;
}

void SettingsWidget::UpdateControlsAvailablility()
{
    bool proxySectionEnabled = true;
    bool proxyTypeComboEnabled = true;
    if (ui.proxyCheckBox->checkState() != Qt::Checked)
    {
            proxySectionEnabled = false;
            proxyTypeComboEnabled = false;
    }
    else
    {
            if (GetSelectedProxyType() == QNetworkProxy::DefaultProxy)
            {
                    proxySectionEnabled = false;
            }
    }


    ui.proxyHostEdit->setEnabled(proxySectionEnabled);
    ui.proxyPortEdit->setEnabled(proxySectionEnabled);
    ui.proxyTypeComboBox->setEnabled(proxyTypeComboEnabled);
    ui.proxyUsernameEdit->setEnabled(proxySectionEnabled);
    ui.proxyPassEdit->setEnabled(proxySectionEnabled);
}

void SettingsWidget::widgetStateChanged()
{
     m_changed = true;
     UpdateControlsAvailablility();
     emit settingsChanged();
}

void SettingsWidget::SaveSettings()
{
    QString settingsPath = "qutim/qutim."+m_profile;
    QString settingsFile = "mrimsettings";

    if (!m_account.isEmpty())
    {
        settingsPath += "/mrim."+m_account;
        settingsFile = "accountsettings";
    }
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, settingsPath, settingsFile);

    settings.setValue("main/host", GetHostText());
    settings.setValue("main/port", GetPortText());
    settings.setValue("main/useProxy", IsProxyEnabled());
    settings.setValue("main/proxyType", GetSelectedProxyType());
    settings.setValue("main/proxyHost", GetProxyHostText());
    settings.setValue("main/proxyPort", GetProxyPortText());
    settings.setValue("main/proxyUser", GetProxyUsernameText());
    settings.setValue("main/proxyPass", GetProxyPasswordText());
}
