#!/bin/env python
# -*- coding: utf-8 -*-
'''
TAR helper
'''

import sys
from PyQt4 import QtCore
from base import *

reload(sys)
sys.setdefaultencoding('utf-8')

exts = ('tar', 'gtar', 'gem', 'tar.gz', 'tgz', 'taz', 'tar.bz', 'tar.bz2', 'tbz', 'tbz2', 'tar.lrz', 'tlrz', 'tar.lzma', 'tlz', 'tar.lzo', 'tzo', 'tar.xz', 'txz',)
mimes = ('application/x-7z-compressed-tar', 'application/x-bzip-compressed-tar', 'application/x-bzip1-compressed-tar', 'application/x-compressed-tar', 'application/x-gtar', 'application/x-lrzip-compressed-tar', 'application/x-lzip-compressed-tar', 'application/x-lzma-compressed-tar', 'application/x-lzop-compressed-tar', 'application/x-tar', 'application/x-tarz', 'application/x-tzo', 'application/x-xz-compressed-tar',)

class	ArchHelperTar(ArchHelper):
 
	@classmethod
	def	get_capabilities():
		return HCAN_LIST

	@classmethod
	def	get_list(self, archive):
		return 0, list()

mainclass = ArchHelperTar

if (__name__ == '__main__'):
	mainclass.clitest(sys.argv)
