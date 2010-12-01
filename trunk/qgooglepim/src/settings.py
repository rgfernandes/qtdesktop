# -*- coding: utf-8 -*-
'''
'''

import		os, platform, sys
from	PyQt4	import QtCore, QtGui
from	ui		import Ui_settings

### Variables
dSettings	= None

# Application
Application		= QtCore.QObject()
Application.sName	= "GooglePIM"
Application.sVersion	= "0.1"
Application.sRelease	= "0"
Application.sDomain	= "code.google.com/p/googlepim"
QtCore.QCoreApplication.setApplicationName( Application.sName )

# Author
Author			= QtCore.QObject()
Author.sName		= "TI_Eugene"
Author.Email		= QtCore.QObject()
Author.Email.sUser	= "ti.eugene"
Author.Email.sDomain	= "@gmail.com"
QtCore.QCoreApplication.setOrganizationName( Author.sName )
QtCore.QCoreApplication.setOrganizationDomain( Application.sDomain )

# Default values
Default			= QtCore.QObject()
Default.Login	= ""
Default.Password	= ""

# Settings key names
Key				= QtCore.QObject()
Key.Login		= "login"
Key.Password	= "password"

# Setting
sSettings			= QtCore.QSettings()
Setting			= QtCore.QObject()
Setting.Login	= None
Setting.Password	= None

class	SettingsDialog(QtGui.QDialog, Ui_settings.Ui_SettingsDialog):
	def	__init__(self):
		QtGui.QDialog.__init__(self)
		self.setupUi(self)
		self.__setSlots()

	def	__setSlots(self):
		self.okButton.connect( self.okButton, QtCore.SIGNAL( "clicked()" ), self.slAccept )

	def slLoad(self) :
		'''
		Load settings from config file or set default values.
		'''
		Setting.Login	= sSettings.value(Key.Login).toString()	if sSettings.contains(Key.Login)		else Default.Login
		Setting.Password	= sSettings.value(Key.Password).toString()	if sSettings.contains(Key.Password)	else Default.Password

	def slSave(self) :
		'''
		Save settings from variables to config file.
		'''
		sSettings.setValue( Key.Login,		QtCore.QVariant(Setting.Login))
		sSettings.setValue( Key.Password,	QtCore.QVariant(Setting.Password))

	def slDialog(self) :
		'''
		Call Options dialog w/ settings.
		'''
		self.leLogin.setText(Setting.Login)
		self.lePassword.setText(Setting.Password)

		self.exec_()

		self.slLoad()

	def slAccept(self) :
		'''
		Accept settings from Options dialog.
		'''
		Setting.Login		= self.leLogin.text()
		Setting.Password		= self.lePassword.text()

		self.slSave()
