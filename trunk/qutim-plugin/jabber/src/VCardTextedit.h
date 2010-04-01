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

#ifndef VCARDTEXTEDIT_H
#define VCARDTEXTEDIT_H

#include <QTextEdit>

class VCardTextedit : public QTextEdit
{
	Q_OBJECT

	public:
		VCardTextedit(QWidget *parent = 0);

	protected:
		void focusOutEvent(QFocusEvent *event);

	signals:
		void readMode();

};

#endif // VCARDTEXTEDIT_H
