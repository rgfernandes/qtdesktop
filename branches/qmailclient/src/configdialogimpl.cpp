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

#include <QtCore>
#include <QtGui>

#include "configdialogimpl.h"

ConfigDialogImpl::ConfigDialogImpl(QWidget *parent) 
	: QDialog(parent)
{
	setupUi(this);
	
	QSettings s(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	// general
	cbService->setCurrentIndex(s.value("service", 0).toInt());
	cbTransport->setCurrentIndex(s.value("transport", 0).toInt());
	leName->setText(s.value("name","").toString());
	leMail->setText(s.value("mail","").toString());
	// POP3
	s.beginGroup("pop3");
	lePOP3Host->setText(s.value("host","").toString());
	sbPOP3Port->setValue(s.value("port",110).toInt());
	lePOP3Login->setText(s.value("login","").toString());
	lePOP3Password->setText(s.value("password","").toString());
	cbPOP3Crypto->setCurrentIndex(s.value("crypto","").toInt());
	cbPOP3Auth->setCurrentIndex(s.value("auth","").toInt());
	s.endGroup();
	// MailDir
	s.beginGroup("maildir");
	leMailDirFolder->setText(s.value("folder", QDesktopServices::storageLocation(QDesktopServices::DataLocation)).toString());
	s.endGroup();
	// SMTP
	s.beginGroup("smtp");
	leSMTPHost->setText(s.value("host","").toString());
	sbSMTPPort->setValue(s.value("port",25).toInt());
	leSMTPLogin->setText(s.value("login","").toString());
	leSMTPPassword->setText(s.value("password","").toString());
	cbSMTPCrypto->setCurrentIndex(s.value("crypto","").toInt());
	cbSMTPAuth->setCurrentIndex(s.value("auth","").toInt());
	s.endGroup();
	// Sendmail
	connect(this,SIGNAL(accepted()),this,SLOT(writeSettings()));
}

ConfigDialogImpl::~ConfigDialogImpl()
{
}

void ConfigDialogImpl::on_tbMailDirFolder_clicked()
{
	QString folder=QFileDialog::getExistingDirectory(this, tr("Select the root path of the maildir"), leMailDirFolder->text());
	if(folder!="") leMailDirFolder->setText(folder);
}

void ConfigDialogImpl::writeSettings()
{
	QSettings s(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	s.setValue("service", cbService->currentIndex());
	s.setValue("transport", cbTransport->currentIndex());
	s.setValue("name", leName->text());
	s.setValue("mail", leMail->text());
	s.beginGroup("pop3");
	s.setValue("host", lePOP3Host->text());
	s.setValue("port", sbPOP3Port->value());
	s.setValue("login", lePOP3Login->text());
	s.setValue("password", lePOP3Password->text());
	s.setValue("crypto", cbPOP3Crypto->currentIndex());
	s.setValue("auth", cbPOP3Auth->currentIndex());
	s.endGroup();
	s.beginGroup("smtp");
	s.setValue("host", leSMTPHost->text());
	s.setValue("port", sbSMTPPort->value());
	s.setValue("login", leSMTPLogin->text());
	s.setValue("password", leSMTPPassword->text());
	s.setValue("crypto", cbSMTPCrypto->currentIndex());
	s.setValue("auth", cbSMTPAuth->currentIndex());
	s.endGroup();
	s.beginGroup("maildir");
	s.setValue("folder", leMailDirFolder->text());
	s.endGroup();
}
