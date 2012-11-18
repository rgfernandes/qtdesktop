'''
ArchItem* - representation of archive entries in model
'''

from architemset import ArchItemSet

class	ArchItem:
	def __init__(self, name,  mtime, parent):
		self.__name = name
		self.__mtime = mtime
		self.__parent = parent
		self.__row = 0

	def	setName(self, value):
		self.__name = value

	def	getName(self):
		return self.__name

	def	setMtime(self, value):
		self.__mtime = value

	def	getMtime(self):
		return self.__mtime

	def	setParent(self, value):
		self.__parent = value

	def	getParent(self):
		return self.__parent

	def	setRow(self, value):
		self.__row = value

	def	getRow(self):
		return self.__row

	def	getFullPath(self):
		if self.__parent:
			s = self.__parent.getFullPath()
			if s:
				return s + "/" + self.__name
		else:
			return self.__name

class	ArchItemFile(ArchItem):
	def __init__(self, name,  mtime, parent, size):
		super(ArchItemFile, self).__init__(name,  mtime, parent)
		self.__size = size

	@classmethod
	def	isDir():
		return False

class	ArchItemFolder(ArchItem):
	def __init__(self, name,  mtime, parent):
		super(ArchItemFile, self).__init__(name,  mtime, parent)
		self.__children = ArchItemSet()

	@classmethod
	def	isDir():
		return True
