#!/bin/env python
# -*- coding: utf-8 -*-
'''
7z helper. Load from archfile.ArchFile.load()
'''

import sys, os, re, datetime, subprocess, pprint
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('zip',)
mimes = ('application/x-zip', 'application/x-zip-compressed', 'application/zip',)

class	ArchHelperZip(ArchHelper):
	exts = ('7z',)
	mimes = ('application/x-7z-compressed',)
	#                  date              time              attrs                size       csize       name
	__rx = re.compile("\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} [D.][R.][H.][S.][A.] [ 0-9]{12} [ 0-9]{12}  .*\n")
	#                  size        meth csize     compressn  date              time        crc          name
	__rx = re.compile("[ 0-9]{8}  .{6} [ 0-9]{8} [ 0-9]{3}% \d{2}-\d{2}-\d{4} \d{2}:\d{2} [0-9a-f]{8}  .*\n")
 
	@classmethod
	def	get_capabilities():
		return HCAN_LIST

	def	list(self, path, files=list()):
		'''
		TODO: common list with re and handler (callback)
		'''
		p = subprocess.Popen(["unzip", "-v", path] + files, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out, err = p.communicate()
		if (p.returncode):
			return (p.returncode, err)
		retvalue = list()
		for match in self.__rx.finditer(out):
			s = match.group().rstrip("\n")
			retvalue.append((
					s[58:].rstrip('/'),	# name:str
					(s.endswith('/')),	# isdir:bool
					datetime.datetime.strptime(s[31:47], "%m-%d-%Y %H:%M"),	# mtime:datetime
					long(s[:8].lstrip()),		# size:long
					long(s[17:25].lstrip()),	# csize:long
				)
			)
		return (0, retvalue)

mainclass = ArchHelperZip

if __name__ == '__main__':
	helper = ArchHelperZip()
	err, values = helper.list(sys.argv[1])
	for i in values:
		print i[0], i[1], i[2], i[3], i[4]
