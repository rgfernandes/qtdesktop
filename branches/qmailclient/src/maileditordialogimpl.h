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

#ifndef MAILEDITORDIALOGIMPL_H
#define MAILEDITORDIALOGIMPL_H
//
#include "ui_MailEditorDialog.h"
#include "dummy.h"
#include <QtGui>

class AttachmentTreeWidgetItem : public QTreeWidgetItem
{
public:
	AttachmentTreeWidgetItem(QTreeWidget* item, const QString& _fname) : QTreeWidgetItem(item, 
		QStringList()<<QFileInfo(_fname).fileName()<<QString::number(QFileInfo(_fname).size()))
	{
		fname = _fname;
		is_file = true;
	};

	AttachmentTreeWidgetItem(QTreeWidget* item, Mail* _mail) : QTreeWidgetItem(item, 
		QStringList()<<"FW:"+_mail->subject()<<QString::number(_mail->getMemSize()))
	{
		fname = "";
		is_file = false;
		mail = _mail;
	};
	
	Mail *getMail(){return mail;};

	QString fname;
	Mail* mail;
	bool is_file;
};

class MailEditorDialogImpl : public QMainWindow, public Ui::MailEditorDialog
{
	Q_OBJECT
public:
	MailEditorDialogImpl(QWidget * parent = 0, Qt::WFlags f = 0 );
	void setCc(QString copy){cc->setText(copy);}; //set the cc-recipient of the mail
	void setFileName(QString name);
	void setDraftDir(QString dir){draftDir=dir;};
	void setMail(Mail *m); //set mail-attachment, e.g. a response or a forwarded message
	void setTo(QString rcpt){to->setText(rcpt);}; //set the recipient of the mail
	void setSender(QString sender){from->setText(sender);}; //set the sender of the mail
	void setSubject(QString s){subject->setText(s);}; //set the subject of the mail
	
signals:
	void sendMail(Mail *m);
	
private slots:
	void currentFontChanged(QFont font);
	void currentSizeChanged(QString size);
	void on_actionAddAttachment_triggered();
	void on_actionAlignmentCenter_toggled(bool checked);
	void on_actionAlignmentJustified_toggled(bool checked);
	void on_actionAlignmentLeft_toggled(bool checked);
	void on_actionAlignmentRight_toggled(bool checked);
	void on_actionBold_toggled(bool checked);
	void on_actionExit_triggered();
	void on_actionItalic_toggled(bool checked);
	void on_actionSendMail_triggered();
	void on_actionSave_triggered();
	void on_actionUnderlined_toggled(bool checked);
	void updateFontActionState();
		
private:
	QAction *actionFontCombo;
	QAction *actionFontSize;
	Mail *buildMail();
	void createActions();
	void createMenus();
	QString generateMailName(QString dir=""); //routine to generate a unique name for a mail, dir=path to file
	void setActionAlignmentChecked(int a);

	QWidget *p;
	QFontComboBox *fontCombo;
	QComboBox *fontSize;
	
	QString draftDir;
	QString fileName;
	
	bool updateFormat;
	
};
#endif
