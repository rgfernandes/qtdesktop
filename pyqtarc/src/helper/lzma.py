#!/bin/env python
# -*- coding: utf-8 -*-
'''
7z helper
'''

import sys, re, datetime, subprocess, pprint

reload(sys)
sys.setdefaultencoding('utf-8')

class	ArchHelper7z:
	exts = ('7z',)
	mimes = ('application/x-7z-compressed',)

	@classmethod
	def	get_ext():
		return self.exts

	@classmethod
	def	get_mime():
		return self.mimes

	def	__init__(self):
		self.__rx = re.compile("\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} [D.][R.][H.][S.][A.] [ 0-9]{12} [ 0-9]{12}  .*\n")

	def	list(self, path):
		'''
		@param path: full path to archive
		@type path: str
		@param root: where to load to
		@type root: ArchItemFolder
		@return: (name, isdir, mtime, size, csize)
		@rtype: list
		'''
		p = subprocess.Popen(["7za", "l", path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out, err = p.communicate()
		retvalue = list()
		for match in self.__rx.finditer(out):
			s = match.group()
			retvalue.append((
					s[53:].rstrip("\n"),
					(s[20] == 'D'),
					datetime.datetime.strptime(s[:19], "%Y-%m-%d %H:%M:%S"),
					long(s[26:38].lstrip()),
					long(s[39:51].lstrip()) if s[39:51].lstrip() else 0,
				)
			)
		return retvalue

if __name__ == '__main__':
	helper = ArchHelper7z()
	for i in helper.list(sys.argv[1]):
		print i[0], i[1], i[2], i[3], i[4]
