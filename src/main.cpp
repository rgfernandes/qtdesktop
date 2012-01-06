#include <QtCore>
#include <QApplication>
#include "mainwindowimpl.h"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	QCoreApplication::setApplicationName("qrss");
	QCoreApplication::setApplicationVersion("0.0.1");
	QCoreApplication::setOrganizationName("TI_Eugene");
	QCoreApplication::setOrganizationDomain("qtdesktop.org");
	// <tr>
	QTranslator appTranslator;
	QString trpath = QString("/usr/share/") + QCoreApplication::applicationName();
	QString trfile = QCoreApplication::applicationName() + "_" + QLocale::system().name().left(2);
	if (not QFile::exists(trpath + QDir::separator() + trfile + ".qm"))
		trpath = app.applicationDirPath() + QDir::separator() + "l10n";
	appTranslator.load(trpath + QDir::separator() + trfile);
	app.installTranslator(&appTranslator);
	// </tr>
	MainWindowImpl win;
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed( )), &app, SLOT( quit( )));
	app.connect( &app, SIGNAL( aboutToQuit( )), &win, SLOT( quit( )));
	return app.exec();
}
