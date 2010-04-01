/*
	ClientConfigPage

	Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "clientconfigpage.h"
#include "ui_clientconfigpage.h"
#include <qutim/iconmanagerinterface.h>
#include <QFileDialog>
#include <QTextCodec>

namespace HistoryManager {

ClientConfigPage::ClientConfigPage(HistoryManagerWindow *parent) :
    QWizardPage(parent),
    m_ui(new Ui::ClientConfigPage)
{
    m_ui->setupUi(this);
	m_parent = parent;
	registerField("historypath", m_ui->filenameEdit);
	QList<QByteArray> codecs = QTextCodec::availableCodecs();
	qSort(codecs);
	QTextCodec *locale_codec = QTextCodec::codecForLocale();
	foreach(const QByteArray &codec, codecs)
	{
		QString codec_str = QString::fromLatin1(codec);
		if(codec_str.startsWith("windows"))
			codec_str[0] = 'W';
		else if(codec_str == "System")
			codec_str = tr("System");
		m_ui->codepageBox->addItem(codec_str, codec);
	}
	m_ui->codepageBox->setCurrentIndex(m_ui->codepageBox->findData(locale_codec ? locale_codec->name() : "UTF-8"));
	connect(m_ui->filenameEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
	setTitle(tr("Configuration"));
	setCommitPage(true);
	setButtonText(QWizard::CommitButton, m_parent->nextStr());
}

ClientConfigPage::~ClientConfigPage()
{
    delete m_ui;
}

void ClientConfigPage::initializePage()
{
	m_valid = false;
	if(m_valid_pixmap.isNull())
	{
		m_valid_pixmap = qutim_sdk_0_2::Icon("apply").pixmap(16);
		m_invalid_pixmap = qutim_sdk_0_2::Icon("cancel").pixmap(16);
	}
	m_ui->validIcon->setPixmap(m_invalid_pixmap);
	QString subtitle;
	if(m_parent->getCurrentClient()->chooseFile())
		subtitle = tr("Enter path of your %1 profile file.");
	else
		subtitle = tr("Enter path of your %1 profile dir.");
	subtitle.replace("%1", m_parent->getCurrentClient()->name());
	if(m_parent->getCurrentClient()->needCharset())
	{
		subtitle += " ";
		subtitle += tr("If your history encoding differs from the system one, choose the appropriate encoding for history.");
		m_ui->label_2->show();
		m_ui->codepageBox->show();
	}
	else
	{
		m_ui->label_2->hide();
		m_ui->codepageBox->hide();
	}
	QString client_specific = m_parent->getCurrentClient()->additionalInfo();
	if(!client_specific.isEmpty())
	{
		subtitle += " ";
		subtitle += client_specific;
	}
	setSubTitle(subtitle);
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+qutim_sdk_0_2::SystemsCity::ProfileName(), "hmsettings");
	settings.beginGroup("importpath");
	m_ui->filenameEdit->setText(settings.value(m_parent->getCurrentClient()->name()).toString());
	settings.endGroup();
	settings.beginGroup("charset");
	m_ui->codepageBox->setCurrentIndex(m_ui->codepageBox->findData(settings.value(m_parent->getCurrentClient()->name(), "System")));
	settings.endGroup();
	onTextChanged(m_ui->filenameEdit->text());
	m_config_list = m_parent->getCurrentClient()->config();
	for(int i = 0; i < m_config_list.size(); i++)
	{
		m_ui->formLayout->setWidget(i + 2, QFormLayout::LabelRole, m_config_list[i].first);
		m_ui->formLayout->setWidget(i + 2, QFormLayout::FieldRole, m_config_list[i].second);
	}
}

void ClientConfigPage::cleanupPage()
{
	m_valid = false;
	foreach(ConfigWidget config, m_config_list)
	{
		delete config.first;
		delete config.second;
	}
	m_config_list.clear();
}

bool ClientConfigPage::validatePage()
{
	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+qutim_sdk_0_2::SystemsCity::ProfileName(), "hmsettings");
	settings.beginGroup("importpath");
	settings.setValue(m_parent->getCurrentClient()->name(), m_ui->filenameEdit->text());
	settings.endGroup();
	QByteArray charset = m_ui->codepageBox->itemData(m_ui->codepageBox->currentIndex()).toByteArray();
	settings.beginGroup("charset");
	settings.setValue(m_parent->getCurrentClient()->name(), charset);
	settings.endGroup();
	m_parent->setCharset(charset);
	m_parent->getCurrentClient()->useConfig();
	return true;
}

bool ClientConfigPage::isComplete() const
{
	return m_valid;
}

int ClientConfigPage::nextId() const
{
	return HistoryManagerWindow::ImportHistory;
}

QString ClientConfigPage::getAppropriateFilePath(const QString &filename_)
{
	if(filename_.startsWith("~/"))
	{
		QString filename =  QDir::homePath();
		filename += QDir::separator();
		filename += filename_.mid(2);
		return filename;
	}
	else
		return filename_;
}

QString ClientConfigPage::getAppropriatePath(const QString &path_)
{
	QString path = getAppropriateFilePath(path_);
	path.replace("\\", "/");
	while(!path.isEmpty() && !QFileInfo(path).isDir())
	{
		int size = path.lastIndexOf("/");
		path.truncate(size);
	}
	return path.isEmpty() ? QDir::homePath() : path;
}

void ClientConfigPage::on_browseButton_clicked()
{
	QString path;
	if(m_parent->getCurrentClient()->chooseFile())
		path = QFileDialog::getOpenFileName(this, tr("Select path"), getAppropriatePath(m_ui->filenameEdit->text()));
	else
		path = QFileDialog::getExistingDirectory(this, tr("Select path"), getAppropriatePath(m_ui->filenameEdit->text()));
	if (!path.isEmpty())
		m_ui->filenameEdit->setText(path);
}

void ClientConfigPage::onTextChanged(const QString &filename)
{
	m_valid = m_parent->getCurrentClient()->validate(getAppropriateFilePath(filename));
	m_ui->validIcon->setPixmap(m_valid ? m_valid_pixmap : m_invalid_pixmap);
	emit completeChanged();
}

void ClientConfigPage::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

}
