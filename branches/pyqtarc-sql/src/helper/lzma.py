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

	def	__exec(self, cmd, args):
		'''
		Execute external program
		@param cmd:str - command ("7za")
		@param args:QStringList
		@return errcode, stdout, stderr
		'''
		proc = QtCore.QProcess()
		proc.start(cmd, args)
		proc.waitForFinished(-1)
		return (
			proc.exitCode(),
			QtCore.QString.fromLocal8Bit(proc.readAllStandardOutput()),
			QtCore.QString.fromLocal8Bit(proc.readAllStandardError()),
		)

	def	list(self, path, files=[]):
		#print files
		errcode, out, err = self.__exec("7za", (QtCore.QStringList("l") << path) + files)
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

	def	add(self, apath, fpaths, skip):
		'''
		TODO: skip => mode (replace, update, skip)
		'''
		if skip:
			# 1. get archive list to set
			err, result = self.list(apath)
			if err:
				return err, "cant list archive"
			dst = set()
			for i in result:
				dst.add(i[0])
				if (not i[1]):	# file; workaround
					dst.add(QtCore.QFileInfo(i[0]).dir().path())
			# 2. walk through fs searching what _absent_ (top-down, cutting header (dirname))
			src = QtCore.QStringList()
			for fpath in fpaths:
				src += self.__get_extras(dst, fpath)
			cwd = QtCore.QDir.currentPath()
			QtCore.QDir.setCurrent(QtCore.QFileInfo(fpaths[0]).dir().path())
			errcode, out, err = self.__exec("7za", (QtCore.QStringList("a") << apath) + src)
			QtCore.QDir.setCurrent(cwd)
		else:
			errcode, out, err = self.__exec("7za", (QtCore.QStringList("a") << apath) + fpaths)
		#print out
		return (errcode, err)

	def	__get_extras(self, dst, srcpath):
		'''
		get extra dirs/files in srcpath
		@param dst:set - arc dirs/files
		@param srcpath:str - absolute FS path
		@return: QStringList - files to add (rdlative)
		'''
		base = QtCore.QFileInfo(srcpath).dir().path()
		cutlen = base.size() + 1
		retvalue = QtCore.QStringList()
		self.__walk(srcpath, cutlen, dst, retvalue)
		return retvalue

	def	__walk(self, path, cutlen, dirs, retvalue):
		'''
		@param path:str
		'''
		relname = path.mid(cutlen)
		if (not relname in dirs):
			dirs.add(relname)
		else:
			if (QtCore.QFileInfo(path).isDir()):
				for f in QtCore.QDir(path).entryInfoList():
					fp = f.fileName()
					if not (fp == "." or fp == ".."):
						self.__walk(f.absoluteFilePath(), cutlen, dirs, retvalue)

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
					#print "out", outfoldererrcode, out, err = self.__exec("7za", (QtCore.QStringList("d") << apath) + fpaths)
					errcode, out, err = self.__exec("7za", (QtCore.QStringList("e") << replacekey << "-o"+outfolder << apath << i[0]))
					#return (p.returncode, err)
		return 0, ''

	def	delete(self, apath, fpaths):
		errcode, out, err = self.__exec("7za", (QtCore.QStringList("d") << apath) + fpaths)
		return (errcode, err)

mainclass = ArchHelper7z
