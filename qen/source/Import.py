"""
QEN:		Qt eGroupWare Notifier
Designed by Eugene A. Pivnev

File:	Import.py
Purpose:	Importation handler
"""


### Imports

# Python
try :
	import	sys
except :
	#slException( "Python" )

	print ""
	print "The required 'Python' modules could not be found.  See 'Doc/INSTALL.txt' for more info."
	print ""

	sys.exit()

# QEN
#import	Settings


### Slots

def slException( sAppName ) :

	print ""
	print "The required '" + sAppName + "' modules could not be found.  See 'Doc/INSTALL.txt' for more info."
	print ""

	# QEN
	if sAppName == "QEN" :
	#if sAppName == Settings.Application.sName :
		print "You must run \"cd ../Build/ ; make ; cd ../Source/\" first."
		print ""

	sys.exit()
