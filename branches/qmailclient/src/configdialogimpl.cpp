/*
 * 1. split by lines
 * 2. each line - split by separator
 * 3. to QMap
 * QSettings s(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
 */ 


#include <QtCore>
#include <QtGui>

#include "configdialogimpl.h"

ConfigDialogImpl::ConfigDialogImpl(QSqlDatabase *d, int i, QWidget *parent) 
	: QDialog(parent), db(d), id(i)
{
	setupUi(this);
	if (id) {
		QSqlQuery q(QString("SELECT name, options FROM account WHERE id=%1").arg(QString().number(id)), *db);
		if (q.next()) {
			leTitle->setText(q.value(0).toString());
			QMap<QString, QString> map = fromString(q.value(1).toString());
			// general
			leName->setText(map.value("name",""));
			leMail->setText(map.value("mail",""));
			cbService->setCurrentIndex(map.value("service", "0").toInt());
			cbTransport->setCurrentIndex(map.value("transport", "0").toInt());
			// POP3
			switch (cbService->currentIndex()) {
				case 1:	// pop3
					lePOP3Host->setText(map.value("pop3_host",""));
					sbPOP3Port->setValue(map.value("pop3_port", "110").toInt());
					lePOP3Login->setText(map.value("pop3_login",""));
					lePOP3Password->setText(map.value("pop3_password",""));
					cbPOP3Crypto->setCurrentIndex(map.value("pop3_crypto","0").toInt());
					cbPOP3Auth->setCurrentIndex(map.value("pop3_auth","0").toInt());
					break;
				case 2:	// imap
					break;
				case 3:	// maildir
					leMailDirFolder->setText(map.value("maildir_folder", QDesktopServices::storageLocation(QDesktopServices::DataLocation)));
					break;
				default:
					break;
			}
			switch (cbTransport->currentIndex()) {
				case 1:	// smtp
					leSMTPHost->setText(map.value("smtp_host",""));
					sbSMTPPort->setValue(map.value("smtp_port", "25").toInt());
					leSMTPLogin->setText(map.value("smtp_login",""));
					leSMTPPassword->setText(map.value("smtp_password",""));
					cbSMTPCrypto->setCurrentIndex(map.value("smtp_crypto","0").toInt());
					cbSMTPAuth->setCurrentIndex(map.value("smtp_auth","0").toInt());
					break;
				case 2:	// sendmail
					break;
				default:
					break;
			}
		}
	}
	connect(this,SIGNAL(accepted()), this, SLOT(writeSettings()));
}

ConfigDialogImpl::~ConfigDialogImpl() {
}

void ConfigDialogImpl::on_tbMailDirFolder_clicked() {
	QString folder=QFileDialog::getExistingDirectory(this, tr("Select the root path of the maildir"), leMailDirFolder->text());
	if(folder != "")
		leMailDirFolder->setText(folder);
}

void ConfigDialogImpl::writeSettings() {
	QMap<QString, QString> map;
	map["name"] = leName->text();
	map["mail"] = leMail->text();
	map["service"] = QString().number(cbService->currentIndex());
	map["transport"] = QString().number(cbTransport->currentIndex());
	switch (cbService->currentIndex()) {
		case 1:	// pop3
			map["pop3_host"] = lePOP3Host->text();
			map["pop3_port"] = QString().number(sbPOP3Port->value());
			map["pop3_login"] = lePOP3Login->text();
			map["pop3_password"] = lePOP3Password->text();
			map["pop3_crypto"] = QString().number(cbPOP3Crypto->currentIndex());
			map["pop3_auth"] = QString().number(cbPOP3Auth->currentIndex());
			break;
		case 2:	// imap
			break;
		case 3:	// maildir
			map["maildir_folder"] = leMailDirFolder->text();
			break;
		default:
			break;
	}
	switch (cbTransport->currentIndex()) {
		case 1:	// smtp
			map["smtp_host"] = leSMTPHost->text();
			map["smtp_port"] = QString().number(sbSMTPPort->value());
			map["smtp_login"] = leSMTPLogin->text();
			map["smtp_password"] = leSMTPPassword->text();
			map["smtp_crypto"] = QString().number(cbSMTPCrypto->currentIndex());
			map["smtp_auth"] = QString().number(cbSMTPAuth->currentIndex());
			break;
		case 2:	// sendmail
			break;
		default:
			break;
	}
	QSqlQuery q(*db);
	if (id)
		q.exec(QString("UPDATE account SET name='%1', options='%2' WHERE id=%3").arg(leTitle->text(), toString(map), QString().number(id)));
	else
		q.exec(QString("INSERT INTO account (name, options) VALUES ('%1', '%2')").arg(leTitle->text(), toString(map)));
}

QMap<QString, QString> ConfigDialogImpl::fromString(QString s) {
	QMap<QString, QString> map;
	QStringList sl = s.split("\n"), sll;
	QStringListIterator sli(sl);
	while (sli.hasNext()) {
		QString s1 = sli.next();
		map[s1.section('=', 0, 0)] = s1.section('=', 1);
	}
	return map;
}

QString ConfigDialogImpl::toString(QMap<QString, QString> map) {
	QString s;
	QMapIterator<QString, QString> i(map);
	while (i.hasNext()) {
		i.next();
		if (s.size())
			s += "\n";
		s = s + i.key() + "=" + i.value() + "\n";
	}
	return s;
}
