#ifndef __ACCOUNTSDIALOGIMPL_H__
#define __ACCOUNTSDIALOGIMPL_H__

#include "ui_AccountsDialog.h"

#include <QSettings>
#include <QtGui>
#include <QtSql>

class AccountsDialogImpl : public QDialog, public Ui::AccountsDialog {
	Q_OBJECT
	public:
		AccountsDialogImpl(QWidget * parent=0);
		~AccountsDialogImpl();
		void	setModel(QSqlTableModel *);
	private slots:
		void on_pbAdd_clicked();
		void on_pbEdit_clicked();
		void on_pbDel_clicked();
	private:
		QSqlTableModel		*model;
};

#endif // __ACCOUNTSDIALOGIMPL_H__
