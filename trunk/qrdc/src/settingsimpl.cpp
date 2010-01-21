#include "settingsimpl.h"
//
DialogSettingsImpl::DialogSettingsImpl( QWidget * parent, Qt::WFlags f ) 
	: QDialog(parent, f) {
	// TODO
	setupUi(this);
	setfile = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	terminal = setfile->value("term").toString();
}

void	DialogSettingsImpl::Go(void) {
	leTerm->setText(terminal);
	if (exec() == QDialog::Accepted) {
		terminal = leTerm->text();
		setfile->setValue("term", terminal);
	}
}

//
