#ifndef __ACCOUNTSDIALOGIMPL_H__
#define __ACCOUNTSDIALOGIMPL_H__

#include "ui_AccountsDialog.h"

#include <QSettings>

class AccountsModel : public QAbstractListModel {
	public:
		AccountsModel(QSettings *, QObject * parent = 0);
		~AccountsModel();
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		void reloadsettings() const; 
		//headerData()
	private:
		QSettings *settings;
		QStringList lv;
};

class AccountsDialogImpl : public QDialog, public Ui::AccountsDialog {
Q_OBJECT
public:
	AccountsDialogImpl(QWidget * parent=0);
	~AccountsDialogImpl();
private slots:
	void on_pbAdd_clicked();
	void on_pbEdit_clicked();
	void on_pbDel_clicked();
private:
	QSettings *settings;
};

#endif // __ACCOUNTSDIALOGIMPL_H__
