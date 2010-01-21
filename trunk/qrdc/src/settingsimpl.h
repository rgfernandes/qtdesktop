#ifndef SETTINGSIMPL_H
#define SETTINGSIMPL_H
//
#include <QtCore>
#include "ui_settings.h"
//
class DialogSettingsImpl : public QDialog, public Ui::DialogSettings
{
Q_OBJECT
public:
	DialogSettingsImpl(QWidget * parent = 0, Qt::WFlags f = 0);
	void		Go(void);
	QString		terminal;
private:
	QSettings	*setfile;
};

#endif
