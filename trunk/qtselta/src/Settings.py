# -*- coding: utf-8 -*-
"""
"""

from PyQt4 import QtCore, QtGui
import Var

Default = None
Key	= None

def	__initSetting():
	Var.Setting			= QtCore.QObject()
	Var.Setting.Host		= None
	Var.Setting.Login		= None
	Var.Setting.Password		= None

def	__initKey():
	global Key
	Key			= QtCore.QObject()
	Key.Host		= "host"
	Key.Login		= "login"
	Key.Password	= "password"

def	__initDefault():
	global Default
	Default			= QtCore.QObject()
	Default.Host	= "server"
	Default.Login	= "postgres"
	Default.Password	= "postgres"

def Load() :
	'''
	Load settings from config file or set default values.
	'''
	global Key, Default
	sSettings = QtCore.QSettings()

	if (Var.Setting == None):
		__initSetting()
	if (Key == None):
		__initKey()
	if (Default == None):
		__initDefault()

	Var.Setting.Host		= sSettings.value(Key.Host).toString()		if sSettings.contains(Key.Host)		else Default.Host
	Var.Setting.Login		= sSettings.value(Key.Login).toString()		if sSettings.contains(Key.Login)		else Default.Login
	Var.Setting.Password	= sSettings.value(Key.Password).toString()		if sSettings.contains(Key.Password)		else Default.Password

def Save() :
	'''
	Save settings from variables to config file.
	'''
	global Key
	sSettings = QtCore.QSettings()

	sSettings.setValue(Key.Host,		QtCore.QVariant(Var.Setting.Host))
	sSettings.setValue(Key.Login,		QtCore.QVariant(Var.Setting.Login))
	sSettings.setValue(Key.Password,		QtCore.QVariant(Var.Setting.Password))
