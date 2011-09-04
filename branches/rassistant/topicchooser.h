/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on Trolltech Qt Assistant 4.3, http://trolltech.com
****************************************************************************/
#ifndef TOPICCHOOSER_H
#define TOPICCHOOSER_H

#include "ui_topicchooser.h"

#include <QDialog>
#include <QStringList>

class TopicChooser : public QDialog
{
    Q_OBJECT
public:
    TopicChooser(QWidget *parent, const QStringList &lnkNames,
                  const QStringList &lnks, const QString &title);

    QString link() const;

    static QString getLink(QWidget *parent, const QStringList &lnkNames,
                            const QStringList &lnks, const QString &title);

private slots:
    void on_buttonDisplay_clicked();
    void on_buttonCancel_clicked();
    void on_listbox_itemActivated(QListWidgetItem *item);

private:
    Ui::TopicChooser ui;
    QString theLink;
    QStringList links, linkNames;
};

#endif // TOPICCHOOSER_H
