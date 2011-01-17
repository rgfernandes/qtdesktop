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

/* TODO: add "TypeListMulti" DataFormField type
   TODO: testing TypeListSingle
*/

#include "jDataForm.h"
#include "utils.h"

jDataForm::jDataForm(const DataForm * form, bool twocolumn, QWidget *parent) : QWidget(parent)
{
	forma = new DataForm(*form);
	forma->setType(TypeSubmit);

	QList<DataFormField*> lst = QList<DataFormField*>::fromStdList(form->fields());
	int fieldCount = lst.size();
	QGridLayout *layout = new QGridLayout();
	this->setLayout(layout);
	bool skip = false;
	QWidget *prevWidget = 0;
	for (int num = 0; num < fieldCount; num++) // Form parsing
	{
		QLabel *label = new QLabel();
		QWidget *widget = 0;
		if (lst[num]->type() == DataFormField::TypeFixed)
		{
			label->setText(utils::fromStd(lst[num]->value()));
			label->setWordWrap(true);
		}
		else if (lst[num]->type() != DataFormField::TypeBoolean)
			label->setText(utils::fromStd(lst[num]->label()));
		if (lst[num]->type()==DataFormField::TypeTextPrivate || lst[num]->type()==DataFormField::TypeTextSingle ){
			line_edits.append(new QLineEdit(this));
			line_edits.last()->setObjectName(utils::fromStd(lst[num]->name()));
			if (lst[num]->type()==DataFormField::TypeTextPrivate )
				line_edits.last()->setEchoMode(QLineEdit::Password);
			line_edits.last()->setText(utils::fromStd(lst[num]->value()));
			widget = line_edits.last();
		}
		else if (lst[num]->type()==DataFormField::TypeTextMulti )
		{
			text_edits.append(new QTextEdit(this));
			text_edits.last()->setObjectName(utils::fromStd(lst[num]->name()));
			widget = text_edits.last();
		}
		else if (lst[num]->type()==DataFormField::TypeBoolean )
		{
			chk_boxes.append(new QCheckBox(this));
			chk_boxes.last()->setObjectName(utils::fromStd(lst[num]->name()));
			widget = chk_boxes.last();
			chk_boxes.last()->setChecked(lst[num]->value()=="1");
			chk_boxes.last()->setText(utils::fromStd(lst[num]->label()));
		}
		else if (lst[num]->type()==DataFormField::TypeListSingle )
		{
			combo_boxes.append(new QComboBox(this));
			combo_boxes.last()->setObjectName(utils::fromStd(lst[num]->name()));
			combo_boxes.last()->setInsertPolicy(QComboBox::NoInsert);
			widget = combo_boxes.last();
			std::string value = lst[num]->value();
			int index = 0;
			//StringList values = lst[num]->values();
			StringMultiMap options = lst[num]->options();
			StringMultiMap::const_iterator it2 = options.begin();
			for( ; it2 != options.end(); ++it2 )
			{
				combo_boxes.last()->addItem(utils::fromStd((*it2).first), utils::fromStd((*it2).second));
				if (value == (*it2).second)
				{
					index = combo_boxes.last()->count() - 1;
				}
			}
			combo_boxes.last()->setCurrentIndex(index);
		}
		if (lst[num]->type() == DataFormField::TypeHidden)
			continue;
		if (!twocolumn || fieldCount > 5 && (num < fieldCount-1 && lst[num+1]->type() == DataFormField::TypeFixed || num && lst[num-1]->type() == DataFormField::TypeFixed || lst[num]->type() == DataFormField::TypeFixed))
				skip = false;
		if (widget)
		{
			if (fieldCount > 5 && skip)
			{
				if (label->text().isEmpty())
					layout->addWidget(widget, layout->rowCount()-1, 2, 1, 2);
				else
				{
					layout->addWidget(label, layout->rowCount()-1, 2);
					layout->addWidget(widget, layout->rowCount()-1, 3);
				}
			}
			else
			{
				if (label->text().isEmpty())
					layout->addWidget(widget, layout->rowCount(), 0, 1, 2);
				else
				{
					layout->addWidget(label, layout->rowCount(), 0);
					layout->addWidget(widget, layout->rowCount()-1, 1);
				}
			}
		}
		else
			layout->addWidget(label, layout->rowCount(), 0, 1, 2);
		skip = !skip;
	}		//Form parsing
}

jDataForm::~jDataForm()
{
	delete(forma);
	destroy(true,true); //Destroy main widget with all child
}

DataForm *jDataForm::getDataForm()
{
	//Getting values from widgets and return this
	for (int i = 0; i < line_edits.size(); ++i) {
		forma->field(utils::toStd(line_edits[i]->objectName()))->setValue(utils::toStd(line_edits[i]->text()));
	}
	for (int i = 0; i < text_edits.size(); ++i) {
		forma->field(utils::toStd(text_edits[i]->objectName()))->setValue(utils::toStd(text_edits[i]->toPlainText()));
	}
	for (int i = 0; i < chk_boxes.size(); ++i) {
		forma->field(chk_boxes[i]->objectName().toStdString())->setValue(chk_boxes[i]->isChecked()?"1":"0");
	}
	for (int i = 0; i < combo_boxes.size(); ++i) {
		int index = combo_boxes[i]->currentIndex();
		forma->field(utils::toStd(combo_boxes[i]->objectName()))->setValue(utils::toStd(combo_boxes[i]->itemData(index, Qt::UserRole).toString()));
	}
	return new DataForm(*forma);
}
