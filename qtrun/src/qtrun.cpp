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

#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

#include "qtrun.h"

QtRun::QtRun(QWidget *parent) :
	QDialog(parent)
{
	this->setWindowTitle(tr("Run application"));
	// 1. create widgets
	gridLayout = new QGridLayout(this);
	lineEdit = new QLineEdit(this);
	pushButton = new QPushButton(tr("Browse"), this);
	buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	// 2. layout
	gridLayout->addWidget(lineEdit, 0, 0, 1, 1);
	gridLayout->addWidget(pushButton, 0, 1, 1, 1);
	gridLayout->addWidget(buttonBox, 1, 0, 1, 2);
	// 3. actions
	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(selectrun()));
}

void	QtRun::selectrun(void) {
	QString file = QFileDialog::getOpenFileName(this, tr("Open executable"), lineEdit->text(), tr("All (*.*)"));
	if (!file.isEmpty())
		lineEdit->setText(file);
}

void	QtRun::accept(void) {
	if (!lineEdit->text().isEmpty())
		path = lineEdit->text();
	QDialog::accept();
}
