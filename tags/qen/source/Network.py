"""
QEN:		Qt eGroupWare Notifier
Designed by Eugene A. Pivnev

File:	Network.py
Purpose:	Network handler
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
import	Display, Main, Settings, Thread
"""
try :
	from	Main_ui			import Ui_Main
	from	Main_rc			import *
	from	Settings_ui		import Ui_Settings
	from	Settings_rc		import *
except :
	Import.slException( Settings.Application.sName )
"""

### Slots


# Start
def slStart() :

	#print "slStart"

	# Exit thread
	slExitControl()

	# Stop timer
	Main.tmRestart.stop()

	# Launched?
	if not Settings.Status.bLaunched :

		# Statusbar
		Main.uiMain.sbMain.showMessage( Settings.Message.sStarting )

		# Execute thread
		Thread.tiStart.start()
		Thread.tiStart.setPriority( QtCore.QThread.LowPriority )
		Thread.tiStart.msleep( 50 )

	else :

		# Display error
		Display.slErrorMessage( "Error: Could not connect server.")

		# Launch website
		Main.uiMain.aUQEN.trigger()


# Stop
def slStop() :

	#print "slStop"

	# Settings
	Settings.slSave()

	# Exits
	slExitUpdate()
	slExitConnect()
	slExitControl()

	# Statusbar
	Main.uiMain.sbMain.showMessage( Settings.Message.sStopping )

	# Execute thread
	Thread.tiStop.start()
	Thread.tiStop.setPriority( QtCore.QThread.LowPriority )
	Thread.tiStop.msleep( 50 )


# Connect
def slConnect() :

	# Exit threads
	slExitUpdate()
	slExitConnect()

	# Statusbar
	Main.uiMain.sbMain.showMessage( Settings.Message.sConnecting )

	# Execute thread
	Thread.tiLogin.start()
	Thread.tiLogin.setPriority( QtCore.QThread.LowPriority )
	Thread.tiLogin.msleep( 50 )


# Disconnect
def slDisconnect() :

	# Exit threads
	slExitUpdate()
	slExitConnect()

	# Statusbar
	Main.uiMain.sbMain.showMessage( Settings.Message.sDisconnecting )

	# Execute thread
	Thread.tiLogout.start()
	Thread.tiLogout.setPriority( QtCore.QThread.LowPriority )
	Thread.tiLogout.msleep( 50 )


# Status
def slStatus() :

	# Go online
	if Main.uiMain.pbOnline.isChecked() and not Settings.Status.bConnected :
		slConnect()
	# Go offline
	elif not Main.uiMain.pbOnline.isChecked() and Settings.Status.bConnected :
		slDisconnect()


def slTaskAccept() :
	# Statusbar
	Main.uiMain.sbMain.showMessage( Settings.Message.sTaskAccepting )

	# get value
	Settings.Input.Task.sID = Main.uiMain.twTasks.item(Main.uiMain.twTasks.currentRow(), 0).text().toInt()[0]		# PyQt feature
	#print Settings.Input.Task.sID
	# Execute thread
	Thread.tiTaskAccept.start()
	Thread.tiTaskAccept.setPriority( QtCore.QThread.LowPriority )
	Thread.tiTaskAccept.msleep( 50 )


# Task Accept Done
def slTaskAcceptDone( sError ) :

	print "Accept Done"
	# Exit threads
	slExitUpdate()

	# Display error
	Display.slErrorMessage( sError )

	# Update Display
	slUpdateList()


# Update List
def slUpdateList() :

	#print "slUpdateList"

	# Exit thread
	slExitUpdate()

	# Statusbar
	Main.uiMain.sbMain.showMessage( Settings.Message.sUpdating )

	# Online
	if Settings.Status.bConnected :

		# Execute thread
		Thread.tiUpdate.start()
		Thread.tiUpdate.setPriority( QtCore.QThread.LowPriority )
		Thread.tiUpdate.msleep( 50 )

	# Offline
	else :

		# Disable
		#Main.uiMain.twNetworks.clear()
		Main.uiMain.twTasks.setEnabled( False )

		# Update GUI
		Display.slUpdateGUI()


# Start Done
def slStartDone( sError ) :

	# Exit thread
	slExitControl()

	# Update GUI
	Display.slUpdateGUI()

	#Display.slErrorMessage( sError )

	# Show tray message?
	if Settings.Init.bHide and QtGui.QSystemTrayIcon.supportsMessages() :
		Main.stiMain.showMessage( Settings.Application.sName , Settings.Message.sStarting , QtGui.QSystemTrayIcon.Information , Settings.Timer.Control.iLaunched )

	if sError != "" :

		# Restart timer
		Main.tmRestart.start( Settings.Timer.Control.iRestart )

		Display.slErrorMessage( sError )

	else :

		if Settings.Init.bConnect :
			slConnect()

# Stop Done
def slStopDone( sError ) :

	#print "slStopDone"

	# Exits
	"""
	slExitNetwork()
	slExitUpdate()
	slExitConnect()
	"""
	slExitControl()

	# Update GUI
	Display.slUpdateGUI()

	# Display error
	Display.slErrorMessage( sError )


# Exit Done
def slExitDone() :

	#print "slExitDone"

	# Thread
	if Thread.tiStop.isRunning() :
		Thread.tiStop.exit()
		Thread.tiStop.wait( Settings.Timer.Process.iPause )

	# Hide system tray icon
	Main.stiMain.hide()

	# Close main window
	Main.mwMain.close()


# Connect Done
def slConnectDone( sError ) :

	#print "slConnectDone"

	# Exit thread
	#slExitUpdate()
	slExitConnect()

	# Update GUI
	Display.slUpdateGUI()

	Display.slErrorMessage( sError )

	if sError != "" :

		# Reconnect timer
		Main.tmReconnect.start( Settings.Timer.Control.iRestart )

	else :

		# Reconnect timer
		Main.tmReconnect.stop()

		# Update List
		slUpdateList()

	"""
	# Show tray message?
	if Settings.Init.bHide :
		Main.stiMain.showMessage( Settings.Application.sName , "Launched" , QtGui.QSystemTrayIcon.Information , Settings.iTLaunched )
	"""


# Update Done
def slUpdateListDone() :

	#print "slUpdateListDone"

	# Exit threads
	slExitUpdate()

	# Update Display
	Display.slUpdateList()
	Display.slUpdateGUI()

	# Update timer
	Main.tmUpdate.start( Settings.Setting.RefreshTime )


# Exit Control
def slExitControl() :

	#print "slExitControl"

	# End processes
	if Thread.tiStart.isRunning() :
		Thread.tiStart.Stop()
	if Thread.tiStop.isRunning() :
		Thread.tiStop.Stop()
		#print "slExitControl: Stop is Running"

	"""
	# End threads
	Thread.tiStart.exit()
	Thread.tiStop.exit()
	"""

# Exit Connect
def slExitConnect() :

	#print "slExitConnect"

	# End processes
	if Thread.tiLogin.isRunning() :
		Thread.tiLogin.Stop()
	if Thread.tiLogout.isRunning() :
		Thread.tiLogout.Stop()

	"""
	# End threads
	Thread.tiLogin.exit()
	Thread.tiLogout.exit()
	"""

# Exit Update
def slExitUpdate() :

	#print "slExitUpdate"

	# End process
	if Thread.tiUpdate.isRunning() :
		Thread.tiUpdate.Stop()

	"""
	# End thread
	Thread.tiUpdate.exit()
	"""
