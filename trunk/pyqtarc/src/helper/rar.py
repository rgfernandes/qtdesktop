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
			# skip head
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
			# body
		'''
		pos = self.__rx.indexIn(out)
		while (pos != -1):
			isdir = self.__rx.cap(4).endsWith("/")
			retvalue.append((
				self.__rx.cap(4).left(self.__rx.cap(4).length()-1) if isdir else self.__rx.cap(4),		#name
				isdir,	#isdir
				QtCore.QDateTime.fromString(self.__rx.cap(3), "MM-dd-yyyy hh:mm"),
				self.__rx.cap(1).trimmed().toULong()[0],
				self.__rx.cap(2).trimmed().toULong()[0],
			))
			pos += self.__rx.matchedLength()
			pos = self.__rx.indexIn(out, pos)
		'''
		return (errcode, retvalue)		

mainclass = ArchHelperRar

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
