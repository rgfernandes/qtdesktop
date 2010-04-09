# -*- coding: utf-8 -*-
'''
Main.py: main app loop
'''

import sys
from PyQt4 import QtCore, QtGui, QtSql
import Var, Settings, MainWindow

def	Main():
	'''
	Main module
	'''
	# 0. Main
	a = QtGui.QApplication( sys.argv )
	# 1. app info
	QtCore.QCoreApplication.setApplicationName(Var.AppName)
	QtCore.QCoreApplication.setApplicationVersion(Var.AppVersion)
	QtCore.QCoreApplication.setOrganizationName(Var.AppAuthorName)
	QtCore.QCoreApplication.setOrganizationDomain(Var.AppOrgDomain)
	# 2. load settings
	Settings.Load()
	# 3. Translations
	translator	= QtCore.QTranslator()
	trpath		= QtCore.QLibraryInfo.location(QtCore.QLibraryInfo.TranslationsPath)
	trfile		= QtCore.QString('qtselta') + '_' + QtCore.QLocale.system().name().section('_', 0, 0)
	if (not QtCore.QFile.exists(trpath + QtCore.QDir.separator() + trfile)):
		trpath = QtCore.QString(Var.DirI18n)
	translator.load(trpath + QtCore.QDir.separator() + trfile)
	a.installTranslator(translator)
	# 4. dialogs
	mw	= MainWindow.MainWindow()
	# 6. models
	#mw.setModel()
	# 7. Lets go
	mw.letsgo()
	a.exec_()
	del mw
