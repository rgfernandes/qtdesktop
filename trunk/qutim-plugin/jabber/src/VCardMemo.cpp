/*
Copyright (c) 2009 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "VCardMemo.h"

VCardMemo::VCardMemo(bool mode, QWidget *parent) : VCardEntry(parent)
{
	m_mode = mode;
	m_editable = false;
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setMargin(0);
	setLayout(layout);
	QHBoxLayout *labelLayout = new QHBoxLayout();
	labelLayout->setMargin(0);
	layout->addLayout(labelLayout);
	label = new QLabel("About");
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	label->setSizePolicy(sizePolicy);
	labelLayout->addWidget(label);
	QSpacerItem *horisontalSpacer = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
	labelLayout->addSpacerItem(horisontalSpacer);
	labelText = new VCardLabel(m_mode);
	labelText->setWordWrap(true);
	connect(labelText, SIGNAL(editMode()), this, SLOT(setLabelEdit()));
	layout->addWidget(labelText);
	if (m_mode)
	{
		editField = new VCardTextedit();
		connect(editField, SIGNAL(readMode()), this, SLOT(setLabelRead()));
		layout->addWidget(editField);
		editField->setVisible(false);
	}
}

void VCardMemo::setLabelEdit()
{
	m_editable = true;
	emit mouseOut();
	if (labelText->text() == VCardConst::emptyString())
		editField->setPlainText("");
	else
		editField->setPlainText(labelText->text());
	editField->setVisible(true);
	labelText->setVisible(false);
}

void VCardMemo::setLabelRead()
{
	m_editable = false;
	if (editField->toPlainText() == "")
		labelText->setText(VCardConst::emptyString());
	else
		labelText->setText(editField->toPlainText());
	editField->setVisible(false);
	labelText->setVisible(true);
}

void VCardMemo::addDeleteButton(QToolButton *button)
{
	//button->setFixedHeight(labelText->height());
	//labelLayout->insertWidget(1, button);
	button->setParent(this);
	int x = label->pos().x()+label->width()+6;
	button->setGeometry(QRect(QPoint(x, label->pos().y()), button->size()));
	button->setVisible(true);
}

void VCardMemo::enterEvent(QEvent *event)
{
	if (m_mode && !m_editable)
		emit mouseOver();
}

void VCardMemo::leaveEvent(QEvent *event)
{
	if (m_mode)
		emit mouseOut();
}

void VCardMemo::setText(const QString &text)
{
	labelText->setText(text);
}

void VCardMemo::setFont(QFont font)
{
	labelText->setFont(font);
}

QString VCardMemo::text()
{
	return labelText->text();
}

QString VCardMemo::type()
{
	return "memo";
}
