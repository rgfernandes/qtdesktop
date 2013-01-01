class	ArchItemSet:
	def	__init__(self):
		self.__dirs = dict()
		self.__files = dict()
		self.__list = list()

	def	add(self, item):
		name = item.getName()
		if item.isDir():
			self.__dirs[name] = item
		else:
			self.__files[name] = item

	def	get(self, item):
		'''
		@param item: int or str
		@return: item or none
		'''
		if (isinstance(item,  int)):
			return self.__list[item] if ((item >= 0) and (item < len(self.__list))) else None
		else:
			return self.__dirs.get(item,  self.__files.get(item,  None))

	def	contains(self, name):
		return (name in self.__dirs) or (name in self.__files)

	def	count(self):
		return len(self.__dirs) + len(self.__files)

	def	sort(self):
		self.__list = self.__dirs.values() + self.__files.values()
		for i, item in enumerate(self.__list):
			item.setRow(i)
			if (item.isDir()):
				item.getChildren().sort()

	def	del_item(self, item):
		name = item.getName()
		row = item.getRow()
		del self.__list[row]
		if item.isDir():
			del self.__dirs[name]
		else:
			del self.__files[name]
		for i in self.__list[row:]:
			i.setRow(i.getRow() - 1)

	def	clear(self):
		'''
		TODO: realy del items
		'''
		for item in self.__dirs.values():
			item.getChildren().clear()
		self.__dirs.clear()
		self.__files.clear()
		self.__list.clear()
