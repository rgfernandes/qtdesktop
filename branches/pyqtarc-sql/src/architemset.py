from PyQt4 import QtCore

class	ArchItemSet:
	def	__init__(self):
		self.__dirs = dict()
		self.__files = dict()
		self.__list = QtCore.QStringList()	# list of names

	def	__resort(self, recurse = False):
		self.__list.sort()
		for i, name in enumerate(self.__list):
			item = self.get(name)
			item.setRow(i)
			if (recurse) and (item.isDir()):
				item.getChildren().sort()

	def	add(self, item):
		name = item.getName()
		if item.isDir():
			self.__dirs[name] = item
		else:
			self.__files[name] = item
		self.sort(False)

	def	get(self, item):
		'''
		@param item: int or str
		@return: item or none
		'''
		if (isinstance(item,  int)):
			item = self.__list[item]
		return self.__dirs.get(item,  self.__files.get(item,  None))

	def	contains(self, name):
		return (name in self.__dirs) or (name in self.__files)

	def	count(self):
		return len(self.__dirs) + len(self.__files)

	def	sort(self, recurse = True):
		self.__list.clear()
		for i in self.__dirs.values():
			self.__list.append(i.getName())
		self.__list.sort()
		tmplist = QtCore.QStringList()
		for i in self.__files.values():
			tmplist.append(i.getName())
		tmplist.sort()
		self.__list += tmplist
		for i, name in enumerate(self.__list):
			item = self.get(name)
			item.setRow(i)
			if recurse and (item.isDir()):
				item.getChildren().sort()

	def	del_item(self, item):
		name = item.getName()
		row = item.getRow()
		self.__list.removeAt(row)
		if item.isDir():
			del self.__dirs[name]
		else:
			del self.__files[name]
		for name in self.__list[row:]:
			item = self.get(name)
			item.setRow(item.getRow() - 1)

	def	clear(self):
		'''
		TODO: realy del items
		'''
		for item in self.__dirs.values():
			item.getChildren().clear()
		self.__dirs.clear()
		self.__files.clear()
		self.__list.clear()
