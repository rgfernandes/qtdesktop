/*
 * 
 */

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include "mainwindowimpl.h"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	// <settings>
	QCoreApplication::setApplicationName("qtarc");
	QCoreApplication::setApplicationVersion("0.0.1");
	QCoreApplication::setOrganizationName("TI_Eugene");
	QCoreApplication::setOrganizationDomain("eap.su");
	// </settings>
	// <tr>
	QTranslator appTranslator;
	QString trpath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);	// /usr/share/qt4/translations
	QString trfile = QCoreApplication::applicationName() + "_" + QLocale::system().name().left(2);
	if (not QFile::exists(trpath + QDir::separator() + trfile + ".qm"))
		trpath = app.applicationDirPath() + QDir::separator() + "l10n";
	appTranslator.load(trpath + QDir::separator() + trfile);
	app.installTranslator(&appTranslator);
	// </tr>
	MainWindowImpl win;
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
