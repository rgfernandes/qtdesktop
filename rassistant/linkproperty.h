/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef __LINKPROPERTY_H__
#define __LINKPROPERTY_H__

#include "ui_linkproperty.h"

//===================== class LinkProperties =====================
class LinkProperties : public QDialog
{
     Q_OBJECT

public:
	LinkProperties(QWidget *parent = 0);
	bool valid() { return validProperties; }
	QString LinkText() { return linkText; }
	QString LinkLocation() { return linkLocation; }

signals:
	void removeLink();//(QString title, QString fileName, QString iconFN);
	void updateLink(QString lText, QString lLocation);
     
public slots:
	void chooseFile();
	void RemoveLink();
	void accept();
	void reject();
	void setProperties(QString lText, QString lLocation);
	
private:
	Ui::LinkProperty ui;
	
	QString linkText, linkLocation;
	bool validProperties;

}; // class LinkProperties

#endif // __LINKPROPERTY_H__
