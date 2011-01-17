# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui
import Var, Settings
from	Ui_settings import Ui_SettingsDialog

class	SettingsDialog(QtGui.QDialog, Ui_SettingsDialog):
	def	__init__(self):
		QtGui.QDialog.__init__(self)
		self.setupUi(self)
#		self.__setSlots()

#	def	__setSlots(self):
#		pass
#		#self.okButton.connect( self.okButton, QtCore.SIGNAL( "clicked()" ), self.__slAccept )

	def slDialog(self) :
		'''
		Call Options dialog w/ settings.
		'''
		self.leHost.setText(Var.Setting.Host)
		self.leLogin.setText(Var.Setting.Login)
		self.lePassword.setText(Var.Setting.Password)
		return self.exec_()

	def accept(self) :
		'''
		Accept settings from Options dialog.
		'''
		Var.Setting.Host		= self.leHost.text()
		Var.Setting.Login		= self.leLogin.text()
		Var.Setting.Password	= self.lePassword.text()
		Settings.Save()
		super(SettingsDialog, self).accept()
