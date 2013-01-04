from PyQt4 import QtCore, QtGui
import architem
import os, pprint

def	listdir(path):
	'''
	'''
	dirset = set()
	fileset = set()
	cutlen = len(os.path.dirname(path))+1
	for root, dirs, files in os.walk(path):
		dir = root[cutlen:]
		dirset.add(dir)
		for f in files:
			fileset.add(os.path.join(dir, f))
			
	return dirset, fileset
#	print "Dirs:"
#	pprint.pprint(dirset)
#	print "Files:"
#	pprint.pprint(fileset)

class	ArchFile(QtCore.QObject):
	'''
	Archive file representation
	'''

	def	__init__(self):
		QtCore.QObject.__init__(self)
		self.__root = architem.ArchItemFolder("")
		self.__file = None
		self.__helper = None
		self.__dir_cache = None
		self.__file_cache = None

	# from model interface
	def	getRoot(self):
		return self.__root

	def	sort(self):
		self.__root.getChildren().sort()

	def	List(self):
		return self.__root.getChildren()

	# from main window interface
	def	load(self, hclass, path):
		self.__file = path
		self.__helper = hclass()
		self.__dir_cache = set()
		self.__file_cache = set()
		self.__root.getChildren().clear()
		errcode, result = self.__helper.list(str(path))
		if (errcode):
			return
		for i in result:
			# name:str, isdir:bool, mtime:datetime, size:int
			self.__root.addChildRecursive(i[0].split("/"), i[1], QtCore.QDateTime(i[2]), i[3])
			if i[1]:
				self.__dir_cache.add(i[0])
			else:
				self.__file_cache.add(i[0])
		self.__root.getChildren().sort()

	def	add(self, paths):
		'''
		'''
		# 1. chk exist
		chk, newdirs, newfiles = self.__chk_add(paths)	# 0 - add, 1 - u?, 2 - a+x, 3 - return
		if chk == 3:
			print "3"
			return
		# 2. add
		errcode, msg = self.__helper.add(self.__file, map(str, paths), chk == 2)
		if errcode:
			print "error adding"
			print msg
			return
		# 3. update caches
		self.__dir_cache.update(newdirs)
		self.__file_cache.update(newfiles)
		# 4. update gfx
		errcode, result = self.__helper.list(self.__file, list(newdirs|newfiles))
		if (errcode):
			return
		for i in result:
			self.__root.addChildRecursive(i[0].split("/"), i[1], QtCore.QDateTime(i[2]), i[3])
		self.__root.getChildren().sort()

	def	extract(self, fileNames, dest):
		'''
		fileNames: list(ArchItem*)
		dest: str
		'''
		src = list()
		for i in fileNames:
			src.append(str(i.getFullPath()))
		return self.__helper.extract(str(self.__file), src, str(dest))

	def	delete(self, fileNames):
		'''
		fileNames: [ArchItem*,]
		'''
		src = list()
		for i in fileNames:
			src.append(str(i.getFullPath()))
		err, msg = self.__helper.delete(self.__file, src)
		if (not err):
			for i in src:
				self.__root.delChildRecursively(i.split('/'))
		return err, msg

	# utility
	def	__list(self, result):
		for i in result:
			# name:str, isdir:bool, mtime:datetime, size:int
			self.__root.addChildRecursive(i[0].split("/"), i[1], QtCore.QDateTime(i[2]), i[3])
			if i[1]:
				self.__dir_cache.add(i[0])
			else:
				self.__file_cache.add(i[0])
		self.__root.getChildren().sort()
#		print "Dir:"
#		pprint.pprint(self.__dir_cache)
#		print "File:"
#		pprint.pprint(self.__file_cache)

	def	__chk_add(self, sources):
		'''
		@return:
		  - 0 - ok (nothing to do) => simply add
		  - 1 - exist => over (update?)
		  - 2 - exist => skip
		  - 3 - cancel
		'''
		dirset = set()
		fileset = set()
		# 1. load sources
		for src in sources:
			d, f = listdir(str(src))
			dirset.update(d)
			fileset.update(f)
		# 2. chk type conflict
		# 2.1. dirs as files
		d2f = dirset & self.__file_cache
		# 2.2. files as dirs
		f2d = fileset & self.__dir_cache
		# ask
		if d2f or f2d:
			msg = QtGui.QMessageBox(QtGui.QMessageBox.Critical, self.tr("Type conflict"), self.tr("Type conflict detected"))
			msg.setInformativeText(self.tr("Some folders and files to add exist in archive and have another type"))
			detailed = ""
			if (d2f):
				detailed += self.tr("Folders to add are files in archive:\n")
				for i in d2f:
					detailed += (i + "\n")
			if (f2d):
				detailed += self.tr("Files to add are folder in archive:\n")
				for i in f2d:
					detailed += (i + "\n")
			msg.setDetailedText(detailed)
			msg.exec_()
			return 3
		newdirs = dirset - self.__dir_cache
		newfiles = fileset - self.__file_cache
		# 3. chk interception
		# 3.1. dirs exists
		dirsexist = dirset & self.__dir_cache
		# 3.2. files exists
		filesexist = fileset & self.__file_cache
		if dirsexist or filesexist:
			msg = QtGui.QMessageBox(QtGui.QMessageBox.Question, self.tr("John, I need help"), self.tr("Some folders and/or files to add exist in archive"), QtGui.QMessageBox.Yes|QtGui.QMessageBox.No|QtGui.QMessageBox.Cancel)
			msg.setInformativeText(self.tr("Do you want to replace them?"))
			detailed = ""
			if (dirsexist):
				detailed += self.tr("Folders already exist in archive:\n")
				for i in dirsexist:
					detailed += (i + "\n")
			if (filesexist):
				detailed += self.tr("Files already exist in archive:\n")
				for i in filesexist:
					detailed += (i + "\n")
			msg.setDetailedText(detailed)
			responce = msg.exec_()
			return {QtGui.QMessageBox.Yes:1, QtGui.QMessageBox.No:2, QtGui.QMessageBox.Cancel:3}[responce], newdirs, newfiles
		return 0, newdirs, newfiles
