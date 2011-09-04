"""
QEN:		Qt eGroupWare Notifier
Designed by Eugene A. Pivnev

File:	Main.py
Purpose:	Controller
"""


### Imports

# QEN 1
import	Import

# Python
try :
	import	sys
except :
	Import.slException( "Python" )

# PyQt4
try :
	from	PyQt4	import QtCore, QtGui
except :
	Import.slException( "PyQt4" )

# QEN 2
import	Display, Help, Input, Network, Settings, Thread
try :
	#from	Main_ui			import Ui_Main
	from	Ui_Main			import Ui_Main
	from	Main_rc			import *
	#from	Settings_ui		import Ui_Settings
	from	Ui_Settings		import Ui_Settings
	from	Settings_rc		import *
except :
	print "except"
	#Import.slException( Settings.Application.sName )


### Variables

# Main
aMain			= QtGui.QApplication( sys.argv )
mwMain			= QtGui.QMainWindow()
uiMain			= Ui_Main()
uiMain.setupUi( mwMain )

# Settings
dSettings		= QtGui.QDialog()
uiSettings		= Ui_Settings()
uiSettings.setupUi( dSettings )


### Slots


### Signals

# Actions
uiMain.aStaConnect.connect( uiMain.aStaConnect, QtCore.SIGNAL( "triggered()" ), Network.slConnect )
uiMain.aStaDisconnect.connect( uiMain.aStaDisconnect, QtCore.SIGNAL( "triggered()" ), Network.slDisconnect )
uiMain.aStaExit.connect( uiMain.aStaExit, QtCore.SIGNAL( "triggered()" ), Network.slStop )

uiMain.aOptSettings.connect( uiMain.aOptSettings, QtCore.SIGNAL( "triggered()" ), Settings.slDialog )

#uiMain.aTaskAccept.connect( uiMain.aTaskAccept, QtCore.SIGNAL( "triggered()" ), Network.slTaskAccept )
uiMain.aTaskAccept.connect( uiMain.aTaskAccept, QtCore.SIGNAL( "triggered()" ), Help.slGoTask )

# About
uiMain.aAQEN.connect( uiMain.aAQEN, QtCore.SIGNAL( "triggered()" ), Help.slAboutQEN )
uiMain.aAQt4.connect( uiMain.aAQt4, QtCore.SIGNAL( "triggered()" ), Help.slAboutQt4 )

# URLs
uiMain.aUQEN.connect( uiMain.aUQEN, QtCore.SIGNAL( "triggered()" ), Help.slUQEN )
uiMain.aUQt4.connect( uiMain.aUQt4, QtCore.SIGNAL( "triggered()" ), Help.slUQt4 )
uiMain.aUPyQt4.connect( uiMain.aUPyQt4, QtCore.SIGNAL( "triggered()" ), Help.slUPyQt4 )
uiMain.aUPython.connect( uiMain.aUPython, QtCore.SIGNAL( "triggered()" ), Help.slUPython )

# Push Buttons
uiMain.pbOnline.connect( uiMain.pbOnline, QtCore.SIGNAL( "clicked()" ), Network.slStatus )
uiSettings.pbAccept.connect( uiSettings.pbAccept, QtCore.SIGNAL( "clicked()" ), Settings.slAccept )

# System Tray Icon
stiMain.connect( stiMain, QtCore.SIGNAL( "activated(QSystemTrayIcon::ActivationReason)" ), Display.slMainWindow )

# Timer
tmRestart.connect( tmRestart, QtCore.SIGNAL( "timeout()" ), Network.slStart )
tmReconnect.connect( tmReconnect, QtCore.SIGNAL( "timeout()" ), Network.slConnect )
tmUpdate.connect( tmUpdate, QtCore.SIGNAL( "timeout()" ), Network.slUpdateList )

# Threads
Thread.tiStart.connect( Thread.tiStart, QtCore.SIGNAL( "done(QString)" ), Network.slStartDone )
Thread.tiStop.connect( Thread.tiStop, QtCore.SIGNAL( "done(QString)" ), Network.slStopDone )
Thread.tiLogin.connect( Thread.tiLogin, QtCore.SIGNAL( "done(QString)" ), Network.slConnectDone )
Thread.tiLogout.connect( Thread.tiLogout, QtCore.SIGNAL( "done(QString)" ), Network.slConnectDone )
Thread.tiUpdate.connect( Thread.tiUpdate, QtCore.SIGNAL( "done(QString)" ), Network.slUpdateListDone )
Thread.tiTaskAccept.connect( Thread.tiTaskAccept, QtCore.SIGNAL( "done(QString)" ), Network.slTaskAcceptDone )

Thread.tiUpdate.connect( Thread.tiUpdate, QtCore.SIGNAL( "disconnected()" ), Network.slConnect )

Thread.tiStop.connect( Thread.tiStop, QtCore.SIGNAL( "stopped()" ), Network.slExitDone )


### Execution

# Input arguments
Input.slArgTest()

# Display
Display.slInit()
Settings.slLoad()

# Start services
Network.slStart()

# Temporary
