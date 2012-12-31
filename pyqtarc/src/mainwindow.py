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
		self.treeView.setModel(ArchItemModel(self.__archfile))

	def	__setSlots(self):
		self.connect(self.action_FileOpen,	QtCore.SIGNAL( "triggered()" ), self.__onActionFileOpen )
		self.connect(self.action_AddFile,	QtCore.SIGNAL( "triggered()" ), self.__onActionAddFile )
		#self.connect(self.action_AddDirectory,	QtCore.SIGNAL( "triggered()" ), self.__onActionAddFolder )

	def	__onActionFileOpen(self):
		'''
		filter " (*.7z *.arj *.rar *.zip *.tar *.tar.gz *.tgz *.tar.bz *.tar.bz2 *.tbz *.tbz2  *.tbz *.tar.7z *.tar.lzma *.tlzma *.tar.xz *.txz)")
		'''
		fileName = QtGui.QFileDialog.getOpenFileName(
			parent = None,
			caption=self.tr("Open file"),
			directory="",
			filter = self.tr("Archive") + " (*.7z)")
		if (not fileName.isEmpty()):
			self.__archfile.load(fileName)
			self.treeView.model().refresh()

	def	__onActionAddFile(self):
		fileNames = QtGui.QFileDialog.getOpenFileNames(
			parent = None,
			caption=self.tr("Add file"),
			directory="",
			filter = self.tr("Any file") + " (*.*)")
		if (not fileNames.isEmpty()):
			self.__archfile.add(fileNames)
			self.treeView.model().refresh()
