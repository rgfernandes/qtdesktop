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

	def	extract(self, apath, fpaths, destdir, skip):
		#print apath, fpaths, destdir
		replacekey = "-aos" if skip else "-aoa"
		for src in fpaths:
			srcdir = QtCore.QFileInfo(src).dir().path()	# src path parent - to cut from dst
			srclen = srcdir.size()
			err, dst = self.list(apath, [src,])	# get children
			# 1. mkdirs
			for i in dst:
				dstrel = i[0].mid(srclen+1) if (not srcdir.isEmpty()) else i[0]	# real relative dst path
				dstfolder = dstrel if i[1] else QtCore.QFileInfo(dstrel).dir().path()
				if dstfolder:	# not for files from root
					dstfolderabs = destdir + "/" + dstfolder
					if not QtCore.QFileInfo(dstfolderabs).exists():
						#print "mkdir", dstfolderabs
						QtCore.QDir.mkpath(dstfolderabs)
				#print i[0], i[1], srcdir, dstrel, os.path.dirname(dstrel)
			# 2. expand files
			for i in dst:
				if not i[1]:
					dstrel = i[0].mid(srclen+1) if (not srcdir.isEmpty()) else i[0]	# real relative dst path
					dstfolder = QtCore.QFileInfo(dstrel).dir().path()
					outfolder = destdir + "/" + dstfolder
					#print "out", outfoldererrcode, out, err = self.exec_cmd("7za", (QtCore.QStringList("d") << apath) + fpaths)
					errcode, out, err = self.exec_cmd("7za", (QtCore.QStringList("e") << replacekey << "-o"+outfolder << apath << i[0]))
					#return (p.returncode, err)
		return 0, ''

mainclass = ArchHelper7z
