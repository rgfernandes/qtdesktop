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

class	MyTableModel(QtSql.QSqlTableModel):
	def	__init__(self, parent = None, db = None):
		super(MyTableModel, self).__init__(parent, db)
		self.__iconProvider = QtGui.QFileIconProvider()

	def	data(self, index, role):
		if (not index.isValid()):
			return QtCore.QVariant()
		if (role == QtCore.Qt.DecorationRole):	#index.column() == 2
			return self.__iconProvider.icon(QtGui.QFileIconProvider.Folder) if index.sibling(index.row(), 3).data().toBool() else self.__iconProvider.icon(QtGui.QFileIconProvider.File)
		return super(MyTableModel, self).data(index, role)

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
		self.__model = MyTableModel(self, db)
		self.__model.setTable("arch")
		self.__model.setEditStrategy(QtSql.QSqlTableModel.OnManualSubmit)
		#self.__model.setSort(3, QtCore.Qt.DescendingOrder)	# dir
		#self.__model.setSort(2, QtCore.Qt.AscendingOrder)	# name
		self.__model.setFilter("1=1 ORDER BY isdir DESC, name ASC")	# nice hack
		self.__model.select()
		'''
		self.__model = MyQueryModel()
		self.__model.setQuery("SELECT id, parent_id, nane, isdir FROM arch ORDER BY isdir, name", db)
		'''
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
		def	add_record(id, parent, v, q):
			#print v[2].toTime_t()
			q.bindValue(":id", i)
			q.bindValue(":parent_id", parent)
			q.bindValue(":name", v[0])
			q.bindValue(":isdir", v[1])
			q.bindValue(":datetime", v[2].toTime_t() if v[2] else None)
			q.bindValue(":nsize", v[3])
			q.bindValue(":csize", v[4])
			q.exec_()
		absFileName = QtCore.QFileInfo(fileName).canonicalFilePath()
		mime = self.__magic.file(str(absFileName)).split(';')[0]	# FIXME
		errcode, result = self.__mime2helper[mime]().list(absFileName)
		if (errcode):
			return
		folderdict = dict()	# path: id for folders
		QtSql.QSqlQuery("DELETE FROM arch").exec_()
		q = QtSql.QSqlQuery()
		q.prepare("INSERT INTO arch (id, parent_id, name, isdir, datetime, nsize, csize) VALUES (:id, :parent_id, :name, :isdir, :datetime, :nsize, :csize)")
		i = 1
		for v in result:	# name:QString, isdir:bool, datetime:QDateTime, nsize:ULong, csize:ULong
			pathlist = v[0].split("/")	# FIXME: separator()
			name = pathlist.takeLast()
			parent_id = None
			path = QtCore.QString()
			sep = ""
			for n in pathlist:
				path = path + sep + n
				sep = "/"
				id = folderdict.get(path, None)
				if (id):
					parent_id = id
					continue
				add_record(i, parent_id, (n, True, None, None, None), q)
				parent_id = i
				i += 1
			add_record(i, parent_id, (name, v[1], v[2], v[3], v[4]), q)
			i += 1
		#self.__model.submitAll()
