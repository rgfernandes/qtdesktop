#ifndef CONFIGDIALOGIMPL_H
#define CONFIGDIALOGIMPL_H
//
#include "ui_ConfigDialog.h"
#include <QSettings>
#include <QDir>
#include <QFileDialog>
//
class ConfigDialogImpl : public QDialog, public Ui::ConfigDialog
{
Q_OBJECT
public:
	ConfigDialogImpl(QWidget *parent=0);
	~ConfigDialogImpl();
private slots:
	void writeSettings();
	void on_tbMailDirFolder_clicked();
};
#endif
