#include <QApplication>
#include <QtCore>
#include <QtGui>
#include "mainwindowimpl.h"
//

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
	win.show(); 
	win.setModels();
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
