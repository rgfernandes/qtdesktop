# filter: 
from PyQt4 import QtCore, QtGui, QtSql
from ui.Ui_main import Ui_Main

from archfile	import ArchFile
from architemmodel import ArchItemModel
from architemview import ArchItemView

import pkgutil
import magic

#def test1(pkg):
#	return [name for _, name, _ in pkgutil.iter_modules([pkg])]

class	MySqlTableModel(QtSql.QSqlTableModel):
	def	__init__(self, parent = None, db = None):
		super(MySqlTableModel, self).__init__(parent, db)
		self.__iconProvider = QtGui.QFileIconProvider()

	def	data(self, index, role):
		if (not index.isValid()):
			return QtCore.QVariant()
		if (role == QtCore.Qt.DecorationRole):	#index.column() == 2
			return self.__iconProvider.icon(QtGui.QFileIconProvider.Folder) if index.sibling(index.row(), 3).data().toBool() else self.__iconProvider.icon(QtGui.QFileIconProvider.File)
		return super(MySqlTableModel, self).data(index, role)

class	MainWindow(QtGui.QMainWindow, Ui_Main):
	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		#self.modelMain = None
		self.setupUi(self)
		self.__archfile = ArchFile()	# archive tree
		self.__addressStack = list()	# current root stack
		self.__exts = ""
		self.__mime2helper = dict()
		self.__magic = magic.open(magic.MIME)
		self.__magic.load()
		self.__init_helpers()
		self.treeView = ArchItemView(self.__archfile, self.centralwidget)
		self.verticalLayout.addWidget(self.treeView)
		self.__setSlots()
		#self.__model = ArchItemModel(self.__archfile)
		#self.__proxyModel = QtGui.QSortFilterProxyModel(self)
		#self.__proxyModel.setSourceModel(self.__model)
		#self.treeView.setModel(self.__proxyModel)
		#self.treeView.setModel(self.__model)
		#self.treeView.setSortingEnabled(True)

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

	def	setModel(self, db):
		self.__model = MySqlTableModel(self, db)
		self.__model.setTable("arch")
		self.__model.setEditStrategy(QtSql.QSqlTableModel.OnManualSubmit)
		self.__model.setSort(1, QtCore.Qt.AscendingOrder)
		self.__model.select()
		self.treeView.setModel(self.__model)
		self.treeView.setModelColumn(2)
		#self.treeView.setModel(ArchItemModel(self.__archfile))

	def	__setSlots(self):
		self.connect(self.treeView,		QtCore.SIGNAL( "activated(const QModelIndex &)" ), self.__onActionActivated )
		self.connect(self.treeView,		QtCore.SIGNAL( "selectionChanged(const QItemSelection &, const QItemSelection &)" ), self.__onActionSelected )
		self.connect(self.action_Up,		QtCore.SIGNAL( "triggered()" ), self.__onActionUp )
		self.connect(self.action_FileOpen,	QtCore.SIGNAL( "triggered()" ), self.__onActionFileOpen )
		self.connect(self.action_AddFile,	QtCore.SIGNAL( "triggered()" ), self.__onActionAddFile )
		self.connect(self.action_AddDirectory,	QtCore.SIGNAL( "triggered()" ), self.__onActionAddFolder )
		self.connect(self.action_Extract,	QtCore.SIGNAL( "triggered()" ), self.__onActionExtract )
		self.connect(self.action_Delete,	QtCore.SIGNAL( "triggered()" ), self.__onActionDelete )
		self.connect(self.action_HelpAboutQt,	QtCore.SIGNAL( "triggered()" ), QtGui.QApplication.aboutQt )

	def	__onActionActivated(self, index):
		item = index.internalPointer()	# ArchItem*
		if item.isDir():
			self.__addressStack.append(index.parent())
			self.treeView.setRootIndex(index)
			self.address.setText(item.getFullPath())

	def	__onActionSelected(self, selected, deselected):
		#print "ok"
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
		#fileName = QtCore.QString("test.7z")
		fileName = QtGui.QFileDialog.getOpenFileName(caption=self.tr("Open file"), filter = self.tr("Archive") + " (%s)" % self.__exts)
		if (not fileName.isEmpty()):
			self._file_open(fileName)

	def	__onActionAddFile(self):
		fileNames = QtGui.QFileDialog.getOpenFileNames(
			caption=self.tr("Add file"),
			filter = self.tr("Any file") + " (*.*)")
		if (not fileNames.isEmpty()):
			self.__archfile.add(fileNames)
			self.treeView.model().reset()

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

	def	_file_open(self, fileName):
		#def	add_record(row, v, q):
		#	'''by query'''
		def	add_record(row, v):
			'''by model'''
			self.__model.insertRow(row)
			self.__model.setData(self.__model.index(row, 0), QtCore.QVariant(row+1)) 
			self.__model.setData(self.__model.index(row, 1), QtCore.QVariant(None)) 
			self.__model.setData(self.__model.index(row, 2), QtCore.QVariant(v[0])) 
			self.__model.setData(self.__model.index(row, 3), QtCore.QVariant(v[1])) 
			self.__model.submitAll()
		def	add_record0(v):
			''''by record'''
			rec = self.__model.record()
			rec.setValue("name", v[0])
			rec.setValue("parent_id", None)
			rec.setValue("isdir", v[1])
			print "Insert:", self.__model.insertRecord(-1, rec)
		absFileName = QtCore.QFileInfo(fileName).canonicalFilePath()
		mime = self.__magic.file(str(absFileName)).split(';')[0]	# FIXME
		#self.__archfile.load(self.__mime2helper[mime], absFileName)
		##self.treeView.reset()
		#self.treeView.model().refresh()
		#self.__root.getChildren().clear()
		# clear arch table
		errcode, result = self.__mime2helper[mime]().list(absFileName)
		if (errcode):
			return
		#self.__model.clear()
		#self.__model.submitAll()
		#rows = self.__model.rowCount()
		QtSql.QSqlQuery("DELETE FROM arch").exec_()
		q = QtSql.QSqlQuery()
		q.prepare("INSERT INTO arch (id, parent_id, name, isdir) VALUES (:id, :parent_id, :name, :isdir)")
		for i, v in enumerate(result):	# name:str, isdir:bool, mtime:datetime, size:int
			#add_record(i+rows, v)
			q.bindValue(":id", i+1)
			q.bindValue(":parent_id", None)
			q.bindValue(":name", v[0])
			q.bindValue(":isdir", v[1])
			q.exec_()
		self.__model.submitAll()
		#QtSql.QSqlDatabase.database().commit()
