# -*- coding: utf-8 -*-
'''
DbMgr: database "manager"
'''

from PyQt4 import QtCore, QtGui, QtSql
import Var

class	DbMgr:
	self.maindb = None
	self.mw = None

	def	__init__(self, mw):
		self.mw = mw

	def	init(self):
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

	def	dbCreate(self):
		dbname, result = QtGui.QInputDialog.getText(
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
