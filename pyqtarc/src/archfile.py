import architem
from helper.lzma import ArchHelper7z

class	ArchFile:
	'''
	Archive file representation
	'''

	def	__init__(self):
		self.__root = architem.ArchItemFolder("")

	def	sort(self):
		self.__root.getChildren().sort()

	def	load(self, path):
		helper = ArchHelper7z()
		helper.set_apth(path)
		helper.list(self.__root)

	def	List(self):
		return self.__root.getChildren()
