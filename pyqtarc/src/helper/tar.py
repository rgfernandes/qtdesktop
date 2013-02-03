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
mimes = ('application/x-7z-compressed-tar', 'application/x-bzip-compressed-tar', 'application/x-bzip1-compressed-tar', 'application/x-compressed-tar', 'application/x-gtar', 'application/x-lrzip-compressed-tar', 'application/x-lzip-compressed-tar', 'application/x-lzma-compressed-tar', 'application/x-lzop-compressed-tar', 'application/x-tar', 'application/x-tarz', 'application/x-tzo', 'application/x-xz-compressed-tar',)

class	ArchHelperTar(ArchHelper):
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

mainclass = ArchHelperTar

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
