/*
jDataForm.cpp

Copyright (c) 2008 by Kostin Dmitrij <kostindima@gmail.com>
				  2009 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JDATAFORM_H
#define JDATAFORM_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>

#include <gloox/client.h>
#include <gloox/jid.h>
#include <gloox/dataform.h>
#include <gloox/dataformfield.h>
#include <gloox/dataformfieldcontainer.h>

using namespace gloox;

class jDataForm : public QWidget
{
	Q_OBJECT
public:
	jDataForm(const DataForm * form, bool twocolumn = true, QWidget *parent = 0);
	~jDataForm();
	DataForm *getDataForm();

private:
	DataForm *forma;

	QList<QLineEdit *> line_edits;
	QList<QCheckBox *> chk_boxes;
	QList<QComboBox *> combo_boxes;
	QList<QTextEdit *> text_edits;

};

#endif // JDATAFORM_H
