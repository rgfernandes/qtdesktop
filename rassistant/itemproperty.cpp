/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

#include "itemproperty.h"
#include "config.h"
#include "pcommon.h"

//====================== class ItemProperties ============================
ItemProperties::ItemProperties(QWidget *parent)
     : QDialog(parent)
{
     ui.setupUi(this);
     
     connect(ui.BIcon, SIGNAL(clicked()), this, SLOT(chooseIcon()) );
	 connect(ui.BOpenFileDialog, SIGNAL(clicked()), this, SLOT(chooseFile()) );
	 connect(ui.PBRename, SIGNAL(clicked()), this, SLOT(fileRename()));
}

//-------------------------------------------------
void ItemProperties::chooseIcon()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Choose icon"),
                               Config::configuration()->ImgDir(), tr("PNG (*.png);;JPEG (*.jpg);;All Files (*)"));
    if (!fn.isEmpty()){
		ui.EIcon->setText(fn);
		ui.BIcon->setIcon(QIcon(fn));
	}
}

//-------------------------------------------------
void ItemProperties::chooseFile()
{
	QString *s= new QString();
    QString fn = QFileDialog::getSaveFileName(this, tr("Add file"),
                               Config::configuration()->CurPrjDir(), tr("HTML (*.htm *.html);;All Files (*)"), s, QFileDialog::DontConfirmOverwrite );
    if (!fn.isEmpty()){
    	QFileInfo fi(fn);
    	if (fi.suffix().isEmpty()) 
    		fn += ".html";    		
		ui.LEFile->setText(fn);
   	}
}

//-------------------------------------------------
void ItemProperties::accept()
{
	QString s = "";  //holds list of errors
	bool er = false;
	validProperties = false;
	if (ui.ETitle->text().isEmpty()){
		s = tr("- Please enter a title.\n");
		er = true;
	}
	if ((!ui.EIcon->text().isEmpty()) && (!QFile::exists(ui.EIcon->text()))){
		s += tr("- Please enter a correct path to the icon, or leave this field empty if no icon needed.\n");
		er = true;
	}
	if (ui.LEFile->text().isEmpty()){
		s += tr("- Content item needs a file to be linked to. You can use an existed file or set a name for the new one.");
		er = true;
	}
	if (er){
		QMessageBox::critical(this, tr("Item property error"), s);	
	}else{
		createEmptyHtml(ui.LEFile->text(), ui.ETitle->text());
		QFile file(ui.LEFile->text());
		if (file.exists()){	//file does exist we can proceed with setting item properties
			itemTitle= ui.ETitle->text(); 	//!+! get title from document
			itemFile = urlifyFileName(ui.LEFile->text());
			itemIconFN = ui.EIcon->text();
			validProperties = true;
			if (insertMode){
				emit insertContentsItem(itemTitle, itemFile, itemIconFN);
			}else{
				emit updateContentsItem(itemTitle, itemFile, itemIconFN);
			}
			QWidget::hide();  //close dialog
		}
	}
}

//-------------------------------------------------
void ItemProperties::reject()
{
	validProperties = false;
	QWidget::hide();  //close dialog
}

//-------------------------------------------------
void ItemProperties::setProperties(QString title, QString fileName, QString iconFN, bool mode)
{
	itemTitle= title;
	itemFile = fileName;
	itemIconFN = iconFN;
	insertMode = mode; 
	
	ui.ETitle->setText(title);
	ui.LEFile->setText(fileName);
	ui.EIcon->setText(iconFN);
	if (!iconFN.isEmpty())
		ui.BIcon->setIcon(QIcon(iconFN));
	else 
		ui.BIcon->setIcon(QIcon());
}

//-------------------------------------------------
void ItemProperties::fileRename()
{
	emit updateItemFN(unurlifyFileName( ui.LEFile->text() ));
}

//-------------------------------------------------
void ItemProperties::fileRenamed(QString newFN)
{
	ui.LEFile->setText(newFN);
}
