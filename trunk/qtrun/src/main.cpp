/*
Copyright 2009 Eugene A. Pivnev

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <QApplication>
#include <QFile>
#include <QLibraryInfo>
#include <QLocale>
#include <QProcess>
#include <QString>
#include <QTranslator>

#include "qtrun.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTranslator appTranslator;
	QString trpath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);	// /usr/share/qt4/translations
	QString trfile = QString("qtrun_") + QLocale::system().name().left(2);
	if (not QFile::exists(trpath + "/" + trfile))
		trpath = a.applicationDirPath() + "/translations";
	appTranslator.load(trpath + "/" + trfile);
	a.installTranslator(&appTranslator);

	QtRun *w = new QtRun();
	w->show();
	int retvalue = a.exec();
	if (!w->path.isEmpty())
		QProcess::startDetached(w->path);
	return retvalue;
}
