# -*- coding: utf-8 -*-
'''
Helpers base:
* documentation
* constants
Helper is framework independent and can be called from cli
'''

from PyQt4 import QtCore

import sys, os, re, datetime, subprocess

reload(sys)
sys.setdefaultencoding('utf-8')

HCAN_LIST	= 1
HCAN_ADD	= 2
HCAN_ADDTO	= 4
HCAN_EXTRACT	= 8
HCAN_DELETE	= 16
HCAN_RENAME	= 32
HCAN_MKDIR	= 64
HCAN_TEST	= 128
# TODO: solid, level, method, multivolume

exts = tuple()
mimes = tuple()

class	ArchHelper:
	#exts = (,)
	#mimes = (,)

	@classmethod
	def	exec_cmd(self, cmd, args):
		'''
		Execute external program
		@param cmd:str - command ("7za")
		@param args:QStringList (archive name, options, filenames etc)
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

	@classmethod
	def	get_ext():
		'''
		Get file extensions supported
		@return: extensions
		@rtype: list
		'''
		return self.exts

	@classmethod
	def	get_mime():
		'''
		Get file mimetypes supported
		@return: extensions
		@rtype: list
		'''
		return self.mimes

	@classmethod
	def	get_capabilities():
		'''
		Get handler capabilities
		@return: extensions
		@rtype: list
		'''
		return 0

	@classmethod
	def	get_list(self, archive, files=list()):
		'''
		List archive.
		@param archive: full path to archive
		@type archive: str
		@return: (name, isdir, mtime, size, csize)
		@rtype: list
		'''
		return (0, [])

	@classmethod
	def	delete(self, archive, fpaths):
		'''
		@param archive: full path to archive
		@type archive: str
		@param fpath: entry names to delete
		@type fpath: QStringList
		@return: errcode, stdout, stderr
		@rtype: (int, str, str)
		'''
		return (0, '', '')

	@classmethod
	def	add(self, archive, absprefix, relpaths):
		'''
		@param archive: absolute path of archive
		@type archive: str
		@param absprefix: absolute path of folders which entries need to add
		@type absprefix: str
		@param relpaths: relative path of files/folders to add
		@type relpaths: QStringList
		@return: errcode, stdout, stderr
		@rtype: (int, str, str)
		'''
		return (0, '', '')

#	@classmethod
#	def	addto(self, apath, dst, fpaths):
#		'''
#		Add items to selected archive folder
#		'''
#		return (0, [])

	@classmethod
	def	extract(self, apath, fpaths, destdir):
		'''
		TODO: chg destdir and files
		'''
		return (0, [])

#	@classmethod
#	def	rename(self, apath, src, dst):
#		return (0, [])

#	@classmethod
#	def	mkdir(self, apath, name):
#		return (0, [])

	@classmethod
	def	test(self, apath):
		return (0, [])

	@classmethod
	def	clitest(self, argv):
		if (len(argv) != 2):
			print "Usage: %s <filename>" % argv[0]
		else:
			errcode, retvalue = self.get_list(argv[1])
			if retvalue:
				print "D datetime            nsize      csize      name"
				print "- ------------------- ---------- ---------- -..."
				for i in retvalue:
					print "%s %s %10d %10d %s" % (
						'D' if(i[1]) else '-',
						i[2].toString("yyyy-MM-dd hh:mm:ss"),
						i[3],
						i[4],
						i[0]
					)

mainclass = ArchHelper
