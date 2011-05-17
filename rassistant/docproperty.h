/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef __DOCPROPERTY_H__
#define __DOCPROPERTY_H__

#include "ui_docproperty.h"

//===================== class DocProperties =====================
class DocProperties : public QDialog
{
     Q_OBJECT

public:
	DocProperties(QWidget *parent = 0);
	void setTitle(QString title);
	void setFileName(QString fn);
	
signals:
	void updateTitle(QString title);
	void updateItemFN(QString oldFN);

public slots:
	void accept();
	void reject();	
	void fileRenamed(QString newFN);

private slots:
	void fileRename();

private:	
	Ui::DocProperty ui;
	
}; // class LinkProperties

#endif // __DOCPROPERTY_H__
