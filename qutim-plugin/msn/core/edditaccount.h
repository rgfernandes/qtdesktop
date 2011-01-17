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

#ifndef EDDITACCOUNT_H
#define EDDITACCOUNT_H

#include <QtGui/QWidget>
#include <QApplication>
#include <QDesktopWidget>

struct StatusContainer {
    bool m_dshow_dialog;
    QString m_message;
};

namespace Ui {
    class EdditAccount;
}

class EdditAccount : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(EdditAccount)
public:
    explicit EdditAccount(const QString &account, const QString &profile_name,QWidget *parent = 0);
    virtual ~EdditAccount();

protected:
    virtual void changeEvent(QEvent *e);

private slots:
    void on_okButton_clicked();
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
    void on_statusBox_currentIndexChanged(int index);
    void on_replyBox_stateChanged(int state);
    void on_statusEdit_textChanged();

private:
    void loadSettings();
    void saveSettings();
    Ui::EdditAccount *m_ui;
    QString m_account_name;
    QString m_profile_name;
    QPoint desktopCenter();
    QVector<StatusContainer> m_status_container_list;
};

#endif // EDDITACCOUNT_H
