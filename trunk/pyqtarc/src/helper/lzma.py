#!/bin/env python
# -*- coding: utf-8 -*-
'''
7z helper. Load from archfile.ArchFile.load()
'''

import sys, os, re, datetime, subprocess, pprint
from PyQt4 import QtCore
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('7z',)
mimes = ('application/x-7z-compressed',)

class	ArchHelper7z(ArchHelper):
	exts = ('7z',)
	mimes = ('application/x-7z-compressed',)
	#                  date              time              attrs                size       csize       name
	__rx = QtCore.QRegExp("(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}) ([D.][R.][H.][S.][A.]) ([ 0-9]{12}) ([ 0-9]{12})  ([^\n]*)\n")
	#__rx = re.compile("\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} [D.][R.][H.][S.][A.] [ 0-9]{12} [ 0-9]{12}  .*\n")

	@classmethod
	def	get_capabilities():
		return HCAN_LIST|HCAN_ADD|HCAN_EXTRACT|HCAN_DELETE

	@classmethod
	def	list(self, archive, files=[]):
		errcode, out, err = self.exec_cmd("7za", (QtCore.QStringList("l") << archive) + files)
		retvalue = []
		pos = self.__rx.indexIn(out)
		while (pos != -1):
			retvalue.append((
				self.__rx.cap(5),		#name
				self.__rx.cap(2)[0]=='D',	#isdir
				QtCore.QDateTime.fromString(self.__rx.cap(1), "yyyy-MM-dd hh:mm:ss"),
				self.__rx.cap(3).trimmed().toULong()[0],
				self.__rx.cap(4).trimmed().toULong()[0] or 0L,
			))
			pos += self.__rx.matchedLength()
			pos = self.__rx.indexIn(out, pos)
		return (errcode, retvalue)		

	@classmethod
	def	delete(self, archive, fpaths):
		return self.exec_cmd("7za", (QtCore.QStringList("d") << archive) + fpaths)

	@classmethod
	def	add(self, archive, absprefix, relpaths):
		'''
		TODO: skip => mode (replace, update, skip)
		@param apath:QString - absolute basedir of entries to add
		@param fpaths:QStringList - relative paths of entries to add
		'''
		cwd = QtCore.QDir.currentPath()
		QtCore.QDir.setCurrent(absprefix)
		errcode, out, err = self.exec_cmd("7za", QtCore.QStringList("a") << archive << relpaths)
		QtCore.QDir.setCurrent(cwd)
		return (errcode, out, err)

	def	extract(self, archive, srcfolder, dstfolder, fpaths, skip):
		'''
		@param archive:QString - path of archive
		@param srcfolder:QString - parent archive folder of extractable entries
		@param dstfolder:QString - filesystem folder extract to
		@param fpaths:QStringList - entries to extract
		@param skip:bool - dummy
		'''
		tocut = srcfolder.length() + 1 if srcfolder.length() else 0
		replacekey = "-aos" if skip else "-aoa"
		for src in fpaths:
			dstpath = QtCore.QFileInfo(dstfolder + "/" + src.mid(tocut))
			outfolder = dstpath.absolutePath()
			#print dstdir, dstpath.fileName()
			errcode, out, err = self.exec_cmd("7za", (QtCore.QStringList("e") << replacekey << "-o"+outfolder << archive << src))
		return errcode, out, err

mainclass = ArchHelper7z
