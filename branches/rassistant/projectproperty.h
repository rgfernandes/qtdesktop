/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef __PROJECTPROPERTY_H__
#define __PROJECTPROPERTY_H__

#include "ui_projectproperty.h"

//===================== class ProjectProperties ===========================

class ProjectProperties : public QDialog
{
     Q_OBJECT

public:
	ProjectProperties(QWidget *parent = 0);
	bool valid() { return validProperties; }

signals:
	void createProject(QString title, QString fileName, QString startPage);
	void updateProjectProperties(QString title, QString fileName, QString startPage);
	//void createDb(QString dbFileName);
     
public slots:
	void chooseProjectFineName();
	void chooseStartPage();
	void accept();
	void reject();
	void setProperties(QString title, QString fileName, QString startPage, bool newPrj);
//	QString Title()			{ return prjTitle; }	
	QString FileName()		{ return prjFN; }
//	QString StartPage()		{ return prjStartPage; } 
	
private:
	Ui::ProjectProperty ui; 
	
	bool modeNewProject; 	
	QString prjTitle, prjFN, prjStartPage;
	bool validProperties;
}; // class ProjectProperties

#endif // __PROJECTPROPERTY_H__
