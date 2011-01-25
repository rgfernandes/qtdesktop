#include <QtCore>
#include <QtGui>

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "accountsdialogimpl.h"

AccountsModel::AccountsModel(QSettings * s, QObject * parent) :
	settings(s) {
	lv = settings->childGroups();
}

AccountsModel::~AccountsModel() {
}

void AccountsModel::reloadsettings() const {
	//lv = settings->childGroups();
}

int AccountsModel::rowCount(const QModelIndex & parent) const {
	reloadsettings();
	return lv.size();
}

QVariant AccountsModel::data(const QModelIndex & index, int role) const {
	if (role == Qt::DisplayRole) {
		//qDebug() << index.row();
		return QVariant(lv[index.row()]);
	} else {
		return QVariant();
	}
}
// ====
AccountsDialogImpl::AccountsDialogImpl(QWidget *parent) 
	: QDialog(parent) {
	setupUi(this);
	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	AccountsModel *am = new AccountsModel(settings);
	lvAccounts->setModel(am);
	/*QStringList grouplist = s.childGroups();
	for (int i = 0; i < grouplist.size(); i++) {
		qDebug() << grouplist[i];
	}*/
	//connect(this,SIGNAL(accepted()),this,SLOT(writeSettings()));
}

AccountsDialogImpl::~AccountsDialogImpl() {
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
