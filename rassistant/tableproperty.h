/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef __TABLEPROPERTY_H__
#define __TABLEPROPERTY_H__

#include <QTextTable>

#include "ui_tableproperty.h"

//===================== class TableProperties ===========================

class TableProperties : public QDialog
{
     Q_OBJECT

public:
	TableProperties(QWidget *parent = 0);

signals:
	void createTable(int rows, int colums, QTextTableFormat tableFormat);
	void updateTable(int rows, int colums, QTextTableFormat tableFormat);
     
public slots:
	void accept();
	void reject();
	void setProperties(int rows, int colums, QTextTableFormat tableFormat, bool newTable);
	
private slots:
	void apply();
	void changeColorTable();
	void changeColorBorder();
	
private:
	void getTableFormat();
	
	Ui::TableProperty ui;
	bool modeNewTable;
	QTextTableFormat tFormat;
	QColor colorTable, colorBorder;
 
}; // class TableProperties

#endif // __TABLEPROPERTY_H__
