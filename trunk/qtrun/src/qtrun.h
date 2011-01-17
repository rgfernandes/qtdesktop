/*
Copyright 2009 Eugene A. Pivnev

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef QTRUN_H
#define QTRUN_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

class	QtRun : public QDialog {
	Q_OBJECT
public:
	QGridLayout		*gridLayout;
	QLineEdit		*lineEdit;
	QPushButton		*pushButton;
	QDialogButtonBox	*buttonBox;
	QString			path;

	explicit QtRun(QWidget *parent = 0);
public slots:
	void accept();
	void selectrun();
};

#endif
