#include <QtCore>
#include <QtGui>

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "accountsdialogimpl.h"

AccountsDialogImpl::AccountsDialogImpl(QWidget *parent) 
	: QDialog(parent) {
	setupUi(this);
}

AccountsDialogImpl::~AccountsDialogImpl() {
}

void	AccountsDialogImpl::setModel(QSqlTableModel *m) {
	model = m;
	tvAccounts->setModel(model);
	tvAccounts->resizeColumnsToContents();
	//tvAccounts->setCurrentIndex(tvAccounts->model()->index(0, 1));
	qDebug() << "Model set ok";
}

void AccountsDialogImpl::on_pbAdd_clicked() {
	qDebug() << "Add clicked";
}

void AccountsDialogImpl::on_pbEdit_clicked() {
	qDebug() << "Edit clicked";
}

void AccountsDialogImpl::on_pbDel_clicked() {
	qDebug() << "Del clicked";
}
