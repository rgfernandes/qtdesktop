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
	__rx = re.compile("\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} [D.][R.][H.][S.][A.] [ 0-9]{12} [ 0-9]{12}  .*\n")

	@classmethod
	def	get_capabilities():
		return HCAN_LIST|HCAN_ADD|HCAN_EXTRACT|HCAN_DELETE

	def	list(self, path, files=[]):
		proc = QtCore.QProcess()
		args = QtCore.QStringList("l")
		args << path
		args += files
		print str(args)
		proc.start("7za", args)
		proc.waitForFinished(-1)
		out = QtCore.QString.fromLocal8Bit(proc.readAllStandardOutput())
		err = QtCore.QString.fromLocal8Bit(proc.readAllStandardError())
		print "Exit code:", proc.exitCode()
		print "Stdout:"
		print out
		print "Stderr:"
		print err
		return (0, [])
		

	def	old_list(self, path, files=[]):
		'''
		TODO: err handling: str, errcode
		'''
		p = subprocess.Popen(["7za", "l", path] + files, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out, err = p.communicate()
		if (p.returncode):
			return (p.returncode, err)
		retvalue = list()
		for match in self.__rx.finditer(out):
			s = match.group().rstrip("\n")
			retvalue.append((
					s[53:],	# name:str
					(s[20] == 'D'),		# isdir:bool
					datetime.datetime.strptime(s[:19], "%Y-%m-%d %H:%M:%S"),	# mtime:datetime
					long(s[26:38].lstrip()),	# size:long
					long(s[39:51].lstrip()) if s[39:51].lstrip() else 0,		# csize:long
				)
			)
		return (0, retvalue)

	def	__get_extras(self, dst, srcpath):
		'''
		get extra dirs/files
		@param dst:set - arc dirs/files
		@param srcpath:str - absolute FS path
		'''
		base = os.path.dirname(srcpath)
		cutlen = len(base)+1
		retvalue = list()
		for root, dirs, files in os.walk(srcpath):
			relroot = root[cutlen:]
			if relroot not in dst:
				#retvalue.append(root)
				retvalue.append(relroot)
				del dirs[:]
			else:
				for f in files:
					relfile = os.path.join(relroot, f)
					if relfile not in dst:
						#retvalue.append(os.path.join(root, f))
						retvalue.append(relfile)
		return retvalue

	def	add(self, apath, fpaths, skip):
		'''
		TODO: skip => mode (replace, update, skip)
		'''
		if skip:
			# 1. get archive list to set
			dst = set()
			err, result = self.list(apath)
			if err:
				return err, "cant list archive"
			for i in result:
				dst.add(i[0])
			# 2. walk through fs searching what _absent_ (top-down, cutting header (dirname))
			src = list()
			for fpath in fpaths:
				src.extend(self.__get_extras(dst, fpath))
			cwd = os.getcwd()
			os.chdir(os.path.dirname(fpaths[0]))
			p = subprocess.Popen(["7za", "a", apath,] + src, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
			cwd = os.getcwd()
		else:
			p = subprocess.Popen(["7za", "a", apath,] + fpaths, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out, err = p.communicate()
		#print out
		return (p.returncode, err)

	def	extract(self, apath, fpaths, destdir, skip):
		#print apath, fpaths, destdir
		replacekey = "-aos" if skip else "-aoa"
		for src in fpaths:
			srcdir = os.path.dirname(src)		# src path parent - to cut from dst
			err, dst = self.list(apath, [src,])	# get children
			# 1. mkdirs
			for i in dst:
				dstrel = i[0][len(srcdir)+1:] if srcdir else i[0]	# real relative dst path
				dstfolder = dstrel if i[1] else os.path.dirname(dstrel)
				if dstfolder:	# not for files from root
					dstfolderabs = os.path.join(destdir, dstfolder)
					if not os.path.exists(dstfolderabs):
						#print "mkdir", dstfolderabs
						os.makedirs(dstfolderabs)
				#print i[0], i[1], srcdir, dstrel, os.path.dirname(dstrel)
			# 2. expand files
			for i in dst:
				if not i[1]:
					dstrel = i[0][len(srcdir)+1:] if srcdir else i[0]	# real relative dst path
					dstfolder = os.path.dirname(dstrel)
					outfolder = os.path.join(destdir, dstfolder)
					#print "out", outfolder
					p = subprocess.Popen(["7za", "e", replacekey, "-o"+outfolder, apath, i[0]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
					out, err = p.communicate()
					if p.returncode:
						print err
					#return (p.returncode, err)
		return 0, ''

	@classmethod
	def	delete(self, apath, fpaths):
		p = subprocess.Popen(["7za", "d", apath] + fpaths, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out, err = p.communicate()
		return (p.returncode, err)

mainclass = ArchHelper7z

if __name__ == '__main__':
	helper = ArchHelper7z()
	for i in helper.list(sys.argv[1]):
		print i[0], i[1], i[2], i[3], i[4]
