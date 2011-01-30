#include <QApplication>
#include <QTranslator>
#include <QLocale>

#include "mainwinimpl.h"
#include "mailbox.h"

bool	createConnection(QSqlDatabase &db)
{
	QString s = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QDir d(s);
	d.mkpath(s);
	db.setDatabaseName(s + QDir::separator() + "qmailclient.db");
	if (!db.open()) {
		QMessageBox::critical(0,
			QObject::tr("Opening database error"),
			QObject::tr("Unable to establish a database connection."));
		return false;
	} else {
		if (db.tables().isEmpty()) {
			bool ok = false;
			QStringList list;
			QFile file(":sql/sql/data.sql");
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("fail to open data.sql to read"));
				return false;
			} else {
				list = QString(file.readAll()).split("\n");
				file.close();
			}
			if (!list.isEmpty()) {
				QSqlQuery q;
				QListIterator<QString> itr (list);  
				while (itr.hasNext()) {
					QString s = itr.next();
					if (!s.isEmpty()) {
						if (!q.exec(s)) {
							QMessageBox::critical(0,
								QObject::tr("Creating database error"),
								QObject::tr("Unable to execute query: %1").arg(s));
							return false;
						} else
							ok = true;
					}
				}
				if (ok)
					QMessageBox::information(0,
						QObject::tr("Creating database"),
						QObject::tr("Database created OK"));

			} else {
				QMessageBox::critical(0,
				QObject::tr("Creating database error"),
				QObject::tr("Unable to read sql."));
				return false;
			}
		}
	}
	return true;
}

MailBox	*openMailBox(void) {
	MailBox *mb;
	int	transport, s_port, s_auth, t_port, t_auth;
	QString	name, mail, s_host, s_login, s_password;
	SERVICE	service;
	CRYPTO	s_crypto = CRYPTO_NONE, t_crypto = CRYPTO_NONE;
	AUTH	s_sasl = AUTH_NONE, s_apop = AUTH_NONE, t_pass = AUTH_NONE, t_sasl = AUTH_NONE;

	mb = new MailBox();
	mb->setSession();
	QSettings s(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	service =	(SERVICE) s.value("service", 0).toInt();
	transport =	s.value("transport", 0).toInt();
	name =		s.value("name", "").toString();
	mail =		s.value("mail", "").toString();
	switch (service) {
		case 1:
			s.beginGroup("pop3");
			s_host =	s.value("host", "").toString();
			s_port =	s.value("port", 110).toInt();
			s_login =	s.value("login", "").toString();
			s_password =	s.value("password", "").toString();
			s_crypto =	(CRYPTO) s.value("crypto", 0).toInt();
			s_auth =	s.value("auth", 0).toInt();
			s.endGroup();
			switch (s_auth) {
				case 1:
					s_apop = AUTH_TRY;
					break;
				case 2:
					s_apop = AUTH_REQUIRED;
					break;
				case 3:
					s_sasl = AUTH_TRY;
					break;
				case 4:
					s_apop = AUTH_TRY;
					s_sasl = AUTH_TRY;
					break;
				case 5:
					s_apop = AUTH_REQUIRED;
					s_sasl = AUTH_TRY;
					break;
				case 6:
					s_sasl = AUTH_REQUIRED;
					break;
				default:
					break;
			}
			mb->setService(service, s_host, s_port, s_login, s_password, s_crypto, s_sasl, s_apop);
			break;
		case 2:
			s.beginGroup("imap");
			s_host =	s.value("host", "").toString();
			s_port =	s.value("port", 143).toInt();
			s_login =	s.value("login", "").toString();
			s_password =	s.value("password", "").toString();
			s_crypto =	(CRYPTO) s.value("crypto", 0).toInt();
			s_auth =	s.value("auth", 0).toInt();
			s.endGroup();
			break;
		default:
			break;
	}
	//qDebug() << service;
	return mb;
}

int main(int argc, char ** argv) {
	QApplication app( argc, argv );
	
	QCoreApplication::setApplicationName("qmailclient");
	QCoreApplication::setApplicationVersion("0.0.1");
	QCoreApplication::setOrganizationName("TI_Eugene");
	QCoreApplication::setOrganizationDomain("eap.su");
	// <tr>
	QTranslator appTranslator;
	QString trpath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);	// /usr/share/qt4/translations
	QString trfile = QCoreApplication::applicationName() + "_" + QLocale::system().name().left(2);
	if (not QFile::exists(trpath + QDir::separator() + trfile + ".qm"))
		trpath = app.applicationDirPath() + QDir::separator() + "i18n";
	appTranslator.load(trpath + QDir::separator() + trfile);
	app.installTranslator(&appTranslator);
	// </tr>
	// <db>
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	if (!createConnection(db))
		return 1;
	// </db>
	MainWinImpl *win = new MainWinImpl(&db);
	//MailBox *mb = openMailBox();
	//win->setModels(&db);
	win->show();
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	int retvalue = app.exec();
	delete win;
	db.close();
	return retvalue;
}
