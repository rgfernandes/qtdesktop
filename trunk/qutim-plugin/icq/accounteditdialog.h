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

#ifndef ACCOUNTEDITDIALOG_H
#define ACCOUNTEDITDIALOG_H

#include <QtGui/QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include "ui_accounteditdialog.h"
#include "contactlist.h"



class AccountEditDialog : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(AccountEditDialog)
public:
    AccountEditDialog(const QString &uin, const QString &profile_name,
                      contactListTree *cl, QWidget *parent = 0);
    ~AccountEditDialog();

protected:
    virtual void changeEvent(QEvent *e);
private slots:
    void on_okButton_clicked();
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
    void proxyTypeChanged( int );
private:
    void loadSettings();
    Ui::accountEdit m_ui;
    QString m_uin;
    QPoint desktopCenter();
    QString m_profile_name;
    contactListTree *m_cl;
    void saveSettings();
};

#endif // ACCOUNTEDITDIALOG_H
