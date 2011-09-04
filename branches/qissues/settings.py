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
Default.LoginBlog	= ""
Default.PasswordBlog	= ""

# Settings key names
Key				= QtCore.QObject()
Key.LoginBlog		= "login"
Key.PasswordBlog	= "password"

# Setting
sSettings			= QtCore.QSettings()
Setting			= QtCore.QObject()
Setting.LoginBlog	= None
Setting.PasswordBlog	= None

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
		Setting.LoginBlog	= sSettings.value(Key.LoginBlog).toString()	if sSettings.contains(Key.LoginBlog)		else Default.LoginBlog
		Setting.PasswordBlog	= sSettings.value(Key.PasswordBlog).toString()	if sSettings.contains(Key.PasswordBlog)	else Default.PasswordBlog

	def slSave(self) :
		'''
		Save settings from variables to config file.
		'''
		sSettings.setValue( Key.LoginBlog,		QtCore.QVariant(Setting.LoginBlog))
		sSettings.setValue( Key.PasswordBlog,	QtCore.QVariant(Setting.PasswordBlog))

	def slDialog(self) :
		'''
		Call Options dialog w/ settings.
		'''
		self.leLoginBlog.setText(Setting.LoginBlog)
		self.lePasswordBlog.setText(Setting.PasswordBlog)

		self.exec_()

		self.slLoad()

	def slAccept(self) :
		'''
		Accept settings from Options dialog.
		'''
		Setting.LoginBlog		= self.leLoginBlog.text()
		Setting.PasswordBlog		= self.lePasswordBlog.text()

		self.slSave()
