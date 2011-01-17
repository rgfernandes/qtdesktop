#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include "mainwinimpl.h"
//

#include <vmime/vmime.hpp>
#include <vmime/platforms/posix/posixHandler.hpp>

int main(int argc, char ** argv)
{
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

	//vmime::platform::setHandler <vmime::platforms::posix::posixHandler>();
	
	MainWinImpl win;
	win.show();
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
	
}
