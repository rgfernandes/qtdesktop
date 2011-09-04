/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#include <QtDebug> //to use qWarning and qDebug messages
#include <QFileDialog>

#include "settings.h"
#include "config.h"

//-------------------------------------------------
AppSettings::AppSettings(QWidget *parent)
	 : QDialog(parent)
{
	 ui.setupUi(this);
	 connect(ui.PBExternalEditor, SIGNAL(clicked()), this, SLOT(chooseEditor()));
	 connect(ui.PBExternalBrowser, SIGNAL(clicked()), this, SLOT(chooseBrowser()));
	 connect(ui.PBExternalArchiver, SIGNAL(clicked()), this, SLOT(chooseArchiver()));
	 connect(ui.PBBackupDir, SIGNAL(clicked()), this, SLOT(chooseBackupDir()));
	 connect(ui.PBFontSettings, SIGNAL(clicked()), this, SLOT(showFontSettingsDialog()));
}

//-------------------------------------------------
void AppSettings::reject()
{
	QWidget::hide();  //close dialog
}

//-------------------------------------------------
void AppSettings::accept()
{
	apply();
	QWidget::hide();  //close dialog
}

//-------------------------------------------------
void AppSettings::apply()
{
	Config::configuration()->setContentsAdditionalView(ui.CBContentsAdditionalView->isChecked());
	emit showContentsAV(ui.CBContentsAdditionalView->isChecked());
	Config::configuration()->setShowSubItemsTitle(ui.CBShowSubItemsTitle->isChecked());
	emit showContentsAVHeader(ui.CBShowSubItemsTitle->isChecked());
	Config::configuration()->setAutoCollapse(ui.CBAutoCollapse->isChecked());
	Config::configuration()->setExternalEditor(ui.LEExternalEditor->text());
	Config::configuration()->setExternalBrowser(ui.LEExternalBrowser->text());
	Config::configuration()->setExternalArchiver(ui.LEExternalArchiver->text());
	Config::configuration()->setExternalArchiverOptions(ui.LEExternalArchiverOptions->text());
	Config::configuration()->setBackupDir(ui.LEBackupDir->text());
	Config::configuration()->setAppLogLevel(ui.SBAppLogLevel->value());
	Config::configuration()->setPrjLogLevel(ui.SBPrjLogLevel->value());
	Config::configuration()->setItemAutoProperties(ui.CBItemAutoProperties->isChecked());
	Config::configuration()->setAcceptDropImages(ui.CBAcceptDropImages->isChecked());
}

//-------------------------------------------------
void AppSettings::set()
{
	ui.CBContentsAdditionalView->setChecked(Config::configuration()->ContentsAdditionalView());
	ui.CBShowSubItemsTitle->setChecked(Config::configuration()->ShowSubItemsTitle());
	ui.CBAutoCollapse->setChecked(Config::configuration()->AutoCollapse());
	ui.CBItemAutoProperties->setChecked(Config::configuration()->ItemAutoProperties());
	ui.CBAcceptDropImages->setChecked(Config::configuration()->AcceptDropImages());
	ui.LEExternalEditor->setText(Config::configuration()->ExternalEditor());
	ui.LEExternalBrowser->setText(Config::configuration()->ExternalBrowser());
	ui.LEExternalArchiver->setText(Config::configuration()->ExternalArchiver());
	ui.LEExternalArchiverOptions->setText(Config::configuration()->ExternalArchiverOptions());
	ui.LEBackupDir->setText(Config::configuration()->BackupDir());
	ui.SBAppLogLevel->setValue(Config::configuration()->AppLogLevel());
	ui.SBPrjLogLevel->setValue(Config::configuration()->PrjLogLevel());
}

//-------------------------------------------------
void AppSettings::chooseEditor()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Choose editor"),
							   Config::configuration()->AppDir(), tr("All Files (*)"));
	if (!fn.isEmpty()){
		ui.LEExternalEditor->setText(fn);
	}
}

//-------------------------------------------------
void AppSettings::chooseBrowser()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Choose browser"),
							   Config::configuration()->AppDir(), tr("All Files (*)"));
	if (!fn.isEmpty()){
		ui.LEExternalBrowser->setText(fn);
	}
}

//-------------------------------------------------
void AppSettings::chooseArchiver()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Choose archiver"),
							   Config::configuration()->AppDir(), tr("All Files (*)"));
	if (!fn.isEmpty()){
		ui.LEExternalArchiver->setText(fn);
	}
}

//-------------------------------------------------
void AppSettings::chooseBackupDir()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose where to store backup archives"),
							   Config::configuration()->BackupDir(),
							   QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()){
		ui.LEBackupDir->setText(dir);
	}
}

//-------------------------------------------------
void AppSettings::showFontSettingsDialog()
{
	Config *config = Config::configuration();
	FontSettings settings = config->fontSettings();

	{ // It is important that the dialog be deleted before UI mode changes.
		FontSettingsDialog dialog;
		if (!dialog.showDialog(&settings))
			return;
	}
	config->setFontPointSize(settings.browserFont.pointSizeF());
	config->setFontSettings(settings);

	emit updateApplicationFontSettings(settings);
}
