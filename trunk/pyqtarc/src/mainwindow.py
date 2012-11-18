from PyQt4 import QtCore, QtGui
from ui.Ui_main import Ui_Main

from archfile	import ArchFile

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

	def	__onActionFileOpen(self):
		fileName = QtGui.QFileDialog.getOpenFileName(
			parent = None,
			caption=self.tr("Open file"),
			directory="",
			# " (*.7z *.arj *.rar *.zip *.tar *.tar.gz *.tgz *.tar.bz *.tar.bz2 *.tbz *.tbz2  *.tbz *.tar.7z *.tar.lzma *.tlzma *.tar.xz *.txz)")
			filter = self.tr("Archive") + " (*.7z)")
		if (not fileName.isEmpty()):
			self.__archfile.load(fileName)
			self.treeView.model().refresh()
