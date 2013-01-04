# filter: 
from PyQt4 import QtCore, QtGui
from ui.Ui_main import Ui_Main

from archfile	import ArchFile
from architemmodel import ArchItemModel

import pkgutil
import magic

#def test1(pkg):
#	return [name for _, name, _ in pkgutil.iter_modules([pkg])]

class	MainWindow(QtGui.QMainWindow, Ui_Main):
	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		#self.modelMain = None
		self.setupUi(self)
		self.__setSlots()
		self.__archfile = ArchFile()	# archive tree
		self.__addressStack = list()	# current root stack
		self.__exts = ""
		self.__mime2helper = dict()
		self.__magic = magic.open(magic.MIME)
		self.__magic.load()
		self.__init_helpers()
		self.treeView.setModel(ArchItemModel(self.__archfile))

	def	__init_helpers(self):
		#exec "from helper import %s" % ','.join(test1('helper'))
		from helper import lzma, zip
		a = locals()
		for i in a:
			if (i != 'self'):
				self.__exts += (" *." + " ".join(a[i].exts))
				for j in a[i].mimes:
					self.__mime2helper[j] = a[i].mainclass
				#Sprint a[i].mainclass
		self.__exts = self.__exts.lstrip(' ')

	def	__setSlots(self):
		self.connect(self.treeView,		QtCore.SIGNAL( "activated(const QModelIndex &)" ), self.__onActionActivated )
		self.connect(self.treeView,		QtCore.SIGNAL( "selectionChanged(const QItemSelection &, const QItemSelection &)" ), self.__onActionSelected )
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

	def	__onActionSelected(self, selected, deselected):
		print "ok"
		indexes = selected.indexes()
		if indexes.count() != 1:
			self.statusBar().clearMessage()
		else:
			item = indexes[0].internalPointer()	# ArchItem*
			if not item.isDir():
				self.statusBar().showMessage(item.getFullPath())

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
		fileName = QtCore.QString("test.7z")
		#fileName = QtGui.QFileDialog.getOpenFileName(caption=self.tr("Open file"), filter = self.tr("Archive") + " (%s)" % self.__exts)
		if (not fileName.isEmpty()):
			absFileName = QtCore.QFileInfo(fileName).canonicalFilePath()
			#print str(fileName), str(absFileName)
			mime = self.__magic.file(str(absFileName)).split(';')[0]
			self.__archfile.load(self.__mime2helper[mime], absFileName)
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
			dest = QtGui.QFileDialog.getExistingDirectory(caption=self.tr("Extract to folder"),)
			#dest = QtCore.QString("/home/eugene/Version/SVN/qtdesktop/trunk/pyqtarc/tmp")
			if (not dest.isEmpty()):
				self.__archfile.extract(fileNames, dest)

	def	__onActionDelete(self):
		fileNames = self.__getSelected()
		if (fileNames):
			err, msg = self.__archfile.delete(fileNames)
			self.treeView.model().reset()
