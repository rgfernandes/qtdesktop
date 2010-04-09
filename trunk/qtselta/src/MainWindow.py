# -*- coding: utf-8 -*-
'''
MainWindow: Main window
'''

from PyQt4 import QtCore, QtGui, QtSql
import Var, Settings, SettingsDialog,  Help,  DbMgr
from Ui_main import Ui_MainWindow

class	MainWindow(QtGui.QMainWindow, Ui_MainWindow):
	self.maindb = None

	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		#self.modelMain = None
		self.setupUi(self)
		self.__settingsDialog = SettingsDialog.SettingsDialog()
		self.__setSlots()
		self.dbmgr = DbMgr.DbMgr()
		self.__dbdict = {}

	def	letsgo(self):
		self.show()
		self.__refreshDB()

	def	__setSlots(self):
		self.connect( self.actionAbout,	QtCore.SIGNAL( "triggered()" ), Help.About )
		self.connect( self.actionAboutQt,	QtCore.SIGNAL( "triggered()" ), Help.AboutQt4 )
		self.connect( self.actionHelp,	QtCore.SIGNAL( "triggered()" ), Help.Help )
		self.connect( self.actionSettings,	QtCore.SIGNAL( "triggered()" ), self.__slSettings )
		self.connect( self.actionDbCreate,	QtCore.SIGNAL( "triggered()" ), self.__slDbCreate )
		self.connect( self.actionDbRename,	QtCore.SIGNAL( "triggered()" ), self.__slDbRename )
		self.connect( self.actionDbDelete,	QtCore.SIGNAL( "triggered()" ), self.__slDbDelete )
		self.connect( self.actionDbInit,	QtCore.SIGNAL( "triggered()" ), self.__slDbInit )
		self.connect( self.actionDbUpgrade,	QtCore.SIGNAL( "triggered()" ), self.__slDbUpgrade )
		self.connect( self.actionDbDeinit,	QtCore.SIGNAL( "triggered()" ), self.__slDbDeinit )
		self.connect( self.actionDbBackup,	QtCore.SIGNAL( "triggered()" ), self.__slDbBackup )
		self.connect( self.actionDbRestore,	QtCore.SIGNAL( "triggered()" ), self.__slDbRestore )

	def	__slSettings(self):
		'''
		Call Settings dialog
		'''
		if self.__settingsDialog.slDialog():
			self.__refreshDB()

	def	__addDB(self,  dbname):
		'''
		Add new DB to db list
		'''
		db = QtSql.QSqlDatabase.addDatabase("QPSQL", dbname)
		db.setDatabaseName(dbname)
		db.setHostName(Var.Setting.Host)
		db.setUserName(Var.Setting.Login)
		db.setPassword(Var.Setting.Password)
		self.__dbdict[dbname] = db
		return db

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
		self.maindb = self.__addDB('postgres')
		if not self.maindb.open():
			err = self.maindb.lastError()
			QtGui.QMessageBox.critical(self,
				QtGui.QApplication.translate("Main", "Opening database error"),
				QtGui.QApplication.translate("Main", "Err no: %1\nDriver err: %2\nDB err: %3").arg(err.number()).arg(err.driverText()).arg(err.databaseText()),
				QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton)	#Unable to establish a database connection: $1
			self.maindb.removeDatabase("postgres")
			return False
		return True

	def	__refreshDB(self):
		self.twMain.setEnabled(False)
		if self.__createConnection():
			self.twMain.setEnabled(True)
			self.__refreshList()

	def	__refreshList(self):
		q = QtSql.QSqlQuery(self.maindb)
		row = 0;
		if q.exec_('SELECT datname FROM pg_database WHERE NOT datistemplate ORDER BY datname'):
			# ":/sql/sql/listdbs.sql"
			while q.next():
				dbname = q.value(0).toString()
				if (dbname != 'postgres'):
					db = self.__addDB(dbname)
				else:
					db = self.maindb
				# check ver
				ver = ''
				if db.open():
					q1 = QtSql.QSqlQuery(db)
					if q1.exec_('SELECT ver FROM pg_selta_version'):
						if q1.next():
							ver = q1.value(0).toString()
					db.close()
				else:
					print "Can't open db"
				# /check ver
				self.twMain.insertRow(row)
				self.twMain.setItem(row, 0, QtGui.QTableWidgetItem(dbname))
				self.twMain.setItem(row, 1, QtGui.QTableWidgetItem(ver))
				row += 1
				#print dbname, ver

	def	__slDbCreate(self):
		dbname,  result = QtGui.QInputDialog.getText(
			self,
			QtGui.QApplication.translate("Main", "New database name"),
			QtGui.QApplication.translate("Main", "Database name")
		)
		if result:
			if self.maindb.open():
				#q = QtSql.QSqlQuery()
				if self.maindb.exec_("CREATE DATABASE %s WITH OWNER %s;" % (dbname,  Var.Setting.Login)):
					# FIXME: lastErr
					self.__addDB(dbname)
					row = self.twMain.rowCount()
					self.twMain.insertRow(row)
					self.twMain.setItem(row, 0, QtGui.QTableWidgetItem(dbname))
				else:
					print "Can't create db"
				self.maindb.close()
			else:
				print "Can't open maindb"

	def	__slDbRename(self):
		pass

	def	__slDbDelete(self):
		pass

	def	__slDbInit(self):
		pass

	def	__slDbUpgrade(self):
		pass

	def	__slDbDeinit(self):
		pass

	def	__slDbBackup(self):
		pass

	def	__slDbRestore(self):
		pass
