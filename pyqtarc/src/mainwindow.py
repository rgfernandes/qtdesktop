# filter: 
from PyQt4 import QtCore, QtGui
from ui.Ui_main import Ui_Main

from archfile	import ArchFile
from architemmodel import ArchItemModel

class	MainWindow(QtGui.QMainWindow, Ui_Main):
	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		#self.modelMain = None
		self.setupUi(self)
		self.__setSlots()
		self.__archfile = ArchFile()
		self.__addressStack = list()
		self.treeView.setModel(ArchItemModel(self.__archfile))

	def	__setSlots(self):
		self.connect(self.treeView,		QtCore.SIGNAL( "activated(const QModelIndex &)" ), self.__onActionActivated )
		self.connect(self.action_Up,		QtCore.SIGNAL( "triggered()" ), self.__onActionUp )
		self.connect(self.action_FileOpen,	QtCore.SIGNAL( "triggered()" ), self.__onActionFileOpen )
		self.connect(self.action_AddFile,	QtCore.SIGNAL( "triggered()" ), self.__onActionAddFile )
		self.connect(self.action_AddDirectory,	QtCore.SIGNAL( "triggered()" ), self.__onActionAddFolder )
		self.connect(self.action_Extract,	QtCore.SIGNAL( "triggered()" ), self.__onActionExtract )
		self.connect(self.action_Delete,	QtCore.SIGNAL( "triggered()" ), self.__onActionDelete )

	def	__onActionActivated(self, index):
		item = index.internalPointer()	# ArchItem*
		if item.isDir():
			self.__addressStack.append(index.parent())
			self.treeView.setRootIndex(index)
			self.address.setText(item.getFullPath())

	def	__onActionUp(self):
		if len(self.__addressStack):
			index = self.__addressStack.pop()
			self.treeView.setRootIndex(index)
			item = index.internalPointer()
			self.address.setText(item.getFullPath() if item else "")

	def	__onActionFileOpen(self):
		'''
		filter " (*.7z *.arj *.rar *.zip *.tar *.tar.gz *.tgz *.tar.bz *.tar.bz2 *.tbz *.tbz2  *.tbz *.tar.7z *.tar.lzma *.tlzma *.tar.xz *.txz)")
		'''
		#fileName = QtCore.QString("test.7z")
		fileName = QtGui.QFileDialog.getOpenFileName(
			caption=self.tr("Open file"),
			filter = self.tr("Archive") + " (*.zip)")
		if (not fileName.isEmpty()):
			self.__archfile.load(fileName)
			self.treeView.model().refresh()

	def	__onActionAddFile(self):
		fileNames = QtGui.QFileDialog.getOpenFileNames(
			caption=self.tr("Add file"),
			filter = self.tr("Any file") + " (*.*)")
		if (not fileNames.isEmpty()):
			self.__archfile.add(fileNames)
			self.treeView.model().refresh()

	def	__onActionAddFolder(self):
		folderName = QtGui.QFileDialog.getExistingDirectory(
			caption=self.tr("Add folder"),)
		if (not folderName.isEmpty()):
			self.__archfile.add([folderName,])
			self.treeView.model().refresh()
		#if (dialog.exec_()):
		#	fileNames = dialog.selectedFiles()

	def	__getSelected(self):
		'''
		@return: [ArchItem*,]
		'''
		fileNames = list()
		selected = self.treeView.selectedIndexes()
		for i in selected:
			if (i.column() == 0):	# exclude 1+ columns
				fileNames.append(i.internalPointer())
		return fileNames

	def	__onActionExtract(self):
		'''
		If 1 - select file
		else: select folder
		'''
		fileNames = self.__getSelected()
		if (fileNames):
			dest = QtGui.QFileDialog.getExistingDirectory(
				caption=self.tr("Extract to folder"),)
			if (not dest.isEmpty()):
				self.__archfile.extract(fileNames, dest)

	def	__onActionDelete(self):
		fileNames = self.__getSelected()
		if (fileNames):
			err, msg = self.__archfile.delete(fileNames)
			self.treeView.model().reset()
