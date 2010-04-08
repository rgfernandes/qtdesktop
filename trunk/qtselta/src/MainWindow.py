# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui, QtSql
import Var, Settings, SettingsDialog,  Help
from Ui_main import Ui_MainWindow

class	MainWindow(QtGui.QMainWindow, Ui_MainWindow):
	maindb = None

	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		#self.modelMain = None
		self.setupUi(self)
		self.__settingsDialog = SettingsDialog.SettingsDialog()
		self.__setSlots()

	def	letsgo(self):
		self.show()
		self.__refreshDB()

	def	__setSlots(self):
		self.connect( self.actionAbout,	QtCore.SIGNAL( "triggered()" ), Help.About )
		self.connect( self.actionAboutQt,	QtCore.SIGNAL( "triggered()" ), Help.AboutQt4 )
		self.connect( self.actionHelp,	QtCore.SIGNAL( "triggered()" ), Help.Help )
		self.connect( self.actionSettings,	QtCore.SIGNAL( "triggered()" ), self.__slSettings )

	def	__slSettings(self):
		'''
		Call Settings dialog
		'''
		if self.__settingsDialog.slDialog():
			self.__refreshDB()

	def	__doquery(self, q, s):
		'''
		@param wm:QMainWindow - the subj
		@param q:QSqlQuery - empty (?) query
		@param s:QString - SQL expression
		@return 
		'''
		retvalue = q.exec_(s)
		if (not retvalue):
			err = q.lastError()
			QtGui.QMessageBox.critical(self,
				QtGui.QApplication.translate("Main", "Saving error"),
				QtGui.QApplication.translate("Main", "Driver error: %1;\nDatabase error: %2;\nSQL string: %3").arg(err.driverText()).arg(err.databaseText()).arg(s),
				QtGui.QMessageBox.Cancel,
				QtGui.QMessageBox.NoButton)
		return retvalue

	def	__createConnection(self):
		'''
		Creates db connection
		@return - bool: True if success
		'''
		self.maindb = QtSql.QSqlDatabase.addDatabase("QPSQL", "pg_database")
		self.maindb.setHostName(Var.Setting.Host)	# Var.Setting.Host
		self.maindb.setDatabaseName('postgres')
		self.maindb.setUserName(Var.Setting.Login)
		self.maindb.setPassword(Var.Setting.Password)
		if not self.maindb.open():
			err = self.maindb.lastError()
			QtGui.QMessageBox.critical(self,
				QtGui.QApplication.translate("Main", "Opening database error"),
				QtGui.QApplication.translate("Main", "Err no: %1\nDriver err: %2\nDB err: %3").arg(err.number()).arg(err.driverText()).arg(err.databaseText()),
				QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton)	#Unable to establish a database connection: $1
			self.maindb.removeDatabase("pg_database")
			return False
		return True

	def	__refreshDB(self):
		self.twMain = False
		if self.__createConnection():
			self.twMain = True
			self.__refreshList()

	def	__refreshList(self):
		q = QtSql.QSqlQuery(self.maindb)
		row = 0;
		if q.exec_('SELECT datname FROM pg_database WHERE NOT datistemplate ORDER BY datname'):
			while q.next():
				dbname = q.value(0).toString()
				print dbname
				db = QtSql.QSqlDatabase.addDatabase("QPSQL", dbname)
				db.setHostName(Var.Setting.Host)	# Var.Setting.Host
				db.setDatabaseName(dbname)
				db.setUserName(Var.Setting.User)
				db.setPassword(Var.Setting.Password)
				ver = ''
				if db.open():
					q1 = QtSql.QSqlQuery(db)
					if q1.exec_('SELECT ver FROM pg_selta_version'):
						if q1.next():
							ver = q1.value(0).toString()
					db.close()
				#else:
				#	print "Can't open db"
				self.twMain.insertRow(row)
				self.twMain.setItem(row, 0, QtGui.QTableWidgetItem(dbname))
				self.twMain.setItem(row, 1, QtGui.QTableWidgetItem(ver))
				row += 1
				#print dbname, ver
