import architem

class	ArchFile:
	'''
	Archive file representation
	'''

	def	__init__(self):
		self.__root = architem.ArchItemFolder("")

	def	sort(self):
		self.__root.getChildren().sort()

	def	load(self,  path):
		pass

	def	List(self):
		return self.__root.getChildren()
