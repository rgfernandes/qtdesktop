'''
ArchItem* - representation of archive entries in model
TODO: ArchItemRoot
'''

from PyQt4 import QtCore

from architemset import ArchItemSet

class	ArchItem(object):
	def	__init__(self, name,  mtime, parent):
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
		#print self.__name, self.__parent
		if self.__parent:
			s = self.__parent.getFullPath()
			if s:
				return s + "/" + self.__name
		return self.__name

class	ArchItemFile(ArchItem):
	def	__init__(self, name, mtime = QtCore.QDateTime.currentDateTime(), parent = None, size = 0L):
		super(ArchItemFile, self).__init__(name, mtime, parent)
		self.__size = size

	@classmethod
	def	isDir(self):
		return False

	@classmethod
	def	isRoot(self):
		return False

	def	setSize(self, value):
		self.__size = value

	def	getSize(self):
		return self.__size

class	ArchItemFolder(ArchItem):
	def	__init__(self, name, mtime = QtCore.QDateTime.currentDateTime(), parent = None):
		super(ArchItemFolder, self).__init__(name, mtime, parent)
		self.__children = ArchItemSet()

	#def	__del__(self):
	#	self.__children.clear()

	@classmethod
	def	isDir(self):
		return True

	@classmethod
	def	isRoot(self):
		return False

	def	getChildren(self):
		return self.__children

	def	getChildrenCount(self):
		return self.__children.count()

	def	getChild(self, item):
		return self.__children.get(item)

	def	addChild(self, item):
		if (not self.__children.contains(item.getName())):
			self.__children.add(item)

	def	addChildRecursive(self, filePath, isDir, mtime, size):
		s = filePath[0]
		if (len(filePath) == 1):			# last itme in stack
			if self.__children.contains(s):		# update
				item = self.getChild(s)
				if item.isDir() != isDir:	# FIXME:
					pass
				else:
					item.setMtime(mtime)
					if (not isDir):
						item.setSize(size)
			else:
				self.addChild(ArchItemFolder(s, mtime, self) if isDir else ArchItemFile(s, mtime, self, size))
		else:						# dive in
			if self.__children.contains(s):		# update
				item = self.getChild(s)
				if item.isDir() != isDir:	# FIXME: convert file to item
					pass
			else:
				item = ArchItemFolder(s, mtime, self)	# FIXME: wrong mtime
				self.addChild(item)
			item.addChildRecursive(filePath[1:], isDir, mtime, size)

	def	delChild(self, item):
		return self.__children.del_item(item)

	def	delChildRecursively(self, itemlist):
		'''
		itemlist: [str,]
		@return: 0 on success
		'''
		item = self.getChild(itemlist[0])
		if (item):
			if (len(itemlist) > 1):
				if (item.isDir()):
					return item.delChildRecursively(itemlist[1:])
				return 1
			self.delChild(item)
			return 0
		return 1

class	ArchItemRoot(ArchItemFolder):

	@classmethod
	def	isRoot(self):
		return False
