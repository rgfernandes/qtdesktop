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

#include "mainwinimpl.h"
#include "configdialogimpl.h"
#include "maileditordialogimpl.h"

MainWinImpl::MainWinImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	p=0;
	smtp=0;
	currentMail=0;

	setupUi(this);

	mailBrowser->setOpenLinks(false);

	readSettings();
	iniMailFolders();
	iniMailEngine();

	connect(smtp,SIGNAL(status(QString)),this,SLOT(displayState(QString)));
	connect(smtp,SIGNAL(mailSent(Mail*)),this,SLOT(mailSent(Mail*)));
	connect(p,SIGNAL(newMail(Mail*)),this,SLOT(newMail(Mail*)));
	connect(p,SIGNAL(status(QString)),this,SLOT(displayState(QString)));
	//downloadMails();
	connect(actionDownloadMails,SIGNAL(triggered()),this,SLOT(downloadMails()));
	//loadMails();
}

void MainWinImpl::addMailPartsToList(QTreeWidgetItem*parent, Mail *m)
{
	uint parts=m->getPartCount();
	for(uint i=0;i<parts;i++){
		Mail *mp=m->getPart(i);
		MailTreeWidgetItem* twi=new MailTreeWidgetItem(parent,mp);
		//parent->addChild(twi);
		if(mp->getPartCount()>0){
			addMailPartsToList(twi,mp);
			mailParts->expandItem(twi);
		}
	};
}

void MainWinImpl::addMailToList(Mail *m,QString fileName)
{
	lstMails->setSortingEnabled(false);
	QTableWidgetItem *subject=new QTableWidgetItem(m->subject());
	subject->setData(32,fileName);
	QTableWidgetItem *from=new QTableWidgetItem(m->from());
	QTableWidgetItem *date=new DateTableWidgetItem(m->header("Date"));
	QTableWidgetItem *size=new QTableWidgetItem(QString("%1 B").arg(m->getMemSize()));
	lstMails->insertRow(0);
	lstMails->setItem(0,0,subject);
	lstMails->setItem(0,1,from);
	lstMails->setItem(0,2,date);
	lstMails->setItem(0,3,size);
	lstMails->setSortingEnabled(true);
}

void MainWinImpl::displayState(QString s)
{
	this->statusBar()->showMessage(s,15000);
}

void MainWinImpl::downloadMails()
{
	p->getAllMails(false); //delete mails?
}

QString MainWinImpl::generateMailName(QString dir)
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

void MainWinImpl::iniMailEngine()
{
	if(p) delete p;
	if(smtp) delete smtp;
	p=new Pop3(popUsr,popPass,popHost,this);
	smtp=new Smtp(smtpUsr,smtpPass,smtpHost,this);
	smtp->setPreserveMails(true);
	qDebug()<<"mail engine initialised";
}

void MainWinImpl::iniMailFolders()
{
	dirModel=new QDirModel(this);
	dirModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	dirTree->setModel(dirModel);
	QDir d(mailBoxRootPath);

	if(!d.exists()) d.mkpath(d.absolutePath());
	dirTree->setRootIndex(dirModel->index(d.absolutePath()));

	if(!d.exists("inbox")) d.mkdir("inbox");
	if(!d.exists("outbox")) d.mkdir("outbox");
	if(!d.exists("trash")) d.mkdir("trash");
	if(!d.exists("draft")) d.mkdir("draft");
	
	inboxDir=d.absolutePath();
	inboxDir.cd("inbox");
	outboxDir=d.absolutePath();
	outboxDir.cd("outbox");
	trashDir=d.absolutePath();
	trashDir.cd("trash");
	draftDir=d.absolutePath();
	draftDir.cd("draft");
	
	return;
}


void MainWinImpl::loadMails(QString path)
{
	while(lstMails->rowCount()>0)lstMails->removeRow(0);
	
	QFileInfoList fi=QDir(path).entryInfoList(QDir::Files);
	for(int i=0;i<fi.count();i++){
		QFile f(fi[i].absoluteFilePath());
		if(!f.open(QFile::ReadOnly)) continue;
		Mail *m=new Mail();
		m->setData(QString(f.readAll()));
		f.close();
		addMailToList(m,f.fileName());
	};
}

void MainWinImpl::mailSent(Mail *mail)
{
	qDebug()<<"mail sent";
	QString fileName;
	fileName=outboxDir.absolutePath()+QDir::separator()+generateMailName();
	qDebug()<<fileName;
	QFile f(fileName);
	f.open(QFile::WriteOnly);
	f.write(mail->getSendData().toAscii());
	f.close();
	delete mail;
}

void MainWinImpl::newMail(Mail *mail)
{
	QString fileName;
	fileName=generateMailName(inboxDir.absolutePath());
	QFile f(fileName);
	f.open(QFile::WriteOnly);
	f.write(mail->getSendData().toAscii());
	f.close();
	delete mail;
}

void MainWinImpl::on_actionDeleteMail_triggered()
{
	if (lstMails->currentRow()<0 || lstMails->rowCount()<1) return;
	if(dirModel->filePath(dirTree->currentIndex())==trashDir.absolutePath()){
		QFile f(lstMails->item(lstMails->currentRow(),0)->data(32).toString());
		f.remove();
	}else{
		QFile f(lstMails->item(lstMails->currentRow(),0)->data(32).toString());
		QString fileName=f.fileName();
		fileName=fileName.right(fileName.size()-fileName.lastIndexOf("/")-1);
		f.rename(trashDir.absolutePath()+"/"+fileName);
	}
	lstMails->removeRow(lstMails->currentRow());
}

void MainWinImpl::on_actionEditConfiguration_triggered()
{
	ConfigDialogImpl dialog; //=new ConfigDialogImpl(this);
	dialog.exec();
	if(dialog.result()){
		readSettings();
		iniMailEngine();
	}
}

void MainWinImpl::on_actionEditMail_triggered()
{
	if (lstMails->currentRow()<0 || lstMails->rowCount()<1) return;
	if(currentMail==0) return;
	
	MailEditorDialogImpl *dialog=new MailEditorDialogImpl(this);
	dialog->setDraftDir(draftDir.absolutePath());
	dialog->setFileName(lstMails->item(lstMails->currentRow(),0)->data(32).toString());
	connect(dialog,SIGNAL(sendMail(Mail*)),this,SLOT(sendMail(Mail*)));
	dialog->show();
}

void MainWinImpl::on_actionForwardMail_triggered()
{
	if(currentMail==0) return;
	
	MailEditorDialogImpl *dialog=new MailEditorDialogImpl(this);
	dialog->setDraftDir(draftDir.absolutePath());
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setMail(currentMail);
	dialog->setSender(QString("%1 <%2>").arg(username).arg(mailAddress));
	dialog->setSubject("Fwd: "+currentMail->subject());
	connect(dialog,SIGNAL(sendMail(Mail*)),this,SLOT(sendMail(Mail*)));
	dialog->show();
}

void MainWinImpl::on_actionNewMail_triggered()
{
	MailEditorDialogImpl *dialog=new MailEditorDialogImpl(this);
	dialog->setDraftDir(draftDir.absolutePath());
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setSender(QString("%1 <%2>").arg(username).arg(mailAddress));
	connect(dialog,SIGNAL(sendMail(Mail*)),this,SLOT(sendMail(Mail*)));
	dialog->show();
}

void MainWinImpl::on_actionReplyAll_triggered()
{
	if(currentMail==0) return;
	
	MailEditorDialogImpl *dialog=new MailEditorDialogImpl(this);
	dialog->setDraftDir(draftDir.absolutePath());
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setMail(currentMail);
	dialog->setSender(QString("%1 <%2>").arg(username).arg(mailAddress));
	dialog->setSubject("Re: "+currentMail->subject());
	dialog->setTo(currentMail->header("From"));
	
	QString cc=currentMail->header("To");
	if(currentMail->header("Cc")!="") cc+=cc+"; "+currentMail->header("Cc");
	if(cc.contains(mailAddress)){
		int pos1=cc.lastIndexOf(QRegExp("[,;]"),cc.indexOf(mailAddress));
		int pos2=cc.indexOf(QRegExp("[,;]"),cc.indexOf(mailAddress));
		if(pos1==-1 && pos2>-1){
			cc=cc.right(cc.size()-pos2);
		}else if(pos1>-1 && pos2==-1){
			cc=cc.left(pos1-1);
		}else if(pos1>-1 && pos2>-1){
			cc=cc.left(pos1-1)+cc.right(cc.size()-pos2);
		}
	}
	dialog->setCc(cc);
	
	connect(dialog,SIGNAL(sendMail(Mail*)),this,SLOT(sendMail(Mail*)));
	dialog->show();
}

void MainWinImpl::on_actionReplyMail_triggered()
{
	if(currentMail==0) return;
	
	MailEditorDialogImpl *dialog=new MailEditorDialogImpl(this);
	dialog->setDraftDir(draftDir.absolutePath());
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setMail(currentMail);
	dialog->setSender(QString("%1 <%2>").arg(username).arg(mailAddress));
	dialog->setSubject("Re: "+currentMail->subject());
	dialog->setTo(currentMail->header("From"));
	connect(dialog,SIGNAL(sendMail(Mail*)),this,SLOT(sendMail(Mail*)));
	dialog->show();
}

void MainWinImpl::on_dirTree_clicked(const QModelIndex &index)
{
	loadMails(dirModel->filePath(index));
}

void MainWinImpl::on_lstMails_cellClicked(int curRow, int curCol)
{
  (void) curCol; // Not used.
	if (curRow<0 || lstMails->rowCount()<1) return;
	
	delete currentMail;
	
	QFile f(lstMails->item(curRow,0)->data(32).toString());
	if(!f.open(QFile::ReadOnly)) return;
	
	currentMail=new Mail();
	currentMail->setData(QString(f.readAll()));
	f.close();
	
	if(currentMail->header("Content-Type")=="text/plain") mailBrowser->setPlainText(currentMail->getShowText());
	else mailBrowser->setHtml(currentMail->getShowText());
	
	lblFrom->setText(tr("From: %1").arg(currentMail->header("From")));
	lblTo->setText(tr("To: %1").arg(currentMail->header("To")));
	lblDate->setText(tr("Date: %1").arg(currentMail->header("Date")));
	lblSubject->setText(tr("Subject: %1").arg(currentMail->header("Subject")));
		
	//update mailParts
	mailParts->clear();
	
	QTreeWidgetItem *parent=new QTreeWidgetItem(QStringList()<<currentMail->header("Subject")
	<<currentMail->header("Content-Type")<<currentMail->header("Content-Transfer-Encoding")<<QString("%1 B").arg(currentMail->getMemSize()));
	mailParts->addTopLevelItem(parent);
	if(currentMail->getPartCount()>0){
		addMailPartsToList(parent,currentMail);
		mailParts->expandItem(parent);
	}
}

void MainWinImpl::on_mailBrowser_anchorClicked( const QUrl &url)
{
	QDesktopServices::openUrl(url);
}

void MainWinImpl::on_mailParts_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous )
{
	MailTreeWidgetItem *i=dynamic_cast<MailTreeWidgetItem *>(current);
	if(i!=0){
		delete currentMail;
		currentMail=i->getMail();

		if(currentMail->header("Content-Type")=="text/plain") mailBrowser->setPlainText(currentMail->getShowText());
		else mailBrowser->setHtml(currentMail->getShowText());
	}
}

void MainWinImpl::readSettings()
{
	QSettings s(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	username=s.value("pop/name","").toString();
	mailAddress=s.value("pop/mail","").toString();
	popHost=s.value("pop/host").toString();
	popUsr=s.value("pop/usr").toString();
	popPass=s.value("pop/pwd").toString();
	smtpUsr=s.value("smtp/usr").toString();
	smtpPass=s.value("smtp/pwd").toString();
	smtpHost=s.value("smtp/host").toString();
	//mailBoxRootFolder->setText(s.value("mailBoxRootFolder", QDesktopServices::storageLocation(QDesktopServices::DataLocation)).toString());
}

void MainWinImpl::sendMail(Mail *m)
{
	qDebug()<<smtp->isRunning();
	if(!smtp->isRunning()) smtp->start();
	qDebug()<<"send Mail";
	smtp->sendMail(m,true);
}
