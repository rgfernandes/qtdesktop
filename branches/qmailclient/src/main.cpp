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
	
	/*
	QTranslator myappTranslator;
	//if (QLocale::system().name().contains("de", Qt::CaseInsensitive)){myappTranslator.load("nl_de");}

	//else 
	myappTranslator.load("nl_" + QLocale::system().name().left(QLocale::system().name().indexOf("_")));
	//QtSystem.load("qt_" + QLocale::system().name().left(QLocale::system().name().indexOf("_")),QLibraryInfo::location(QLibraryInfo::TranslationsPath));

	app.installTranslator(&myappTranslator);
	*/
	vmime::platform::setHandler <vmime::platforms::posix::posixHandler>();
	
	MainWinImpl win;
	win.show();
	//if (argc==2) win.openFile((QString)argv[1]);
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
	
}
