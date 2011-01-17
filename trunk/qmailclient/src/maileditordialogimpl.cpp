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

#include "maileditordialogimpl.h"
//
MailEditorDialogImpl::MailEditorDialogImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	p=parent;
	setupUi(this);
	createActions();
	
	attachments->setVisible(false);
	
	connect(text,SIGNAL(selectionChanged()),this,SLOT(updateFontActionState()));
	
}

Mail *MailEditorDialogImpl::buildMail()
{
	Mail *m=new Mail();
	m->setHeader(from->text(),to->text(),cc->text(),bcc->text(),subject->text());
	
	if(attachments->topLevelItemCount()==0){
		if(actionToggleHtml->isChecked()){
			m->setMessageBody(text->toHtml());
			m->setContentType("text/html");
			m->setEncoding("7bit");
		} else {
			m->setMessageBody(text->toPlainText());
			m->setContentType("text/plain");
			m->setEncoding("7bit");
		}
		m->setContentTransferEncoding("quoted-printable");
	} else {
		m->setContentType("multipart/mixed");
		m->setHeader("boundary","-=_NextPart_000_003C_01C8ADDB.5D3ED0E0");
		m->setEncoding("7bit");
		
		Mail *mp1=new Mail(m);
		if(actionToggleHtml->isChecked()){
			mp1->setMessageBody(text->toHtml());
			mp1->setContentType("text/html");
		} else {
			mp1->setMessageBody(text->toPlainText());
			mp1->setContentType("text/plain");
		}
		mp1->setContentTransferEncoding("quoted-printable");
		mp1->setEncoding("7bit");
		m->addPart(mp1);
		
		for (int i=0;i<attachments->topLevelItemCount();i++){
			//mp2=new Mail(m);
			AttachmentTreeWidgetItem *item=dynamic_cast<AttachmentTreeWidgetItem *>(attachments->itemAt(0,i));
			if(!item) continue;
			
			if(item->is_file) m->attachFile(item->fname);
			else m->attachMail(item->getMail()); //should content type be changed to message/rfc822???
		};
	}
	
	return m;
}


void MailEditorDialogImpl::createActions()
{	
	fontCombo=new QFontComboBox(textEditToolBar);
	fontSize=new QComboBox(textEditToolBar);
	fontSize->setEditable(true);
	fontSize->addItems(QStringList()<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12"<<"13"<<"14"<<"15"<<"16");
	fontSize->addItems(QStringList()<<"18"<<"20"<<"22"<<"24"<<"28"<<"32"<<"36"<<"40"<<"44"<<"48"<<"54"<<"60");
	fontSize->addItems(QStringList()<<"66"<<"72"<<"80");
	
	actionFontCombo=textEditToolBar->insertWidget(actionBold,fontCombo);
	actionFontSize=textEditToolBar->insertWidget(actionBold,fontSize);
	connect(fontCombo,SIGNAL(currentFontChanged(QFont)),this,SLOT(currentFontChanged(QFont)));
	connect(fontSize,SIGNAL(currentIndexChanged(QString)),this,SLOT(currentSizeChanged(QString)));
	connect(fontSize,SIGNAL(editTextChanged(QString)),this,SLOT(currentSizeChanged(QString)));
	//textEditToolBar->insertSeparator(actionBold);
}

void MailEditorDialogImpl::createMenus()
{
	QMenu *m= new QMenu(tr("Toolbars"),this);
	m->addAction(fileToolBar->toggleViewAction());
	m->addAction(textEditToolBar->toggleViewAction());
	menu_View->addMenu(m);
}

void MailEditorDialogImpl::currentFontChanged(QFont font)
{
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QTextCharFormat f=c.charFormat();
	f.setFontFamily(font.family());
	c.setCharFormat(f);
	text->setTextCursor(c);
}

void MailEditorDialogImpl::currentSizeChanged(QString size)
{
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QFont f=c.charFormat().font();
	f.setPointSize(size.toDouble());
	QTextCharFormat cf=c.charFormat();
	cf.setFont(f);
	c.setCharFormat(cf);
	text->setTextCursor(c);
}

QString MailEditorDialogImpl::generateMailName(QString dir)
{
	QFileInfo f;
	QString rand_name;
	
	while(1)
	{
#ifdef Q_OS_WIN32
		rand_name = QString("%1").arg(rand());
#else
        rand_name = QString("%1").arg(random());
#endif		
		f.setFile(dir + QDir::separator () + rand_name);
		
		if (!f.exists())
		{
			rand_name=f.absoluteFilePath();
			break;
		}
	}
	
	return rand_name;
}

void MailEditorDialogImpl::on_actionAddAttachment_triggered()
{
	QStringList files = QFileDialog::getOpenFileNames(this,tr("Select attachment"),"",
		tr("All Files (*.*)"));
	if (files.size()==0) return;
	for(int i=0;i<files.size();i++){
		AttachmentTreeWidgetItem *item=new AttachmentTreeWidgetItem(attachments,files.at(i));
	};
	attachments->setVisible(true);
}


void MailEditorDialogImpl::on_actionAlignmentCenter_toggled(bool checked)
{
	if(checked) setActionAlignmentChecked(1);
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QTextBlockFormat f=c.blockFormat();
	f.setAlignment(Qt::AlignCenter);
	c.setBlockFormat(f);
	text->setTextCursor(c);
}

void MailEditorDialogImpl::on_actionAlignmentJustified_toggled(bool checked)
{
	if(checked) setActionAlignmentChecked(0);
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QTextBlockFormat f=c.blockFormat();
	f.setAlignment(Qt::AlignLeft);
	c.setBlockFormat(f);
	text->setTextCursor(c);
}

void MailEditorDialogImpl::on_actionAlignmentLeft_toggled(bool checked)
{
	if(checked) setActionAlignmentChecked(3);
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QTextBlockFormat f=c.blockFormat();
	f.setAlignment(Qt::AlignJustify);
	c.setBlockFormat(f);
	text->setTextCursor(c);
}

void MailEditorDialogImpl::on_actionAlignmentRight_toggled(bool checked)
{
	if(checked) setActionAlignmentChecked(2);
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QTextBlockFormat f=c.blockFormat();
	f.setAlignment(Qt::AlignRight);
	c.setBlockFormat(f);
	text->setTextCursor(c);
}

void MailEditorDialogImpl::on_actionBold_toggled(bool checked)
{
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QTextCharFormat f=c.charFormat();
	int w;
	if(checked) w=QFont::Bold;
	else w=QFont::Normal;
	f.setFontWeight(w);
	c.setCharFormat(f);
	text->setTextCursor(c);
}

void MailEditorDialogImpl::on_actionExit_triggered()
{
	this->close();
}

void MailEditorDialogImpl::on_actionItalic_toggled(bool checked)
{
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QTextCharFormat f=c.charFormat();
	f.setFontItalic(checked);
	c.setCharFormat(f);
	text->setTextCursor(c);
}

void MailEditorDialogImpl::on_actionSave_triggered()
{
	Mail* m=buildMail();
	if(fileName.isEmpty()) fileName=generateMailName(draftDir);
	QFile f(fileName);
	f.open(QIODevice::WriteOnly);
	f.write(m->getSendData().toAscii());
	f.close();
	if(f.error()==QFile::NoError) QMessageBox::information(this,tr("Save Mail"),tr("Mail was successfully saved in the draft-folder"));
	else QMessageBox::information(this,tr("Save Mail"),tr("Mail could not be saved:\r\n")+f.errorString());
	delete m;
}

void MailEditorDialogImpl::on_actionSendMail_triggered()
{
	Mail* m=buildMail();
	qDebug()<<"\r\n\r\n"<<m->getSendData()<<"\r\n\r\n";
	emit sendMail(m);
	this->close();
}


void MailEditorDialogImpl::on_actionUnderlined_toggled(bool checked)
{
	if(updateFormat) return;
	QTextCursor c=text->textCursor();
	QTextCharFormat f=c.charFormat();
	f.setFontUnderline(checked);
	c.setCharFormat(f);
	text->setTextCursor(c);
}


void MailEditorDialogImpl::setActionAlignmentChecked(int a)
{
	if(a==0){
		actionAlignmentLeft->setChecked(true);
		actionAlignmentCenter->setChecked(false);
		actionAlignmentRight->setChecked(false);
		actionAlignmentJustified->setChecked(false);
	}else if(a==1){
		actionAlignmentLeft->setChecked(false);
		actionAlignmentCenter->setChecked(true);
		actionAlignmentRight->setChecked(false);
		actionAlignmentJustified->setChecked(false);
	}else if(a==2){
		actionAlignmentLeft->setChecked(false);
		actionAlignmentCenter->setChecked(false);
		actionAlignmentRight->setChecked(true);
		actionAlignmentJustified->setChecked(false);
	}else if(a==3){
		actionAlignmentLeft->setChecked(false);
		actionAlignmentCenter->setChecked(false);
		actionAlignmentRight->setChecked(false);
		actionAlignmentJustified->setChecked(true);
	}
}

void MailEditorDialogImpl::setFileName(QString name)
{
	fileName=name;
	QFile f(fileName);
	f.open(QIODevice::ReadOnly);
	Mail *m=new Mail();
	m->setData(QString(f.readAll()));
	f.close();
	
	to->setText(m->to());
	cc->setText(m->cc());
	bcc->setText(m->bcc());
	subject->setText(m->subject());
	
	//list of attachments has to be filled
	
	delete m;
}

void MailEditorDialogImpl::setMail(Mail *m)
{
	AttachmentTreeWidgetItem *item=new AttachmentTreeWidgetItem(attachments,m);
	attachments->setVisible(true);
}


void MailEditorDialogImpl::updateFontActionState()
{
	updateFormat=true;
	QTextCursor c=text->textCursor();
	QFont f=c.charFormat().font();
	QTextBlockFormat bf=c.blockFormat();
	
	//qDebug()<<c.charFormat().fontFamily();
	//fontCombo->setCurrentIndex(fontCombo->findText(c.charFormat().fontFamily()));
	//fontCombo->setCurrentText(c.charFormat().fontFamily());
	fontCombo->setCurrentFont(f);

	int index=fontSize->findText(QString::number(f.pointSize()));
	if(index==-1) fontSize->insertItem(0,QString::number(f.pixelSize()));
	else fontSize->setCurrentIndex(index);
	
	actionBold->setChecked(f.bold());
	actionItalic->setChecked(f.italic());
	actionUnderlined->setChecked(f.underline());
	
	if(bf.alignment()==Qt::AlignLeft) setActionAlignmentChecked(0);
	else if(bf.alignment()==Qt::AlignCenter) setActionAlignmentChecked(1);
	else if(bf.alignment()==Qt::AlignRight) setActionAlignmentChecked(2);
	else if(bf.alignment()==Qt::AlignJustify) setActionAlignmentChecked(3);
	//if(bf.alignment()==Qt::AlignBlock) actionAlignmentBlock->setChecked(true);
	updateFormat=false;
}


