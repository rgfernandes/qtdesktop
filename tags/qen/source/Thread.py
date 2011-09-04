"""
QEN:		Qt eGroupWare Notifier
Designed by Eugene A. Pivnev

File:	Thread.py
Purpose:	Thread handler
"""


### Imports

# QEN 1
import	Import

# PyQt4
try :
	from	PyQt4	import QtCore
except :
	Import.slException( "PyQt4" )

# eGW
try :
	import egroupware
except :
	Import.slException( "egroupware" )

# QEN 2
import	Main, Settings

### Threads

# Start
class tStart( QtCore.QThread ) :

	def Start( self ) :
		if Settings.Init.bVerbose is True :
			print "tStart.Start: Beginning"
		Settings.Status.bLaunched = True
		self.emit( QtCore.SIGNAL( "done(QString)" ) , "" )

	def Stop( self ) :

		# Verbose
		if Settings.Init.bVerbose is True :
			print "tStart.Stop: Beginning"

		# Signal
		self.emit( QtCore.SIGNAL( "stopped()" ) )

		# Exit
		self.exit()

	def run( self ) :

		self.Start()
		# Start event loop
		self.exec_()


# Stop
class tStop( QtCore.QThread ) :
	'''
	Stops egw session
	'''
	def Stop( self ) :

		# Verbose
		if Settings.Init.bVerbose is True :
			print "tStop.Stop: Beginning"

		# Signal
		self.emit( QtCore.SIGNAL( "stopped()" ) )

		# Exit
		self.exit()

	def run( self ) :

		if Settings.Init.bVerbose is True :
			print "tStop.run: Initiated"

		Settings.Status.bLaunched = False

		# Signal
		self.emit( QtCore.SIGNAL( "done(QString)" ) , "" )

		# Start event loop
		self.exec_()


# Login
class tLogin( QtCore.QThread ) :

	'''
	Starts egw session
	'''
	def Login( self ) :
		if Settings.Init.bVerbose is True :
			print "tLogin.Login: Initiating"
		#data = ['http://server/egroupware', 'eugene', 'password']
		try:
			Settings.Status.oSession = egroupware.getsession(
				Settings.Setting.Server,
				Settings.Setting.Login,
				Settings.Setting.Password
			)
			Settings.Status.bConnected = True
			if Settings.Init.bVerbose is True :
				print "tStart.EGWLogin: Succeeded."
		except:
			Settings.Status.bConnected = False
			if Settings.Init.bVerbose is True :
				print "tStart.EGWLogin: Filed."
		self.emit( QtCore.SIGNAL( "done(QString)" ) , "" )
		return

	def Stop( self ) :

		# Verbose
		if Settings.Init.bVerbose is True :
			print "tLogin.Stop: Beginning"

		# Signal
		self.emit( QtCore.SIGNAL( "stopped()" ) )

		# Exit
		self.exit()

	def run( self ) :

		# Go online
		self.Login()

		# Update
		#slUpdateList()

		# Start event loop
		self.exec_()


# Logout
class tLogout( QtCore.QThread ) :

	def Logout( self ) :
		Settings.Status.oSession = None
		Settings.Status.bConnected = False
		self.emit( QtCore.SIGNAL( "done(QString)" ) , "" )
		return

	def Stop( self ) :

		# Verbose
		if Settings.Init.bVerbose is True :
			print "tLogout.Stop: Beginning"

		# Signal
		self.emit( QtCore.SIGNAL( "stopped()" ) )

		# Exit
		self.exit()

	def run( self ) :

		# Go offline
		self.Logout()

		# Start event loop
		self.exec_()


# Update
class tUpdate( QtCore.QThread ) :

	def	GetTasks ( self ) :
		'''
		Get tasks from InfoLog
		'''
		# Verbose
		if Settings.Init.bVerbose is True :
			print "tUpdate.GetTasks: Beginning"

		ok = False
		if (Settings.Status.bConnected) :
			try:
				tasks = Settings.Status.oSession.getinfolog({"info_type": "task", "info_status": "not-started"})
				sOutput = ""
				ok = True
			except:
				sOutput = "Can't get data from server"
			if (ok):
				Settings.Status.lFreshData = []
				for task in tasks:
#					if (task['status'] == 'offer'):
					Settings.Status.lFreshData.append((task['id'], task['subject'], task['des']))
		else:
			sOutput = "Not connected to server."
		if Settings.Init.bVerbose is True :
			print "tUpdate.GetTasks: Succeeded."
			if (sOutput):
				print sOutput
		self.emit( QtCore.SIGNAL( "done(QString)" ) , sOutput )

	def Stop( self ) :

		# Signal
		self.emit( QtCore.SIGNAL( "stopped()" ) )

		# Exit
		self.exit()

	def run( self ) :

		# Verbose
		if Settings.Init.bVerbose is True :
			print "tUpdate.run: Beginning"

		self.GetTasks()

		# Start event loop
		self.exec_()


class tTaskAccept( QtCore.QThread ) :

	def Accept( self ) :
		if Settings.Init.bVerbose is True :
			print "tTaskAccept.Accept: Beginning"

		ok = False
		if (Settings.Status.bConnected) :
			#print "Accepting %d" % Settings.Input.Task.sID
			try:
				ok = Settings.Status.oSession.accepttask({"info_id": Settings.Input.Task.sID, "info_status": "not-started"})
				#ok = True	# FIXME:
				sOutput = ""
			except:
				sOutput = "Can't put data to server"
#			if Settings.Init.bVerbose is True :
#				print "Result: " % ok
		else:
			sOutput = "Not connected to server."
		if Settings.Init.bVerbose is True :
			print "tUpdate.GetTasks: Succeeded."
			if (sOutput):
				print sOutput
		self.emit( QtCore.SIGNAL( "done(QString)" ) , sOutput )

	def Stop( self ) :

		# Signal
		self.emit( QtCore.SIGNAL( "stopped()" ) )

		# Exit
		self.exit()

	def run( self ) :

		self.Accept()

		# Start event loop
		self.exec_()

### Variables

# Thread instances

tiStart			= tStart()
tiStop			= tStop()
tiLogin			= tLogin()
tiLogout			= tLogout()
tiUpdate			= tUpdate()
tiTaskAccept		= tTaskAccept()


### Slots
