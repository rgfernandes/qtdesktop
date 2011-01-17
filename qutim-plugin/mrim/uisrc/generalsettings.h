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

#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QtGui/QWidget>
#include "ui_generalsettings.h"

class GeneralSettings : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(GeneralSettings)
public:
    explicit GeneralSettings(QString aProfile, QWidget *parent = 0);
    virtual ~GeneralSettings();
    inline bool GetShowPhoneCnts() { return (m_ui->showPhoneCheckBox->checkState() == Qt::Checked)?true:false; }
    inline bool GetRestoreStatus() { return (m_ui->restoreStatusCheckBox->checkState() == Qt::Checked)?true:false;  }
    inline bool GetStatustextStatus() { return m_ui->showStatusCheck->isChecked(); }
private:
    bool m_changed;
    Ui::GeneralSettings *m_ui;

private slots:
        void widgetStateChanged();
signals:
        void settingsChanged();
        void settingsSaved();
};

#endif // GENERALSETTINGS_H
