# -*- coding: utf-8 -*-
'''
Processed data: base, blog, calendar, docs, photos, spreadsheet
Not processed: addressbook, mail, writer
'''

import		sys, gdata
from	PyQt4	import QtCore, QtGui
import		blog, settings
from	ui		import Ui_main
import		core.blog, cs_gdata.blog, cs_gdata.contacts, cs_gdata.notebook

mwMain = None

class	MainWindow(QtGui.QMainWindow, Ui_main.Ui_MainWindow):
	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		self.setupUi(self)
		self.__setSlots()

	def	__setSlots(self):
		self.actionSettings.connect( self.actionSettings, QtCore.SIGNAL( "triggered()" ), settings.dSettings.slDialog )
		self.actionBlog.connect( self.actionBlog, QtCore.SIGNAL( "triggered()" ), self.__slOpenBlog )
		self.actionContacts.connect( self.actionContacts, QtCore.SIGNAL( "triggered()" ), self.__slOpenContacts )
		self.actionNotebook.connect( self.actionNotebook, QtCore.SIGNAL( "triggered()" ), self.__slOpenNotebook )

	def	__slOpenBlog(self):
		self.blog = blog.BlogDialog(str(settings.Setting.LoginBlog), str(settings.Setting.PasswordBlog))
		self.blog.exec_()

	def	__slOpenContacts(self):
		self.contacts = cs_gdata.contacts.Contacts()
		self.contacts.login(str(settings.Setting.LoginBlog), str(settings.Setting.PasswordBlog))
		self.contacts.load()

	def	__slOpenNotebook(self):
		self.notebook = cs_gdata.notebook.Notebook()
		self.notebook.login(str(settings.Setting.LoginBlog), str(settings.Setting.PasswordBlog))
		self.notebook.load()

def	Main():
	'''
	Main module
	'''
	global mwMain

	# 0. Main
	aMain		= QtGui.QApplication( sys.argv )

	# 1. Translation
	translator	= QtCore.QTranslator()
	translator.load("./googlepim_" + QtCore.QLocale().system().name().left(2))	# FIXME:
	aMain.installTranslator(translator)

	settings.dSettings	= settings.SettingsDialog()
	settings.dSettings.slLoad()
	mwMain			= MainWindow()

	# 6. Lets go
	mwMain.show()
	aMain.exec_()

##def	Main():
##	srcfeed = cs_gdata.blog.BlogSpot()
##	srcfeed.login(sys.argv[1], sys.argv[2])
##	dstfeed = srcfeed.load()
##	dstfeed._print()
