from PyQt4 import QtCore
import architem
from helper.lzma import ArchHelper7z
import os

class	ArchFile:
	'''
	Archive file representation
	'''

	def	__init__(self):
		self.__root = architem.ArchItemFolder("")
		self.__file = None
		self.__helper = None

	# from model interface
	def	getRoot(self):
		return self.__root

	def	sort(self):
		self.__root.getChildren().sort()

	def	List(self):
		return self.__root.getChildren()

	# from main window interface
	def	__list(self, result):
		'''
		TODO: err handling
		'''
		for i in result:
			# name:str, isdir:bool, mtime:datetime, size:int
			self.__root.addChildRecursive(i[0].split("/"), i[1], QtCore.QDateTime(i[2]), i[3])
		self.__root.getChildren().sort()

	def	load(self, path):
		'''
		TODO: choosing helper
		'''
		self.__file = path
		self.__helper = ArchHelper7z()
		errcode, result = self.__helper.list(path)
		if (errcode):
			return
		return self.__list(result)

	def	add(self, path):
		'''
		get file name
		try to add
		get file info
		add ArchItemFile
		TODO: same as load
		TODO: folder
		'''
		f = QtCore.QFileInfo(path)
		errcode, result = self.__helper.add(self.__file, path)
		if errcode:
			return
		return self.__list(result)
