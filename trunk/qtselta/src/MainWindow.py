# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui, QtSql
#import			PolicyDialog, Help, Print, Settings, Record, ImpEx, Utility, Report
import Var, Settings, SettingsDialog
from Ui_main import Ui_MainWindow

class	MainWindow(QtGui.QMainWindow, Ui_MainWindow):
	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		#self.modelMain = None
		self.setupUi(self)
		self.__settingsDialog = SettingsDialog.SettingsDialog()

	def	__setSlots(self):
		self.connect( self.actionAbout,	QtCore.SIGNAL( "triggered()" ), self.__slAbout)
		#self.connect( self.actionAboutQt4,	QtCore.SIGNAL( "triggered()" ), Help.AboutQt4 )
		#self.connect( self.actionHelp,	QtCore.SIGNAL( "triggered()" ), Help.Help )
		self.connect( self.actionSettings,	QtCore.SIGNAL( "triggered()" ), self.__slSettings )

	def	__slAbout(self):
		'''
		About
		'''
		pass

	def	__slSettings(self):
		'''
		Call Settings dialog
		'''
		pass
