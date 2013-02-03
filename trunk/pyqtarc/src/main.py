# -*- coding: utf-8 -*-

import sys

from PyQt4 import QtCore, QtGui, QtSql

from mainwindow import MainWindow
from utility import createConnection

def main():
	app = QtGui.QApplication(sys.argv)
	QtCore.QCoreApplication.setApplicationName("pyqtarc")
	QtCore.QCoreApplication.setApplicationVersion("0.0.1")
	QtCore.QCoreApplication.setOrganizationName("TI_Eugene")
	QtCore.QCoreApplication.setOrganizationDomain("eap.su")
	# <i18n>
	translator = QtCore.QTranslator()
	trpath = QtCore.QLibraryInfo.location(QtCore.QLibraryInfo.TranslationsPath)
	trfile = QtCore.QString('pyqtarc') + '_' + QtCore.QLocale.system().name().section('_', 0, 0)
	if (not QtCore.QFile.exists(trpath + QtCore.QDir.separator() + trfile)):
		trpath = QtCore.QString('i18n')
	translator.load(trpath + QtCore.QDir.separator() + trfile)
	app.installTranslator(translator)
	# </i18n>
	# letsgo
	#QtCore.QTextCodec.setCodecForCStrings(QTextCodec.codecForName("UTF-8"))
	mw = MainWindow()
	mw.show()
	# <db>
	db = QtSql.QSqlDatabase.addDatabase("QSQLITE")
	if (not createConnection(db, mw)):
		return 1
	# </db>
	mw.setModel(db)
	argv = app.arguments()
	if (argv.count() == 2):
		mw._file_open(argv[1])
	retvalue = app.exec_()
	del mw
	db.close()
	sys.exit(retvalue)
