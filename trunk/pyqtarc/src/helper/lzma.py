#!/bin/env python
# -*- coding: utf-8 -*-
'''
7z helper. Load from archfile.ArchFile.load()
'''

import sys, os, re, datetime, subprocess, pprint
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

	def	list(self, path, files=list()):
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

	def	add(self, apath, fpaths):
		p = subprocess.Popen(["7za", "a", apath] + fpaths, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out, err = p.communicate()
		if (p.returncode):
			return (p.returncode, err)
		fnames = map(os.path.basename, fpaths)
		return self.list(apath, fnames)

	@classmethod
	def	extract(self, apath, fpaths, destdir):
		p = subprocess.Popen(["7za", "e", "-o"+destdir, apath] + fpaths, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out, err = p.communicate()
		return (p.returncode, err)

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
