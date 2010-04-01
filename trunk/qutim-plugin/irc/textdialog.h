/*
		textDialogDialog

    Copyright (c) 2008 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H
//
#include <QDialog>
#include "ui_textdialog.h"
//
class textDialog : public QDialog, public Ui::textDialogClass
{
Q_OBJECT
public:
	textDialog( QWidget * parent = 0, Qt::WFlags f = 0 );
	QString getText() { return editText->toPlainText(); }
	void setText(QString text) { editText->setPlainText(text); }
	void setTitle(QString text) { this->setWindowTitle(text); }
private slots:
};
#endif





