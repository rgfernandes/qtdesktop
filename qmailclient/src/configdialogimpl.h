#ifndef CONFIGDIALOGIMPL_H
#define CONFIGDIALOGIMPL_H
//
#include "ui_ConfigDialog.h"
#include <QSettings>
#include <QDir>
#include <QMap>
#include <QFileDialog>
#include <QtSql>
//
class ConfigDialogImpl : public QDialog, public Ui::ConfigDialog
{
Q_OBJECT
public:
	ConfigDialogImpl(QSqlDatabase *, int = 0, QWidget *parent=0);
	~ConfigDialogImpl();
private slots:
	void writeSettings();
	void on_tbMailDirFolder_clicked();
private:
	QMap<QString, QString> fromString(QString);
	QString toString(QMap<QString, QString>);
	QSqlDatabase *db;
	int id;
};
#endif
