#!/bin/env python
# -*- coding: utf-8 -*-
'''
LHA helper
'''

import sys
from PyQt4 import QtCore
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('lha', 'lzh')
mimes = ('application/x-lharc',)

def	lhadt(s):
	# QtCore.QDateTime.fromString(line[41:58], "yy-MM-dd hh:mm:ss").addYears(100)
	if (s[9] == ':'):
		fmt = "MMM d  hh:mm"
	else:
		fmt = "MMM d  yyyy"
	return QtCore.QDateTime.fromString(s, fmt)

class	ArchHelperLha(ArchHelper):
 
	@classmethod
	def	get_capabilities():
		return HCAN_LIST

	@classmethod
	def	get_list(self, archive):
		errcode, out, err = self.exec_cmd("lha", (QtCore.QStringList("vq") << archive))
		print out
		retvalue = list()
		for line in out.split('\n'):
			retvalue.append((
				line[70:],
				line[0] == 'd',
				lhadt(line[57:69]),
				line[31:38].trimmed().toULong()[0],	# size
				line[23:30].trimmed().toULong()[0],	# csize
			))
		return (errcode, retvalue)		

mainclass = ArchHelperLha

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
