/*
jAdhoc.cpp

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

#include "jAdhoc.h"
#include "utils.h"

jAdhoc::jAdhoc(const QString &server, Adhoc *adhoc, QWidget* parent) : QWidget(parent)
{
    setObjectName("adhoc_form");
    m_adhoc = adhoc;
    m_server = utils::toStd(server);
    QGridLayout *layout = new QGridLayout(this);
    m_data_layout = new QVBoxLayout(this);
    m_button_box = new QDialogButtonBox(this);
    layout->addLayout(m_data_layout, 0, 0);
    layout->addWidget(m_button_box, 1, 0);
    setAttribute(Qt::WA_DeleteOnClose, true);
    d_form = 0;
    show();
}

jAdhoc::~jAdhoc()
{
}

void jAdhoc::handleAdhocExecutionResult (const JID &remote, const Adhoc::Command &command)
{
    clear();
   // if (command.status() == Adhoc::Command::Completed) return;
    session_id = command.sessionID();
    //Instruction
    StringList lst = command.form()->instructions();
    StringList::iterator it;
    for ( it=lst.begin() ; it != lst.end(); it++ ){
	m_data_layout->addWidget(new QLabel(utils::fromStd(*it), this));
    }
    //Form
    d_form = new jDataForm(const_cast<DataForm *>(command.form()), this);
    m_data_layout->addWidget(d_form, 1);
    //Buttons
    QSpacerItem *horizontal_spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  //  m_button_layout->addItem(horizontal_spacer);
    if(command.actions() & Adhoc::Command::Execute){
	addButton(tr("Finish"), SLOT(doExecute()));
    }
    if(command.actions() & Adhoc::Command::Cancel){
	addButton(tr("Cancel"), SLOT(doCancel()));
    }
    if(command.actions() & Adhoc::Command::Previous){
	addButton(tr("Previous"), SLOT(doPrev()));
    }
    if(command.actions() & Adhoc::Command::Next){
	addButton(tr("Next"), SLOT(doNext()));
    }
    if(command.actions() & Adhoc::Command::Complete){
	addButton(tr("Complete"), SLOT(doComplete()));
    }
    if (command.actions() == 0){
	addButton(tr("Ok"), SLOT(doCancel()));
    }
}

void jAdhoc::handleAdhocCommands (const JID &remote, const StringMap &commands)
{
    clear();
    qDebug() << utils::fromStd(remote.full());
    StringMap::const_iterator it = commands.begin();
    for( ; it != commands.end(); ++it )
    {
	QRadioButton *option = new QRadioButton(utils::fromStd((*it).second), this);
	m_data_layout->addWidget(option);
	m_options.insert(option, (*it).first);
    }
    QSpacerItem *horizontal_spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
   // m_button_layout->addItem(horizontal_spacer);
    if(!m_options.isEmpty())
	addButton(tr("Next"), SLOT(doExecute()));
}

void jAdhoc::addButton(const QString &name, const char *member)
{
    QPushButton *button = m_button_box->addButton(name, QDialogButtonBox::AcceptRole);
    connect(button, SIGNAL(clicked()), this, member);
}

void jAdhoc::execute(const QString &node)
{
    m_node = utils::toStd(node);
    m_adhoc->execute(JID(m_server), new Adhoc::Command (m_node, Adhoc::Command::Execute), this );
}

void jAdhoc::getCommands()
{
    m_adhoc->getCommands(JID(m_server), this);
}

void jAdhoc::doExecute(){
    if(!m_options.isEmpty())
    {
	QList<QRadioButton *> options = m_options.keys();
	m_node = "";
	foreach(QRadioButton *option, options)
	{
	    if(option->isChecked())
	    {
		m_node = m_options.value(option);
		break;
	    }
	}
	if(m_node.empty())
	    return;
	m_adhoc->execute(JID(m_server), new Adhoc::Command (m_node, Adhoc::Command::Execute), this );
	return;
    }
//    if(sender() != this)
//    {
//        QString node = sender()->property("adhoc_node").toString();
//        if(!node.isEmpty())
//            m_node = utils::toStd(node);
//    }
    m_adhoc->execute(JID(m_server), new Adhoc::Command (m_node, session_id, Adhoc::Command::Executing, d_form?d_form->getDataForm():0), this );
}

void jAdhoc::doCancel(){
    m_adhoc->execute(JID(m_server), new Adhoc::Command (m_node, session_id, Adhoc::Command::Cancel), this );
}

void jAdhoc::doNext(){
    m_adhoc->execute(JID(m_server), new Adhoc::Command (m_node, session_id, Adhoc::Command::Executing, d_form->getDataForm()), this );
}

void jAdhoc::doPrev(){
    m_adhoc->execute(JID(m_server), new Adhoc::Command (m_node, session_id, Adhoc::Command::Previous), this );

}

void jAdhoc::doComplete(){
    m_adhoc->execute(JID(m_server), new Adhoc::Command (m_node, session_id, Adhoc::Command::Complete), this );
   // m_data_layout->removeWidget(d_form);
}

void jAdhoc::clear()
{
    qDeleteAll(m_options.keys());
    m_options.clear();

    m_button_box->clear();

    /*QLayoutItem *child;
    while (child = m_data_layout->takeAt(0))
	delete child;
    while (child = m_button_layout->takeAt(0))
	delete child;*/

    if(d_form)
    delete d_form;
  //  d_form = 0;
}
