/*****************************************************************************
    GeneralSettings

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

#include "generalsettings.h"
#include <QtCore>

GeneralSettings::GeneralSettings(QString aProfile, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::GeneralSettings), m_changed(false)
{
    m_ui->setupUi(this);

    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+aProfile+"/mrimsettings");
    bool restoreStatus = settings.value("main/restoreStatus",true).toBool();
    bool phoneCnts = settings.value("main/phoneCnts").toBool();

    m_ui->restoreStatusCheckBox->setCheckState( (restoreStatus)?Qt::Checked : Qt::Unchecked);
    m_ui->showPhoneCheckBox->setCheckState( (phoneCnts)?Qt::Checked : Qt::Unchecked);
    m_ui->showStatusCheck->setChecked(settings.value("roster/statustext", true).toBool());
    connect( m_ui->showStatusCheck, SIGNAL(stateChanged(int)),this, SLOT(widgetStateChanged()));
    connect( m_ui->restoreStatusCheckBox, SIGNAL(stateChanged(int)),this, SLOT(widgetStateChanged()));
    connect( m_ui->showPhoneCheckBox, SIGNAL(stateChanged(int)),this, SLOT(widgetStateChanged()));
}

GeneralSettings::~GeneralSettings()
{
    delete m_ui;
}

void GeneralSettings::widgetStateChanged()
{
    m_changed = true;
    emit settingsChanged();
}

