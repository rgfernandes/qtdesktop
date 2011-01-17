/*
jAdhoc.h

Copyright (c) 2008 by Kostin Dmitrij <kostindima@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JADHOC_H
#define JADHOC_H

#include <QWidget>
#include <QtDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QDialogButtonBox>

#include <gloox/adhoc.h>
#include <gloox/discohandler.h>
#include <gloox/disco.h>
#include <gloox/adhochandler.h>

#include "jAccount.h"
#include "jDataForm.h"

using namespace gloox;

class jDataForm;

class jAdhoc : public QWidget, AdhocHandler
{
    Q_OBJECT
public:
    jAdhoc(const QString &server, Adhoc *adhoc, QWidget* parent=0);
    virtual ~jAdhoc();

    virtual void handleAdhocSupport (const JID &remote, bool support){ }
    virtual void handleAdhocCommands (const JID &remote, const StringMap &commands);
    virtual void handleAdhocError (const JID &remote, const Error *error){ }
    virtual void handleAdhocExecutionResult (const JID &remote, const Adhoc::Command &command);

    void execute(const QString &node);
    void getCommands();

private:
    void addButton(const QString &name, const char *member);
    void clear();
    Adhoc *m_adhoc;
    jDataForm* d_form;
    std::string m_node, m_server;
    std::string session_id;
    QDialogButtonBox *m_button_box;
    QVBoxLayout *m_data_layout;
    QHash<QRadioButton *, std::string> m_options;

private slots:
    void doExecute();
    void doCancel();
    void doNext();
    void doPrev();
    void doComplete();
};

#endif // JADHOC_H
