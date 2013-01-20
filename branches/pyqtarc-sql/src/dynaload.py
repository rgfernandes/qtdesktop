#!/bin/env python
# -*- coding: utf-8 -*-
'''
7z helper. Load from archfile.ArchFile.load()
'''

PKG = 'helper'

import pkgutil, importlib

def test1(pkg):
	return [name for _, name, _ in pkgutil.iter_modules([pkg])]

def test2():
  package = __import__(PKG)
  return [module_name for module_name in dir(package) if not module_name.startswith("__")]

def test3():
	#print locals()
	#from helper import lzma, zip
	exec "from helper import %s" % ','.join(test1(PKG))
	a = locals()
	for i in a:
		print a[i].exts
		print a[i].mainclass
	#print lzma.exts

if __name__ == '__main__':
	#print ','.join(test1(PKG))
	test3()
	'''
	modules = test1(PKG)
	#print test2()
	for m in modules:
		if (m != 'base'):
			print m
			#locals()['a'] = __import__(PKG+'.'+m)
			a = importlib.import_module(PKG, m)
			print dir(a)
			print a.exts
	'''