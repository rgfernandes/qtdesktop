/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

#include "projectproperty.h"
#include "config.h"
#include "mainwindow.h"
#include "pcommon.h"

//===================== class ProjectProperties ===========================

ProjectProperties::ProjectProperties(QWidget *parent)
     : QDialog(parent)
{
     ui.setupUi(this);
     
     connect(ui.BprjFileName, SIGNAL(clicked()), this, SLOT(chooseProjectFineName()));
     connect(ui.BStartPage, SIGNAL(clicked()), this, SLOT(chooseStartPage()));  
}

void ProjectProperties::chooseProjectFineName()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("Choose project name"),
                               Config::configuration()->PrjDir(), tr("Research Assistant Project (*.rap);;All Files (*)"));
    if (!fn.isEmpty()){
    	QFileInfo fi(fn);
    	if (fi.suffix().isEmpty()) 
    		fn += ".rap";  	
		ui.EprjFileName->setText(fn);
	}
}

void ProjectProperties::chooseStartPage()
{
	QString *s= new QString();
	QString path = ui.EprjFileName->text();
	if (path.isEmpty()) path = Config::configuration()->CurPrjDir();
    QString fn = QFileDialog::getSaveFileName(this, tr("Choose file for start page"),
                               path, tr("HTML (*.htm *.html);;All Files (*)"), s, QFileDialog::DontConfirmOverwrite );
    if (!fn.isEmpty()){
    	QFileInfo fi(fn);
    	if (fi.suffix().isEmpty()) 
    		fn += ".html";
    	ui.EStartPage->setText(fn);
   	}
}

void ProjectProperties::setProperties(QString title, QString fileName, QString startPage, bool newPrj)
{
	prjTitle= title;
	prjFN = fileName;
	prjStartPage = startPage; 
	modeNewProject = newPrj;
	
	ui.ETitle->setText(title);
	ui.EprjFileName->setText(fileName);
	ui.EStartPage->setText(startPage);
}

void ProjectProperties::reject()
{
	validProperties = false;
	QWidget::hide();  //close dialog
}

void ProjectProperties::accept()
{
	QString s = "";  //holds list of errors
	bool er = false;
	validProperties = false;
	if (ui.ETitle->text().isEmpty()){
		s = tr("- Please enter a title.\n");
		er = true;
	}
	if (ui.EprjFileName->text().isEmpty()) {
		s += tr("- Please enter a project file name.\n");
		er = true;
	}
	//check for correct start page
	if  (ui.EStartPage->text().isEmpty()){ //|| (!itemprop->valid()) ) 
		s += tr("- Please choose a valid start page.");
		er = true;
	}
	if (er){
		QMessageBox::critical(this, tr("Project property error"), s);	
	}else{
		//check for valid project file name
		QFile filePrj(ui.EprjFileName->text());
		if (!filePrj.exists()){		//create file if it's not exist
			if (filePrj.open(QIODevice::ReadWrite)){	//try to create file
				QTextStream ts(&filePrj);
				ts << "<raproject>\n</raproject>";
				filePrj.close();  
			}else{
				QMessageBox::critical(this, tr("Project property error"), tr("Can not create file:\n%1").arg(ui.EprjFileName->text()));
				er = true;
			}
		}  
		//check for valid start page file name
		QFile fileSP(ui.EStartPage->text());
		if (!fileSP.exists()){		//create file if it does not exist
			if (fileSP.open(QIODevice::ReadWrite)){		//try to create file
				QTextStream ts(&fileSP);
				ts.setCodec("UTF-8");
				QString str_header = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>" + 
									tr("About project") + "</title></head>\n";
				QString str_body = "<body>"+tr("<p>This is start page of the project \"%1\"</p>"
									"<p>It is good idea to describe your project on this page.</p>").arg(ui.ETitle->text()); 
				QString str_ender = "</body>\n</html>\n";
				ts << str_header << str_body << str_ender;
				fileSP.close();  
			}else{
				QMessageBox::critical(this, tr("Project property error"), tr("Can not create file:\n%1").arg(ui.EprjFileName->text()));
				er = true;
			}
		}  
		/*
		//create file with sources database
		QString sourcesFN = ui.EprjFileName->text();
		sourcesFN.chop(4);
		sourcesFN = sourcesFN + "-sources.db";
		emit createDb(sourcesFN);
		
		sourcesFN = sourcesFN + "-sources.xml";
		QFile fileSources(sourcesFN);
		if (!fileSources.exists()){		//create file if it does not exist
			if (fileSources.open(QIODevice::ReadWrite)){		//try to create file
				QTextStream ts(&fileSources);
				ts.setCodec("UTF-8");
				QString str_header = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>" + 
									tr("About project") + "</title></head>\n";
				QString str_body = "<body>"+tr("<p>This is start page of the project \"%1\"</p>"
									"<p>It is good idea to describe your project on this page.</p>").arg(ui.ETitle->text()); 
				QString str_ender = "</body>\n</html>\n";
				ts	<< "<raproject-sources version=\"1.0\">" << endl << endl
					<< "<source title=\"" << tr("Book Name") << "\" author=\"" << tr("Book Author") << "\" comment=\"" << tr("Book comment") << "\">" << endl
					<< "	<link comment=\"" << tr("Edition comment or Link name") << "\">" << tr("URL or path to local source file") << "</link>" << endl
					<< "</source>" << endl << endl
					<< "</raproject-sources>";
				fileSources.close();  
			}else{
				QMessageBox::critical(this, tr("Project property error"), tr("Can not create file:\n%1").arg(sourcesFN));
				er = true;
			}
		}  
		*/
		if (!er){	//project, start page and sources files do exist we can proceed with setting project properties
			prjTitle= ui.ETitle->text(); 	
			prjFN = urlifyFileName(ui.EprjFileName->text());
			prjStartPage = urlifyFileName(ui.EStartPage->text());
			validProperties = true;
			if (modeNewProject){ 
				emit createProject(prjTitle, prjFN, prjStartPage);				
			}else{
				emit updateProjectProperties(prjTitle, prjFN, prjStartPage);
			}
			QWidget::hide();  //close dialog
		}
	}
}
