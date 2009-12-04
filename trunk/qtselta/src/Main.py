# -*- coding: utf-8 -*-
"""
"""

import sys
from PyQt4 import QtCore, QtGui, QtSql
import Var, Settings, MainWindow

def	__doquery(mw, q, s):
	'''
	@param wm:QMainWindow - the subj
	@param q:QSqlQuery - empty (?) query
	@param s:QString - SQL expression
	@return 
	'''
	retvalue = q.exec_(s)
	if (not retvalue):
		err = q.lastError()
		QtGui.QMessageBox.critical(mw,
			QtGui.QApplication.translate("Main", "Saving error"),
			QtGui.QApplication.translate("Main", "Driver error: %1;\nDatabase error: %2;\nSQL string: %3").arg(err.driverText()).arg(err.databaseText()).arg(s),
			QtGui.QMessageBox.Cancel,
			QtGui.QMessageBox.NoButton)
	return retvalue

def	__getNewUser(mw):
	'''
	get user requisition on first app startup.
	'''
	retvalue = InitDialog.InitDialog().Open()
	if (retvalue):
		while (retvalue) and ((not bool(retvalue[0])) or (retvalue[1].isEmpty())):
			retvalue = InitDialog.InitDialog().Open()
	return retvalue

def	__createConnection(mw):
	'''
	Creates db connection
	@return - bool: True if success
	'''
	maindb = QtSql.QSqlDatabase.addDatabase("QPSQL",  "pg_database")
	maindb.setHostName('server')	# Var.Setting.Host
	maindb.setDatabaseName('postgres')
	maindb.setUserName('postgres')
	maindb.setPassword('postgres')
	if not maindb.open():
		err = maindb.lastError()
		#print err.text()
		QtGui.QMessageBox.critical(mw,
			QtGui.QApplication.translate("Main", "Opening database error"),
			QtGui.QApplication.translate("Main", err.text()),
			QtGui.QMessageBox.Cancel, QtGui.QMessageBox.NoButton)	#Unable to establish a database connection: $1
		return False
	else:
		q = QtSql.QSqlQuery(maindb)
		row = 0;
		if q.exec_('SELECT datname FROM pg_database WHERE NOT datistemplate ORDER BY datname'):
			while q.next():
				dbname = q.value(0).toString()
				#print dbname
				db = QtSql.QSqlDatabase.addDatabase("QPSQL", dbname)
				db.setHostName('server')	# Var.Setting.Host
				db.setDatabaseName(dbname)
				db.setUserName('postgres')
				db.setPassword('postgres')
				ver = ''
				if db.open():
					q1 = QtSql.QSqlQuery(db)
					if q1.exec_('SELECT ver FROM pg_selta_version'):
						if q1.next():
							ver = q1.value(0).toString()
					db.close()
				#else:
				#	print "Can't open db"
				mw.twMain.insertRow(row)
				mw.twMain.setItem(row, 0, QtGui.QTableWidgetItem(dbname))
				mw.twMain.setItem(row, 1, QtGui.QTableWidgetItem(ver))
				row += 1
				#print dbname, ver
	return True

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
		#print trpath + QtCore.QDir.separator() + trfile
	translator.load(trpath + QtCore.QDir.separator() + trfile)
	a.installTranslator(translator)
	# 4. dialogs
	mw	= MainWindow.MainWindow()
	# 5. database
	if not __createConnection(mw):
		sys.exit(1)
	# 6. models
	#mw.setModel()
	# 7. Lets go
	mw.show()
	a.exec_()
