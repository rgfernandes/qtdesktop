"""
QEN:		Qt eGroupWare Notifier
Designed by Eugene A. Pivnev

File:	Settings.py
Purpose:	Settings handler
"""


### Imports

# QEN 1
import	Import

# Python
try :
	import	commands, os, platform, sys
except :
	Import.slException( "Python" )

# PyQt4
try :
	from	PyQt4	import QtCore
except :
	Import.slException( "PyQt4" )

# QEN 2
import	Help, Main, Network


### Variables

# Application
Application			= QtCore.QObject()
Application.sName		= "QEN"
Application.sVersion	= "0.0.1"
Application.sDomain	= "qen.sourceforge.net"
QtCore.QCoreApplication.setApplicationName( Application.sName )

# Author
Author				= QtCore.QObject()
Author.sName			= "Eugene A. Pivnev"
Author.Email			= QtCore.QObject()
Author.Email.sUser	= "ti.eugene"
Author.Email.sDomain	= "@Gmail.com"
QtCore.QCoreApplication.setOrganizationName( Author.sName )
QtCore.QCoreApplication.setOrganizationDomain( Application.sDomain )

# Operating system
sOS				= platform.system()

# Path
Path				= QtCore.QObject()
Path.sKDE		= os.getenv( 'KDEDIR' )
Path.sUser		= os.getenv( 'HOME' )
Path.sQEN		= ""
Path.sSep		= QtCore.QString( QtCore.QDir.separator() )

# Init
Init				= QtCore.QObject()
Init.bHide		= False
Init.bConnect		= True
Init.bVerbose		= False
Init.bInvisible	= False

# Status
Status				= QtCore.QObject()
Status.bLaunched		= False
Status.bConnected		= False
Status.oSession		= None
Status.lFreshData		= []		# hot data

# Timer
Timer					= QtCore.QObject()
Timer.Update				= QtCore.QObject()
#Timer.Update.iFreq		= None
Timer.Control				= QtCore.QObject()
Timer.Control.iRestart		= 5000				# 5 sec for restart on start (?)
Timer.Control.iLaunched	= 5000				# 5 sec for starting msg
Timer.Process				= QtCore.QObject()
Timer.Process.iPause		= 500				# timeout for stopping (Thread.tiStop)

# Display
#Display							= QtCore.QObject()
#Display.bHideOfflinePeers			= True
#Display.bAlternatingRowColours		= True

# Commands
Command					= QtCore.QObject()

# Icons
Icon						= QtCore.QObject()
Icon.Location				= QtCore.QObject()
Icon.Location.sApps		= ":/apps/icons/apps/"
Icon.Location.sActions		= ":/actions/icons/actions/"
Icon.Action				= QtCore.QObject()
Icon.Action.sOnline		= "led-green"
Icon.Action.sOffline		= "led-red"

# Message
Message					= QtCore.QObject()
Message.sStarting			= "Starting ..."
Message.sStopping			= "Stopping ..."
Message.sConnecting		= "Connecting ..."
Message.sDisconnecting		= "Disconnecting ..."
Message.sUpdating			= "Updating ..."
Message.sTaskAccepting		= "Task accepting ..."

# Input
Input					= QtCore.QObject()
Input.iNumArgs			= len(sys.argv)
Input.Task				= QtCore.QObject()
Input.Task.sID			= QtCore.QString()

# Default values
Default					= QtCore.QObject()
Default.Server			= "http://server/egroupware/"
Default.Login				= "user"
Default.Password			= "password"
Default.RefreshTime		= 600000	# QtCore.QTime(0, 10)	# 1h, 10m, 0s
#Default.UID

# Settings key names
Key						= QtCore.QObject()
Key.Server				= "server"
Key.Login				= "login"
Key.Password				= "password"
Key.RefreshTime			= "refresh"

# Setting
sSettings				= QtCore.QSettings()
Setting					= QtCore.QObject()
Setting.Server			= None
Setting.Login				= None
Setting.Password			= None
Setting.RefreshTime		= None


# Linux
if sOS == "Linux" :

	#if not Path.sKDE :
	#	Path.sKDE	= '/usr/'

	if not Path.sUser :
		Path.sUser	= '/home/' + os.getenv( 'USER' ) + '/'

	# Installation
	if QtCore.QFile.exists( Path.sKDE + "/share/apps/qen/Source/" ) :
		Path.sQEN	= Path.sKDE + "/share/apps/qen/"
	elif QtCore.QFile.exists( Path.sUser + "/.kde/share/apps/qen/Source/" ) :
		Path.sQEN	= Path.sUser + "/.kde/share/apps/qen/"
	else :
		Path.sQEN	= os.getcwd() + "/../"

	# Commands
	#Command.sSU				= "sudo"
	#Command.sSU				= "kdesu"

	Init.bHide		= True

	"""
	# Windows
	elif sOS == "Windows" :

		if not sHomeKDE :
			sHomeKDE	= ''

		if not sHomeUser :
			sHomeUser	= ''

		sReg		= QtCore.QSettings( "HKEY_LOCAL_MACHINE\\Software\\Archiving\\QEN" , QtCore.QSettings.NativeFormat )
		sHomeQEN	= sReg.value( "Path" ).toString()

		# Installation
		sInstQEN	= sHomeQEN

		# Commands
		sCmdSudo		= QtCore.QString( "sudo" )
		sCmdTunCfg	= QtCore.QString( "tuncfg" )
		sReg			= QtCore.QSettings( "HKEY_LOCAL_MACHINE\\Software\\Hamachi" , QtCore.QSettings.NativeFormat )
		sCmdHamachi	= sReg.value( "Path" ).toString() + "/Hamachi.exe"
	"""

# Others
else :

	print ""
	print "Unfortunately, your OS is not supported by " + Application.sName + " " + Application.sVersion + ".  Please email the author, specifying your system details."
	print ""

	sys.exit()


### Slots

# Load
def slLoad() :
	'''
	Load settings from config file or set default values.
	'''
	# Server
	if sSettings.contains(Key.Server) :
		Setting.Server = str(sSettings.value(Key.Server).toString())
	else :
		Setting.Server = Default.Server
	# Login
	if sSettings.contains(Key.Login) :
		Setting.Login = str(sSettings.value(Key.Login).toString())
	else :
		Setting.Login = Default.Login
	# Password
	if sSettings.contains(Key.Password) :
		Setting.Password = str(sSettings.value(Key.Password).toString())
	else :
		Setting.Password = Default.Password
	# RefreshTime
	if sSettings.contains(Key.RefreshTime) :
		Setting.RefreshTime = sSettings.value(Key.RefreshTime).toInt()[0]
	else :
		Setting.RefreshTime = Default.RefreshTime

# Save
def slSave() :
	'''
	Save settings from variables to config file.
	'''
	sSettings.setValue( Key.Server,      QtCore.QVariant(Setting.Server))
	sSettings.setValue( Key.Login,       QtCore.QVariant(Setting.Login))
	sSettings.setValue( Key.Password,    QtCore.QVariant(Setting.Password))
	sSettings.setValue( Key.RefreshTime, QtCore.QVariant(Setting.RefreshTime))


def __Time2MSec(t):
	'''
	Converts QTime into msec
	'''
	return (
		t.hour() * 3600000 +
		t.minute() * 60000 + 
		t.second() * 1000 +
		t.msec()
	)

def __MSec2Time(t):
	'''
	Converts msec into QTime
	'''
	h, u = divmod(t, 3600000)
	m, u = divmod(u,   60000)
	s, u = divmod(u,    1000)
	return QtCore.QTime ( h, m, s, u )

# Dialog
def slDialog() :
	'''
	Call Options dialog w/ settings.
	'''

	# set values
	Main.uiSettings.cbServerURL.setText(Setting.Server)
	Main.uiSettings.cbLogin.setText(Setting.Login)
	Main.uiSettings.cbPassword.setText(Setting.Password)
	Main.uiSettings.cbRefresh.setTime(__MSec2Time(Setting.RefreshTime))

	# Modal
	Main.dSettings.exec_()

	# Load
	slLoad()


# Accept
def slAccept() :
	'''
	Accept settings from Options dialog.
	'''
	# Server
	Setting.Server = Main.uiSettings.cbServerURL.text()
	sSettings.setValue( Key.Server , QtCore.QVariant(Setting.Server) )
	# Login
	Setting.Login = Main.uiSettings.cbLogin.text()
	sSettings.setValue( Key.Login , QtCore.QVariant(Setting.Login) )
	# Password
	Setting.Password = Main.uiSettings.cbPassword.text()
	sSettings.setValue( Key.Password , QtCore.QVariant(Setting.Password) )
	# RefreshTime
	Setting.RefreshTime = __Time2MSec(Main.uiSettings.cbRefresh.time())
	sSettings.setValue( Key.RefreshTime , QtCore.QVariant(Setting.RefreshTime ))

	# Update GUI
	#Display.slUpdateGUI()
	Network.slUpdateList()
