#include <QApplication>
#include "mainwindowimpl.h"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	MainWindowImpl win;
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed( )), &app, SLOT( quit( )));
	app.connect( &app, SIGNAL( aboutToQuit( )), &win, SLOT( quit( )));
	return app.exec();
}
