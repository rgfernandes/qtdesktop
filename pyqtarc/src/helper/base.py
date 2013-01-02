# -*- coding: utf-8 -*-
'''
Helpers base:
* documentation
* constants
'''

import sys, os, re, datetime, subprocess

reload(sys)
sys.setdefaultencoding('utf-8')

class	ArchHelper:
	#exts = (,)
	#mimes = (,)

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
		return self.mimes

	@classmethod
	def	list(self, path, files=list()):
		'''
		List archive.
		@param path: full path to archive
		@type path: str
		@return: (name, isdir, mtime, size, csize)
		@rtype: list
		'''
		return (0, [])

	@classmethod
	def	add(self, apath, fpaths):
		'''
		@param apath: full path to archive
		@type path: str
		@param fpath: full path to file/folder to add
		@type fpath: str
		@return: errcode, errmsg
		@rtype: (int, str)
		'''
		#return (p.returncode, err)
		return (0, [])

	@classmethod
	def	extract(self, apath, fpaths, destdir):
		return (0, [])

	@classmethod
	def	delete(self, apath, fpaths):
		return (0, [])
