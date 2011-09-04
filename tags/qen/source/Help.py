"""
QEN:		Qt eGroupWare Notifier
Designed by Eugene A. Pivnev

File:	Help.py
Purpose:	Help menu handler
"""


### Imports

# QEN 1
import	Import

# PyQt4
try :
	from	PyQt4	import QtCore, QtGui
except :
	Import.slException( "PyQt4" )

# QEN 2
import	Main, Settings



### Slots

# About Qt4
def slAboutQt4() :
	QtGui.QApplication.aboutQt()


# About QEN
def slAboutQEN() :
	QtGui.QMessageBox.about( Main.uiMain.cwMain, "About " + Settings.Application.sName,
		"<b>" + Settings.Application.sName + "</b> " + Settings.Application.sVersion +
		"<br>Designed by " + Settings.Author.sName + "<br>"
		"<br>" + Settings.Application.sName + " is a eGroupWare's InfoLog Notifier.<br>" )


# Open URL
def slUOpen( uURL ) :

	Main.uiMain.sbMain.showMessage( "Launching external application ..." )

	QtGui.QDesktopServices.openUrl( uURL )


# URL: QEN
def slUQEN() :

	# Set URL
	uURL = QtCore.QUrl( "http://uprojects.googlecode.com/svn/trunk/qen" )

	slUOpen( uURL )


# URL: Qt4
def slUQt4() :

	# Set URL
	uURL = QtCore.QUrl( "http://www.trolltech.com/products/qt" )

	slUOpen( uURL )

# URL: PyQt4
def slUPyQt4() :

	# Set URL
	uURL = QtCore.QUrl( "http://www.riverbankcomputing.co.uk/pyqt/" )

	slUOpen( uURL )

# URL: Python
def slUPython() :

	# Set URL
	uURL = QtCore.QUrl( "http://www.python.org/" )

	slUOpen( uURL )

# Go Task
def slGoTask() :
	id = Settings.Input.Task.sID = Main.uiMain.twTasks.item(Main.uiMain.twTasks.currentRow(), 0).text()	#.toInt()[0]
	uURL = QtCore.QUrl(Settings.Setting.Server + "index.php?menuaction=infolog.uiinfolog.edit&info_id=" + id)
	slUOpen( uURL )
