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
			self.__root.addChildRecursive(i[0], i[1], i[2],  i[3])
		self.__root.sort()

	def	List(self):
		return self.__root.getChildren()
