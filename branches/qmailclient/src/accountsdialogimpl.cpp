#include <QtCore>
#include <QtGui>

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "accountsdialogimpl.h"

AccountsDialogImpl::AccountsDialogImpl(QWidget *parent) 
	: QDialog(parent)
{
	setupUi(this);
	QSettings s(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	QStringList grouplist = s.childGroups();
	for (int i = 0; i < grouplist.size(); i++) {
		qDebug() << grouplist[i];
	}
	//connect(this,SIGNAL(accepted()),this,SLOT(writeSettings()));
}

AccountsDialogImpl::~AccountsDialogImpl()
{
}

void AccountsDialogImpl::on_pbAdd_clicked()
{
	qDebug() << "Add clicked";
}

void AccountsDialogImpl::on_pbEdit_clicked()
{
	qDebug() << "Edit clicked";
}

void AccountsDialogImpl::on_pbDel_clicked()
{
	qDebug() << "Del clicked";
}