/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#include <QInputDialog>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>

#include "docproperty.h"
#include "pcommon.h"
#include "config.h"

//===================== class DocProperties =====================
DocProperties::DocProperties(QWidget *parent)
     : QDialog(parent)
{
	 ui.setupUi(this);
	 connect(ui.PBRename, SIGNAL(pressed()), this, SLOT(fileRename()));
}

//-------------------------------------------------
void DocProperties::setTitle(QString title)
{
	ui.ETitle->setText(title);
}

//-------------------------------------------------
void DocProperties::setFileName(QString fn)
{
	ui.LEFileName->setText(fn);
}

//-------------------------------------------------
void DocProperties::reject()
{
	QWidget::hide();  //close dialog	
}

//-------------------------------------------------
void DocProperties::accept()
{
	emit updateTitle( ui.ETitle->text() );	
	QWidget::hide();  //close dialog		
}

//-------------------------------------------------
void DocProperties::fileRename()
{
	emit updateItemFN(unurlifyFileName( ui.LEFileName->text() ));
}

//-------------------------------------------------
void DocProperties::fileRenamed(QString newFN)
{
	ui.LEFileName->setText(newFN);
}

