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

#ifndef VCARDMEMO_H
#define VCARDMEMO_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>

#include "VCardEntry.h"
#include "VCardTextedit.h"
#include "VCardLabel.h"
#include "VCardConst.h"

class VCardMemo : public VCardEntry
{
	Q_OBJECT

	public:
		VCardMemo(bool mode, QWidget *parent = 0);
		QString type();
		void addDeleteButton(QToolButton *button);
		void setText(const QString &text);
		void setFont(QFont font);
		QString text();

	protected:
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	signals:
		void mouseOver();
		void mouseOut();

	private slots:
		void setLabelEdit();
		void setLabelRead();

	private:
		bool m_mode, m_editable;
		QLabel *label;
		VCardLabel *labelText;
		VCardTextedit *editField;

};

#endif // VCARDMEMO_H
