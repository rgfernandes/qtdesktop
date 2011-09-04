/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef __AUTOITEMS_H__
#define __AUTOITEMS_H__

#include "ui_autoitems.h"

//==================== class AutoItems ====================
class AutoItems : public QDialog
{
     Q_OBJECT

public:
	AutoItems(QWidget *parent = 0);
	
	void activateTab(int n);
	void setMaxCounter();
	void setMaxCounter(int n);
	void setCounterType(int n); //set which counter to use: numbers, roman numbers or letters
	void setYears(int yFrom, int yTo);
	void setPeriods(QDate dFrom, QDate dTo);

signals:
	void cloneItem(QTreeWidgetItem *item, bool subItem);
	void cloneDone();
     
public slots:
	void applyCounter(int n);
	void CreateItems();
	
private slots:
	void Apply(); 		//apply changes and close dialog
	void closeDialog(); //close dialog without changes
	//procedures for tab "Counter"
	void changeStartToolTip(QString str);
	void changeEndToolTip(QString str);
	void changeCase(int state);
	void changeFormat(int n); //apply special date format

private:
	Ui::AutoItems ui;
	QString strAlphabet;	// Alphabet of the language user use
	
	void putItem(QTreeWidgetItem *i, QString str, QString link);
	void putItem(QTreeWidget *tw, QString str, QString link); 
	void createFile(QTreeWidgetItem *i);
	void CreateItemsDate();
	void CreateItemsCounter();	
	QString CreateFileName(QString str);
	QString getToolTip(QString str);
	QString IntToCounter(int n);

}; // class AutoItems

#endif // __AUTOITEMS_H__
