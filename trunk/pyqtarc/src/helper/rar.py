#!/bin/env python
# -*- coding: utf-8 -*-
'''
RAR helper
'''

import sys
from PyQt4 import QtCore
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('rar',)
mimes = ('application/x-rar', 'application/x-rar-compressed',)

class	ArchHelperRar(ArchHelper):
	__listend = "-------------------------------------------------------------------------------"
 
	@classmethod
	def	get_capabilities():
		return HCAN_LIST

	@classmethod
	def	get_list(self, archive):
		errcode, out, err = self.exec_cmd("unrar", (QtCore.QStringList("v") << archive))
		retvalue = []
		if (not errcode) and (not err):
			skipmode = True
			odd = True
			for s in out.split("\n"):
				if skipmode:
					if (s == self.__listend):
						skipmode = False
				else:
					if (s == self.__listend):
						break
					else:
						if (odd):
							name = s[1:]
						else:
							#print "\"%s\"" % s[52]
							retvalue.append((
								name,		#name
								s[52] == 'd',	#isdir
								QtCore.QDateTime.fromString(s[37:51], "dd-MM-yy hh:mm").addYears(100),
								s[:22].trimmed().toULong()[0],
								s[23:31].trimmed().toULong()[0],
							))
						odd = not odd
		return (errcode, retvalue)		

mainclass = ArchHelperRar

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
