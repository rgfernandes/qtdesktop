#!/bin/env python
# -*- coding: utf-8 -*-
'''
7z helper. Load from archfile.ArchFile.load()
TODO:
* One helper class+
* 5 callbacks(str) - name, isdir, mtime, size, csize
'''

import sys, os, re, datetime, subprocess, pprint

reload(sys)
sys.setdefaultencoding('utf-8')

class	ArchHelper7z:
	exts = ('7z',)
	mimes = ('application/x-7z-compressed',)

	def	__init__(self):
		self.__rx = re.compile("\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} [D.][R.][H.][S.][A.] [ 0-9]{12} [ 0-9]{12}  .*\n")

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

	def	list(self, path, files=tuple()):
		'''
		List archive.
		TODO: err handling: str, errcode
		@param path: full path to archive
		@type path: str
		@return: (name, isdir, mtime, size, csize)
		@rtype: list
		'''
		p = subprocess.Popen(["7za", "l", path] + list(files), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
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

	def	add(self, apath, fpath):
		'''
		@param apath: full path to archive
		@type path: str
		@param fpath: full path to file/folder to add
		@type fpath: str
		@return: errcode, errmsg
		@rtype: (int, str)
		'''
		p = subprocess.Popen(["7za", "a", apath, fpath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out, err = p.communicate()
		if (p.returncode):
			return (p.returncode, err)
		fname = os.path.basename(str(fpath))
		return self.list(str(apath), (fname,))

if __name__ == '__main__':
	helper = ArchHelper7z()
	for i in helper.list(sys.argv[1]):
		print i[0], i[1], i[2], i[3], i[4]
