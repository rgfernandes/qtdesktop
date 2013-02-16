#!/bin/env python
# -*- coding: utf-8 -*-
'''
ARJ helper
Use arj -v for listing, because -l not shows folders.
Note: arj a not add empty folders
FIME: cyr
'''

import sys
from PyQt4 import QtCore
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('arj',)
mimes = ('application/x-arj',)

class	ArchHelperArj(ArchHelper):
 
	@classmethod
	def	get_capabilities():
		return HCAN_LIST

	@classmethod
	def	get_list(self, archive):
		errcode, out, err = self.exec_cmd("unarj", (QtCore.QStringList("v") << archive))
		#out = QtCore.QString.fromLocal8Bit(out)
		retvalue = []
		state = False	# 0 - wait for start listing; 1 - listing (wait for end)
		subline = 0	# 0..3 - line in a file description
		for line in out.split('\n'):
			if (not state):
				if (line == '------------ ---------- ---------- ----- ----------------- -------------- -----'):
					state = True
					continue
			else:
				if (line == '------------ ---------- ---------- -----'):
					break
				else:
					if (subline == 0):
						filename = line[5:]
					elif (subline == 1):
						retvalue.append((
							filename,
							False,
							QtCore.QDateTime.fromString(line[41:58], "yy-MM-dd hh:mm:ss").addYears(100),
							line[13:23].trimmed().toULong()[0],	# size
							line[24:34].trimmed().toULong()[0],	# csize
						))
					subline = (subline + 1) & 3
		return (errcode, retvalue)		

mainclass = ArchHelperArj

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
