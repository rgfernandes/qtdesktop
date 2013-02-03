#!/bin/env python
# -*- coding: utf-8 -*-
'''
ZIP helper
'''

import sys
from PyQt4 import QtCore
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('zip',)
mimes = ('application/x-zip', 'application/x-zip-compressed', 'application/zip',)

class	ArchHelperZip(ArchHelper):
	__rx = QtCore.QRegExp("([ 0-9]{8})  .{6} ([ 0-9]{8}) [ 0-9]{3}% (\\d{2}-\\d{2}-\\d{4} \\d{2}:\\d{2}) [0-9a-f]{8}  ([^\n]*)\n")
 
	@classmethod
	def	get_capabilities():
		return HCAN_LIST

	@classmethod
	def	get_list(self, archive):
		errcode, out, err = self.exec_cmd("unzip", (QtCore.QStringList("-v") << archive))
		retvalue = []
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
		return (errcode, retvalue)		

mainclass = ArchHelperZip

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
