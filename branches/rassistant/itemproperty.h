/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef __ITEMPROPERTY_H__
#define __ITEMPROPERTY_H__

#include "ui_itemproperty.h"

//==================== class ItemProperties ====================
class ItemProperties : public QDialog
{
     Q_OBJECT

public:
	ItemProperties(QWidget *parent = 0);
	bool valid() { return validProperties; }

signals:
	void insertContentsItem(QString title, QString fileName, QString iconFN);
	void updateContentsItem(QString title, QString fileName, QString iconFN);
	void updateItemFN(QString oldFN);

public slots:
	void chooseIcon();
	void chooseFile();
	void accept();
	void reject();
	void setProperties(QString title, QString fileName, QString iconFN, bool mode);
	QString title()		{ return itemTitle; }	
	QString filename()	{ return itemFile; }
	QString iconFN()	{ return itemIconFN; }
	void fileRenamed(QString newFN);

private slots:
	void fileRename();

private:
	Ui::ItemProperty ui;
	
	bool insertMode; 	
	QString itemTitle, itemFile, itemIconFN;
	bool validProperties;

}; // class ItemProperties

#endif // __ITEMPROPERTY_H__
