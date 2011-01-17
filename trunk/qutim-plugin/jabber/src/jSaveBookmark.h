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

#ifndef JSAVEBOOKMARK_H
#define JSAVEBOOKMARK_H

#include <QDialog>
#include "jAccount.h"
#include "ui_jSaveBookmark.h"

class jSaveBookmark : public QDialog
{
	Q_OBJECT

	public:
		jSaveBookmark(jAccount *jabber_account, const QString &conference, const QString &nick, QString &password);

	private slots:
		void on_saveButton_clicked();

	private:
		Ui::SaveWidget ui;
		jAccount *m_jabber_account;
};

#endif // JSAVEBOOKMARK_H
