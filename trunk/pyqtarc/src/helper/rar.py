#!/bin/env python
# -*- coding: utf-8 -*-
'''
RAR helper
'''

import sys
from PyQt4 import QtCore
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('rar',)
mimes = ('application/x-rar', 'application/x-rar-compressed',)

class	ArchHelperRar(ArchHelper):
 
	@classmethod
	def	get_capabilities():
		return HCAN_LIST

	@classmethod
	def	get_list(self, archive):
		return 0, list()

mainclass = ArchHelperRar

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
