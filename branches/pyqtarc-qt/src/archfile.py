from PyQt4 import QtCore, QtGui
import architem
import os, pprint

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
			print i[0].toLocal8Bit()
			# name:str, isdir:bool, mtime:datetime, size:int
			self.__root.addChildRecursive(i[0].split("/"), i[1], i[2], i[3])
			if i[1]:
				self.__dir_cache.add(i[0])
			else:
				self.__file_cache.add(i[0])
				# patch
				dir = QtCore.QFileInfo(i[0]).dir().dirName()
				if dir and (not dir in self.__dir_cache):
					self.__dir_cache.add(dir)
		self.__root.getChildren().sort()
		#pprint.pprint(self.__dir_cache)
		#pprint.pprint(self.__file_cache)

	def	add(self, paths):
		'''
		'''
		# 1. chk exist
		chk, newdirs, newfiles = self.__chk_add(paths)	# 0 - add, 1 - u?, 2 - a+x, 3 - return
		if chk == 3:
			#print "3"
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
		chk, newdirs, newfiles = self.__chk_extract(fileNames, str(dest))
		if chk == 3:
			return
		src = list()
		for i in fileNames:
			src.append(str(i.getFullPath()))
		return self.__helper.extract(str(self.__file), src, str(dest), chk == 2)

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
	def	__chk_add(self, fsitems):
		'''
		TODO: arcdir add to
		@param: fsitems:list(str) - Folders/files in FS to add
		@param: direction:bool - True = add, False = extract
		@return:
		  - 0 - ok (nothing to do) => simply add/extract
		  - 1 - exist => replace/update
		  - 2 - exist => skip
		  - 3 - cancel
		'''
		direction = True
		arcdirs = self.__dir_cache
		arcfiles = self.__file_cache
		fsdirs = set()
		fsfiles = set()
		#print "1. load sources", fsitems
		for src in fsitems:
			d, f = self.__chk_listdir(str(src), True)
			fsdirs.update(d)
			fsfiles.update(f)
		return self.__chk_chk(arcdirs, arcfiles, fsdirs, fsfiles, True)

	def	__chk_extract(self, arcitems, fsfolder):
		'''
		@param arcitems:[ArcItem*] - items to extract
		@param fsfolder:str - destination
		WARNING: arcitems must be in same folder
		Return partial recursive dir/file list for given item - with cutted head.
		E.g. all subentries of dir1/dir11 start from dir11
		'''
		# 1. prepare extractable
		arcdirs = set()
		arcfiles = set()
		cutlen = len(os.path.dirname(str(arcitems[0].getFullPath())))
		if cutlen:
			cutlen += 1
		for i in arcitems:
			if i.isDir():
				arcdirs.add(i.getName())
				tosearch = i.getFullPath()+'/'
				for j in self.__dir_cache:
					if j.startswith(tosearch):
						arcdirs.add(j[cutlen:])
				for j in self.__file_cache:
					if j.startswith(tosearch):
						arcfiles.add(j[cutlen:])
			else:
				arcfiles.add(i.getName())
		# 2. prepare dest
		fsdirs, fsfiles = self.__chk_listdir(fsfolder, False)
		return self.__chk_chk(arcdirs, arcfiles, fsdirs, fsfiles, False)

	def	__chk_listdir(self, path, mode):
		fsdirs = set()
		fsfiles = set()
		cutlen = len(os.path.dirname(path))+1 if mode else len(path)+1
		#print "Lets go", path
		for root, dirs, files in os.walk(path):
			#print root
			dir = root[cutlen:]
			if (dir):
				fsdirs.add(dir)
			for f in files:
				fsfiles.add(os.path.join(dir, f))
		return fsdirs, fsfiles

	def	__chk_chk(self, arcdirs, arcfiles, fsdirs, fsfiles, direction):
		#print "arcdirs:", arcdirs
		#print "fsdirs:", fsdirs
		#print "arcfiles:", arcfiles
		#print "fsfiles:", fsfiles
		if (direction):	# extract
			strings = [
				self.tr("Some folders and/or files to add exist in archive and have another type"),
				self.tr("Folders to add are files in archive:\n"),
				self.tr("Files to add are folder in archive:\n"),
				self.tr("Some folders and/or files to add exist in archive"),
				self.tr("Folders already exist in archive:\n"),
				self.tr("Files already exist in archive:\n")
			]
		else:
			strings = [
				self.tr("Some folders and/or files to extract exist in file system and have another type"),
				self.tr("Files to extract are folders in file system:\n"),
				self.tr("Folders to extract are files in file system:\n"),
				self.tr("Some folders and/or files to extract exist in file system"),
				self.tr("Folders already exist in file system:\n"),
				self.tr("Files already exist in file system:\n")
			]
		#print "2. chk type conflict"
		# 2.1. dirs as files
		d2f = fsdirs & arcfiles
		# 2.2. files as dirs
		f2d = fsfiles & arcdirs
		#print "ask"
		if d2f or f2d:
			msg = QtGui.QMessageBox(
				QtGui.QMessageBox.Critical,
				self.tr("Type conflict"),
				self.tr("Type conflict detected")
			)
			msg.setInformativeText(strings[0])
			detailed = ""
			if (d2f):
				detailed += strings[1]
				for i in d2f:
					detailed += (i + "\n")
			if (f2d):
				detailed += strings[2]
				for i in f2d:
					detailed += (i + "\n")
			msg.setDetailedText(detailed)
			msg.exec_()
			return 3
		newdirs = fsdirs - arcdirs
		newfiles = fsfiles - arcfiles
		#print "3. chk interception"
		# 3.1. dirs exists
		dirsexist = fsdirs & arcdirs
		# 3.2. files exists
		filesexist = fsfiles & arcfiles
		if dirsexist or filesexist:
			msg = QtGui.QMessageBox(
				QtGui.QMessageBox.Question,
				self.tr("John, I need help"),
				strings[3],
				QtGui.QMessageBox.Yes|QtGui.QMessageBox.No|QtGui.QMessageBox.Cancel
			)
			msg.setInformativeText(self.tr("Do you want to replace them?"))
			detailed = ""
			if (dirsexist):
				detailed += strings[4]
				for i in dirsexist:
					detailed += (i + "\n")
			if (filesexist):
				detailed += strings[5]
				for i in filesexist:
					detailed += (i + "\n")
			msg.setDetailedText(detailed)
			responce = msg.exec_()
			return {QtGui.QMessageBox.Yes:1, QtGui.QMessageBox.No:2, QtGui.QMessageBox.Cancel:3}[responce], newdirs, newfiles
		return 0, newdirs, newfiles
