# -*- coding: utf-8 -*-
'''
MainWindow: Main window
'''

from PyQt4 import QtCore, QtGui, QtSql
import Var, Settings, SettingsDialog,  Help,  DbMgr
from Ui_main import Ui_MainWindow

class	MainWindow(QtGui.QMainWindow, Ui_MainWindow):
	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		#self.modelMain = None
		self.setupUi(self)
		self.__settingsDialog = SettingsDialog.SettingsDialog()
		self.__setSlots()
		self.dbmgr = DbMgr.DbMgr(self)

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

	def	letsgo(self):
		self.show()
		if (self.dbmgr.init()):
			self.dbmgr.fillOut()
		self.__refreshDB()

	def	__refreshDB(self):
		self.twMain.setRowCount(0)
		row = 0;
		self.twMain.setEnabled(True)
		for dbname in self.dbmgr.dbdict.keys():
			self.twMain.insertRow(row)
			self.twMain.setItem(row, 0, QtGui.QTableWidgetItem(dbname))
			#self.twMain.setItem(row, 1, QtGui.QTableWidgetItem(ver))
			row += 1
		self.twMain.sortItems(0)

	def	__currentDBname(self):
		return self.twMain.item(self.twMain.currentRow(), 0).text()

	def	__slSettings(self):
		'''
		Call Settings dialog
		'''
		if self.__settingsDialog.slDialog():
			self.__refreshDB()

	def	__slDbCreate(self):
		dbname, result = QtGui.QInputDialog.getText(
			self,
			QtGui.QApplication.translate("MainWindow", "Creating database"),
			QtGui.QApplication.translate("MainWindow", "Database name")
		)
		if result:
			if (self.dbmgr.dbCreate(dbname)):
					row = self.twMain.rowCount()
					self.twMain.insertRow(row)
					self.twMain.setItem(row, 0, QtGui.QTableWidgetItem(dbname))
					self.twMain.sortItems(0)

	def	__slDbRename(self):
		dbname = self.__currentDBname()
		newdbname, result = QtGui.QInputDialog.getText(
			self,
			QtGui.QApplication.translate("MainWindow", "Renaming database"),
			QtGui.QApplication.translate("MainWindow", "Database name")
		)
		if result:
			if (self.dbmgr.dbRename(dbname, newdbname)):
				self.twMain.item(self.twMain.currentRow(), 0).setText(newdbname)
				self.twMain.sortItems(0)

	def	__slDbDelete(self):
		dbname = self.__currentDBname()
		if (QtGui.QMessageBox.question(self,
			QtGui.QApplication.translate("MainWindow", "Deleting database"),
			QtGui.QApplication.translate("MainWindow", "You are about to delete database \"%1\". Are you sure?").arg(dbname),
			QtGui.QMessageBox.Yes | QtGui.QMessageBox.No,
			QtGui.QMessageBox.NoButton) == QtGui.QMessageBox.Yes):
				if (self.dbmgr.dbDelete(dbname)):
					self.twMain.removeRow(self.twMain.currentRow())

	def	__slDbInit(self):
		dbname = self.__currentDBname()

	def	__slDbUpgrade(self):
		dbname = self.__currentDBname()

	def	__slDbDeinit(self):
		dbname = self.__currentDBname()

	def	__slDbBackup(self):
		dbname = self.__currentDBname()

	def	__slDbRestore(self):
		dbname = self.__currentDBname()
