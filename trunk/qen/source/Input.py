"""
QEN:		Qt eGroupWare Notifier
Designed by Eugene A. Pivnev

File:	Input.py
Purpose:	Input arguments handler
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
	from	PyQt4	import QtCore
except :
	Import.slException( "PyQt4" )

# QEN 2
import	Main, Settings, Thread


### Slots

# Print QEN info
def slPQENInfo() :
	print ""
	print Settings.Application.sName + " " + Settings.Application.sVersion + ""
	print "Designed by " + Settings.Author.sName
	print ""
	print "QEN is a eGroupWare InfoLog Notifier, residing in the System Tray."
	print ""


# Print Usage
def slPUsage() :

	print "Usage: " + Settings.Application.sName + " [commands]"
	print ""
	print "commands"
	print ""
	print "	i: invisible"
	print "	v: verbose"
	print "	d: demonstration"
	print "	h: help"
	print ""


# Input arguments
def slArgTest() :

	# Supplied arguments
	if Settings.Input.iNumArgs >= 2 :

		# Help
		if "h" in sys.argv[1] :

			# QEN
			slPQENInfo()

			# Usage
			slPUsage()

			# Exit
			sys.exit()

		"""
		# Profile
		if "s" in sys.argv[1] :
			Profile.slProLStorage()
		else :
			Profile.slProLDefault()
		"""

		# Verbose
		if "v" in sys.argv[1] :

			Settings.Init.bVerbose = True

			slPQENInfo()

			print "OS = " + Settings.sOS
			print "$KDEDIR = " + Settings.Path.sKDE
			print "$HOME = " + Settings.Path.sUser
			print ""

		# Invisible
		if "i" in sys.argv[1] :
			Settings.Init.bInvisible = True

		# Demo
		if "d" in sys.argv[1] :
			Settings.Init.bDemo = True

	"""
	else :

		# Load 'Default' profile
		Profile.slProLDefault()
	"""
