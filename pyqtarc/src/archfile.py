from PyQt4 import QtCore
import architem
from helper.lzma import ArchHelper7z

class	ArchFile:
	'''
	Archive file representation
	'''

	def	__init__(self):
		self.__root = architem.ArchItemFolder("")

	def	getRoot(self):
		return self.__root

	def	sort(self):
		self.__root.getChildren().sort()

	def	load(self, path):
		helper = ArchHelper7z()
		for i in helper.list(path):
			self.__root.addChildRecursive(i[0].split("/"), i[1], QtCore.QDateTime(i[2]), i[3])	# name:str, isdir:bool, mtime:datetime, size:int
		self.__root.getChildren().sort()

	def	List(self):
		return self.__root.getChildren()
