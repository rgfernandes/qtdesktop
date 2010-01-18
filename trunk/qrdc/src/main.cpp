#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtSql>
#include "mainwindowimpl.h"
//
bool	createConnection(QMainWindow *mw)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "qrdc.db");
	if (!db.open())
	{
		QMessageBox::critical(mw,
			QMainWindow::tr("Opening database error"),
			QMainWindow::tr("Unable to establish a database connection."),
			QMessageBox::Cancel, QMessageBox::NoButton);
		return false;
	}
	else
	{
		if (db.tables().isEmpty())
		{
			
		}
	}
	return true;
}

int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	// <tr>
	QTranslator appTranslator;
	QString trpath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);	// /usr/share/qt4/translations
	QString trfile = QString("qrdc_") + QLocale::system().name().left(2);
	if (not QFile::exists(trpath + QDir::separator() + trfile))
		trpath = app.applicationDirPath() + QDir::separator() + "i18n";
	appTranslator.load(trpath + QDir::separator() + trfile);
	app.installTranslator(&appTranslator);
	// </tr>
	MainWindowImpl win;
	win.setModels();
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
