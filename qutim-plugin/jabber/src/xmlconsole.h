/*
    Сopyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef XmlConsole_H
#define XmlConsole_H

#include "ui_xmlconsole.h"

#include <QWidget>
#include <QDialog>
#include <QPointer>
#include <QVBoxLayout>
#include <QHBoxLayout>

class XmlPrompt : public QDialog
{
	Q_OBJECT
public:
	XmlPrompt(QWidget *parent);
	~XmlPrompt();

signals:
	void textReady(const QString &);

private slots:
	void doTransmit();

private:
	QTextEdit *te;
};

class XmlConsole : public QWidget
{
    Q_OBJECT
    
private:
	Ui::XmlConsole ui;
public:
	XmlConsole(const QString &title, QWidget *parent=0);
	void appendTag(const QString &xml, bool in);
private:
	XmlPrompt* prompt;
private slots:
	void sendXML();
	void clearXml();
	void xml_textReady(const QString &str);
signals:
	void send(QString XML);
};

#endif
