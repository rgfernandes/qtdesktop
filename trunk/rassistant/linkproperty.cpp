/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#include <QFileDialog>
#include "linkproperty.h"
#include "config.h"

//====================== class ItemProperties ============================
LinkProperties::LinkProperties(QWidget *parent)
     : QDialog(parent)
{
     ui.setupUi(this);
     
     connect(ui.BRemove, SIGNAL(clicked()), this, SLOT(RemoveLink()) );
     connect(ui.BChooseFile, SIGNAL(clicked()), this, SLOT(chooseFile()) );  
}

void LinkProperties::RemoveLink()
{
	emit removeLink();
	QWidget::hide();  //close dialog	
}

void LinkProperties::chooseFile()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Choose file to link to"),
                               Config::configuration()->CurPrjDir(), tr("HTML (*.htm *.html);;All Files (*)"));
    if (!fn.isEmpty()){
    	ui.ELinkLocation->setText(fn);
   	}
}

void LinkProperties::setProperties(QString lText, QString lLocation)
{
	linkText = lText;
	linkLocation = lLocation;
	
	ui.ELinkText->setText(lText);
	ui.ELinkLocation->setText(lLocation);
}

void LinkProperties::reject()
{
	validProperties = false;
	QWidget::hide();  //close dialog	
}

void LinkProperties::accept()
{
	linkText = ui.ELinkText->text();
	linkLocation = ui.ELinkLocation->text();
	validProperties = true;
	if ( ui.ELinkLocation->text().isEmpty() )
		emit removeLink();
	else
		emit updateLink(linkText, linkLocation);	
	QWidget::hide();  //close dialog		
}
