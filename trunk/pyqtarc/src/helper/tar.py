#!/bin/env python
# -*- coding: utf-8 -*-
'''
TAR helper
'''

import sys
from PyQt4 import QtCore
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('tar', 'gtar', 'gem', 'tar.gz', 'tgz', 'taz', 'tar.bz', 'tar.bz2', 'tbz', 'tbz2', 'tar.lrz', 'tlrz', 'tar.lzma', 'tlz', 'tar.lzo', 'tzo', 'tar.xz', 'txz',)
mimes = ('application/x-7z-compressed-tar', 'application/x-bzip-compressed-tar', 'application/x-bzip1-compressed-tar', 'application/x-compressed-tar', 'application/x-gtar', 'application/x-lrzip-compressed-tar', 'application/x-lzip-compressed-tar', 'application/x-lzma-compressed-tar', 'application/x-lzop-compressed-tar', 'application/x-tar', 'application/x-tarz', 'application/x-tzo', 'application/x-xz-compressed-tar',
'application/x-gzip', 'application/x-bzip2', 'application/x-xz',	# hack
)
# FIXME: shows application/x-gzip|x-bzip2|x-xz

class	ArchHelperTar(ArchHelper):
	#__rx = QtCore.QRegExp("(.)[rwx]{9} ^\\ * (\\d) (\\d{4}-\\d{2}-\\d{4} \\d{2}:\\d{2}) ([^\n]*)\n")
	__rx = QtCore.QRegExp("(.)[-rwx]{9} [^/]*/[^ ]* ([ \\d]*) (\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}) ([^\n]*)\n")

	@classmethod
	def	get_capabilities():
		return HCAN_LIST

	@classmethod
	def	get_list(self, archive):
		errcode, out, err = self.exec_cmd("tar", (QtCore.QStringList("tvf") << archive))
		retvalue = []
		pos = self.__rx.indexIn(out)
		while (pos != -1):
			#print "\"%s\" => \"%s\", \"%s\", \"%s\", \"%s\"" % (self.__rx.cap(0)[:-1], self.__rx.cap(1), self.__rx.cap(2), self.__rx.cap(3), self.__rx.cap(4))
			isdir = self.__rx.cap(1) == "d"
			retvalue.append((
				self.__rx.cap(4)[:-1] if isdir else self.__rx.cap(4),		#name
				isdir,	#isdir
				QtCore.QDateTime.fromString(self.__rx.cap(3), "yyyy-MM-dd hh:mm"),
				self.__rx.cap(2).trimmed().toULong()[0],
				0,
			))
			pos += self.__rx.matchedLength()
			pos = self.__rx.indexIn(out, pos)
		return (errcode, retvalue)		

mainclass = ArchHelperTar

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
