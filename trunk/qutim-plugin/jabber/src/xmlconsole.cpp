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

#include "xmlconsole.h"
//#include "utils.h"
#include <QPalette>
#include <QDebug>
#include <gloox/tag.h>

XmlPrompt::XmlPrompt(QWidget *parent)
	: QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("XML Input"));

	QVBoxLayout *vb1 = new QVBoxLayout(this);

	te = new QTextEdit(this);
	te->setAcceptRichText(false);
	vb1->addWidget(te);

	QHBoxLayout *hb1 = new QHBoxLayout(this);
	QPushButton *pb;

	pb = new QPushButton(tr("&Send"), this);
	pb->setDefault(TRUE);
	connect(pb, SIGNAL(clicked()), SLOT(doTransmit()));
	hb1->addWidget(pb);
	hb1->addStretch(1);

	pb = new QPushButton(tr("&Close"), this);
	connect(pb, SIGNAL(clicked()), SLOT(close()));
	hb1->addWidget(pb);
	vb1->addLayout(hb1);
	resize(320,240);
}

XmlPrompt::~XmlPrompt()
{
}

void XmlPrompt::doTransmit()
{
  /*  Tag* tg = new Tag("fgfg");
    if(!tg->setXmlns( utils::toStd(te->toPlainText())) )
    {
	  qDebug() << "not valid";
    } else {
	qDebug() << "valid";
    }*/
	textReady(te->toPlainText());
	close();
}

XmlConsole::XmlConsole(const QString &title, QWidget *parent) : QWidget(parent) 
{
    ui.setupUi(this);
    setWindowTitle(title);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

void XmlConsole::appendTag(const QString &xml, bool in)
{
    if(xml.size()==1)
	return;
    QString html = QString("<font color=\"%1\">%2</font><br/><br/>").arg(in?"yellow":"red").arg(Qt::escape(xml).replace("\n","<br/>").replace("&gt;&lt;","&gt;<br/>&lt;"));
    ui.textBrowser->append(html);
}

void XmlConsole::sendXML()
{
    prompt = new XmlPrompt(this);
    connect(prompt, SIGNAL(textReady(const QString &)), SLOT(xml_textReady(const QString &)));
    prompt->show();
}

void XmlConsole::xml_textReady(const QString &str)
{
    emit send(str);
}

void XmlConsole::clearXml()
{
    ui.textBrowser->setHtml(QApplication::translate("XmlConsole", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\" bgcolor=\"#000000\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
}
