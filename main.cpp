/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on Trolltech Qt Assistant 4.3, http://trolltech.com
****************************************************************************/
#include "mainwindow.h"
#include "config.h"
#include "pcommon.h"

#include <QApplication>
#include <QStringList>
#include <QDir>
#include <QPointer>
#include <QTranslator>
//#include <QLocale>
#include <QtDebug> //to use qWarning and qDebug messages
#include <stdlib.h>
#include <stdio.h>

int main( int argc, char ** argv )
{
    Q_INIT_RESOURCE(ra); //Initialize the resources specified by the .qrc file 

    QString aDocPath;
    bool hideSidebar = false;
    bool withGUI = true;
    QApplication a(argc, argv, withGUI);
    a.setOrganizationName("Pavel Malakhov");
    a.setApplicationName("Research Assistant");

	Config *conf = new Config();
    conf->setAppDir(QDir::currentPath() + "/");
    conf->setCacheDir(conf->AppDir() + "cache/");
	conf->setImgDir(conf->AppDir() + "images");
	conf->setPrjDir(conf->AppDir() + "projects/");
	conf->setIniFile(conf->AppDir() + "ra.ini");
	conf->setErrPage(conf->AppDir() + "last-error.html");
	conf->setAppLogFN(conf->AppDir() + "log/ra.log");
	toLog(conf->AppLogFN(), "-------");
	toLog(conf->AppLogFN(), "Research Assitant started.");
	conf->loadSettings();
	conf->loadProject(conf->CurProject());
	
    if (!conf) {
        fprintf( stderr, "Profile '%s' does not exist!\n", conf->CurProject().toLatin1().constData() );
        fflush( stderr );
        return -1;
    }
	//!+! load language based on locale on first startup or if it was not set directly
	//QString locale = QLocale::system().name();
	//qDebug()<< "locale = " << locale;
	QTranslator translator;
	if (conf->Lang() == "Russian"){
    	translator.load("lang/ra_rus");
    	a.installTranslator(&translator);
    }

    conf->hideSideBar( hideSidebar );

    QPointer<MainWindow> mw = new MainWindow();

    FontSettings settings = conf->fontSettings();
    if (mw->font() != settings.windowFont)
        a.setFont(settings.windowFont, "QWidget");
	toLog(conf->AppLogFN(), "Done with setup. Show main window.");
    mw->show();

    QStringList links = conf->source();	
	if (links.isEmpty()) //!+! or option "Remember opened files" is not set 
   	 	mw->showLink( urlifyFileName(conf->CurFile()) );
    else 
        mw->showLinks( links );
	a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    
    int appExec = a.exec();
    delete (MainWindow*)mw;
    toLog(conf->AppLogFN(),"Research Assistant shuts down normally.");
    return appExec;
}

/********************* Comments to project *********************

--------------------- Files description ---------------------

config		- configuration of the application, global functions and variables
docuparser 	- parse profile file
helpdialog	- slidebar functions: content, index, bookmarks, search
helpwindow	- working area functions: text edit, font, file 
index 		- procedures to build up index in index tab on helpdialog slidebar
mainwindow 	- main menu functions
profile 	- configuration of a project
msc			- general purpose procedures with no class relations


------- Forms aviable from (classes interaction) -------

HelpDialog		Ui::HelpDialog ui;		MainWindow *mw;	
HelpWindow								MainWindow *mw;					ItemProperties *itemprop;
MainWindow		Ui::MainWindow ui;		TabbedBrowser *tabs;[private]	HelpDialog *helpDock;[private]
										TabbedBrowser *browsers();		HelpDialog *helpDialog();
TabbedBrowser	Ui::TabbedBrowser ui;	MainWindow *mainWindow();		HelpWindow *currentBrowser();

To access from HelpDialog to HelpWindow functions use:
mw->browsers()->currentBrowser()->...

**********************************************************************/
