/****************************************************************************
* http://sourceforge.net/projects/nlcreator/
*
* QNewsletterCreator - Business Email Client for Mass Mails
* Nuntius Leo - Personal Qt Email Client
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License Version 2 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
* USA. http://www.gnu.org/copyleft/library.html
*
* Please report all bugs and problems to the project admins:
* http://sourceforge.net/projects/nlcreator/
* 
*
* Copyright by dontinelli@users.sourceforge.net if no additional copyright information is given
* otherwise copyright by given authors/organizations 
* and modfified, e.g. ported, by dontinelli@users.sourceforge.net.
* Some code has been taken from 
* http://sourceforge.net/projects/lhmail - License : GNU Library General Public License    
* Authors: lukasz.iwaszkiewicz@gmail.com
* lukasz.iwaszkiewicz@lefthand.com.pl, lukasz.iwaszkiewicz@software.com.pl 
* Copyright (C) 2004/2005 LeftHand sp.z.o.o. info@lefthand.com.pl 
*
*****************************************************************************/

#include "configdialogimpl.h"
//
#ifdef Q_WS_X11
	#define MAIL_PATH "/.qmailclient/mails"
#else
	#define MAIL_PATH "/QMailClient/Mails"
#endif


ConfigDialogImpl::ConfigDialogImpl(QWidget *parent) 
	: QDialog(parent)
{
	setupUi(this);
	
	QSettings s("KiSoft","Nuntius Leo");
	mailBoxRootFolder->setText(s.value("mailBoxRootFolder",QDir::homePath()+MAIL_PATH).toString());
	s.beginGroup("pop");
	popHost->setText(s.value("host","").toString());
	popPort->setValue(s.value("port",110).toInt());
	name->setText(s.value("name","").toString());
	mail->setText(s.value("mail","").toString());
	usr->setText(s.value("usr","").toString());
	pwd->setText(s.value("pwd","").toString());
	s.endGroup();
	s.beginGroup("smtp");
	smtpHost->setText(s.value("host","").toString());
	smtpPort->setValue(s.value("port",25).toInt());
	s.endGroup();
	
	connect(this,SIGNAL(accepted()),this,SLOT(writeSettings()));
}
//
ConfigDialogImpl::~ConfigDialogImpl()
{
}

void ConfigDialogImpl::on_editMailBoxFolder_clicked()
{
	QString folder=QFileDialog::getExistingDirectory(this, tr("Select the root path of the mailbox"),mailBoxRootFolder->text());
	if(folder!="") mailBoxRootFolder->setText(folder);
}

void ConfigDialogImpl::writeSettings()
{
	QSettings s("TI_Eugene","QMailClient");
	s.setValue("mailBoxRootFolder",mailBoxRootFolder->text());
	s.beginGroup("pop");
	s.setValue("host",popHost->text());
	s.setValue("name",name->text());
	s.setValue("mail",mail->text());
	s.setValue("usr",usr->text());
	s.setValue("pwd",pwd->text());
	s.setValue("port",popPort->value());
	s.endGroup();
	s.beginGroup("smtp");
	s.setValue("host",smtpHost->text());
	s.setValue("usr",usr->text());
	s.setValue("pwd",pwd->text());
	s.setValue("port",smtpPort->value());
	s.endGroup();
}

