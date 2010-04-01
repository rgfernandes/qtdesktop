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

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtGui/QWidget>
#include "ui_settingswidget.h"
#include <QNetworkProxy>

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    SettingsWidget(QString aProfileName, QString aAccountName = QString(), QWidget *parent = 0);
    ~SettingsWidget();
	inline QString GetHostText() { return ui.hostEdit->text(); }
	inline quint32 GetPortText() { return ui.portEdit->text().toULong(); }
	inline bool IsProxyEnabled() { return (ui.proxyCheckBox->checkState() == Qt::Checked)?true:false; }
	QNetworkProxy::ProxyType GetSelectedProxyType();
	inline QString GetProxyHostText() { return ui.proxyHostEdit->text(); }
	inline quint32 GetProxyPortText() { return ui.proxyPortEdit->text().toULong(); }
	inline QString GetProxyUsernameText() { return ui.proxyUsernameEdit->text(); }
	inline QString GetProxyPasswordText() { return ui.proxyPassEdit->text(); }
        void SaveSettings();
        void LoadSettings();
private:
	void UpdateControlsAvailablility();
private:
        Ui::SettingsWidgetClass ui;
	bool m_changed;
        QString m_account;
        QString m_profile;

private slots:
	void on_groupBox_2_toggled(bool);
	void widgetStateChanged();
signals:
	void settingsChanged();
	void settingsSaved();        
};

#endif // SETTINGSWIDGET_H
