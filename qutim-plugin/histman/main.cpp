/*
 * main.cpp
 * 
 * Copyright (C) 2008  Nigmatullin Ruslan
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
*/

#include "qimhm.h"
/*#include "dbh.h"
#include "clients/qip.h"
#include "clients/qutim.h"*/

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QString lang = QLocale::system().name().toLower();
	//lang.truncate(2);
	QString tran="translation/qimhm_" + lang;
	QTranslator qtTranslator;
	qtTranslator.load(tran);
	a.installTranslator(&qtTranslator);
    //dbh *data = new dbh("/home/euroelessar/dev/qIMhm/test/history/");
    //qip(data).load("/mnt/sdb1/exe/qip_old/Users/3485140/History/");
    //qutim(data).load("/home/euroelessar/.config/qutim/ICQ.3485140/history/");
   /* for (int i = 0; i < data->uins.size(); ++i) {
		qWarning("================================================"); 
		qWarning(qPrintable("uin: "+data->uins[i].uin));    	
		for (int j = 0; j < data->uins[i].msgs.size(); ++j) {
			qWarning(qPrintable(data->uins[i].msgs[j].getXmlString()));
		}
	}*/
    //data->save();
    HistoryManagerWidget w;
    w.show();
    return a.exec();
}
