#!/bin/env python
# -*- coding: utf-8 -*-
'''
ARJ helper
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
		return 0, list()

mainclass = ArchHelperArj

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
