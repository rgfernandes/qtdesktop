# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui, QtSql
from PyQt4.QtCore import qDebug
from ui.Ui_main import Ui_Main

from utility		import *
from archtablemodel	import ArchTableModel
from architemview	import ArchItemView

import pkgutil
import magic

class	MainWindow(QtGui.QMainWindow, Ui_Main):
	def	__init__(self):
		QtGui.QMainWindow.__init__(self)
		self.setupUi(self)
		self.__addressStack = list()	# current folder stack (ids)
		self.__exts = ""
		self.__mime2helper = dict()
		self.__magic = magic.open(magic.MIME)
		self.__magic.load()
		self.__init_helpers()
		self.__file = None
		self.__helper = None
		self.treeView = ArchItemView(self.centralwidget)
		self.verticalLayout.addWidget(self.treeView)
		self.__setSlots()

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

	def	__init_db(self, db):
		q = QtSql.QSqlQuery()
		if not q.exec_("DELETE FROM arch"):
			db_err(q)

	def	setModel(self, db):
		self.__init_db(db)
		self.__model = ArchTableModel(self, db)
		self.__model.setTable("arch")
		self.__model.setEditStrategy(QtSql.QSqlTableModel.OnManualSubmit)
		self.__setFilter()
		self.treeView.setModel(self.__model)
		self.treeView.setModelColumn(2)

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

	def	__setFilter(self):
		'''
		Set current file filter by addressStack
		'''
		if len(self.__addressStack):
			self.__model.setFilter("parent_id = %d ORDER BY isdir DESC, name ASC" % self.__addressStack[len(self.__addressStack)-1])
		else:
			self.__model.setFilter("parent_id IS NULL ORDER BY isdir DESC, name ASC")	# nice hack
		self.__model.select()
		#self.address.setText(item.getFullPath() if item else "")

	def	__onActionActivated(self, index):
		'''
		Dive into [sub]folder
		'''
		isdir = index.sibling(index.row(), 3).data().toBool()
		if isdir:
			#print index.sibling(index.row(), 0).data().toUInt()
			self.__addressStack.append(self.__model.get_id(index))	# FIXME: check success convert
			self.__setFilter()

	def	__onActionUp(self):
		'''
		Dive out from [sub]folder
		'''
		if len(self.__addressStack):
			self.__addressStack.pop()
			self.__setFilter()

	def	__onActionSelected(self, selected, deselected):
		'''
		Highlighting entry => handle statusbar
		'''
		#print "ok"
		indexes = selected.indexes()
		if indexes.count() != 1:
			self.statusBar().clearMessage()
		else:
			item = indexes[0].internalPointer()	# ArchItem*
			if not item.isDir():
				self.statusBar().showMessage(item.getFullPath())

	def	__onActionFileOpen(self):
		'''
		Open (== load) archive
		filter " (*.7z *.arj *.rar *.zip *.tar *.tar.gz *.tgz *.tar.bz *.tar.bz2 *.tbz *.tbz2  *.tbz *.tar.7z *.tar.lzma *.tlzma *.tar.xz *.txz)")
		'''
		fileName = QtGui.QFileDialog.getOpenFileName(caption=self.tr("Open file"), filter = self.tr("Archive") + " (%s)" % self.__exts)
		if (not fileName.isEmpty()):
			self._file_open(fileName)

	def	__onActionAddFile(self):
		'''
		Add files to archive
		'''
		fileNames = QtGui.QFileDialog.getOpenFileNames(
			caption=self.tr("Add file"),
			filter = self.tr("Any file") + " (*.*)")
		if (not fileNames.isEmpty()):
			self.__add_entries(fileNames)

	def	__onActionAddFolder(self):
		'''
		Add a folder to archive
		'''
		folderName = QtGui.QFileDialog.getExistingDirectory(
			caption=self.tr("Add folder"),)
		if (not folderName.isEmpty()):
			self.__add_entries([folderName,])	# FIXME: to QStringList

	def	__onActionExtract(self):
		'''
		Extract entries from archive
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
		'''
		Delete entries in archive.
		'''
		pathlist = QtCore.QStringList()
		idlist = list()
		for index in self.treeView.selectedIndexes():
			#print index.column()
			if (index.column() == 7):	# exclude 1+ columns
				pathlist.append(index.data().toString())
				idlist.append(self.__model.get_id(index))
		if len(idlist):
			#print "Something to delete"
			errcode, out, err = self.__helper.delete(self.__file, pathlist)
			if (errcode or err):
				print "Err:", out
			else:
				#print "DELETE FROM arch WHERE id IN (%s)" % ",".join(map(str, idlist))
				QtSql.QSqlQuery("DELETE FROM main.arch WHERE id IN (%s)" % ",".join(map(str, idlist))).exec_()
				self.__model.select()
				#self.treeView.model().reset()

	def	__getSelected(self):
		'''
		Called from __onActionExtract
		@return: [ArchItem*,]
		'''
		ids = list()
		selected = self.treeView.selectedIndexes()
		for i in selected:
			#if (i.column() == 0):	# exclude 1+ columns
			ids.append(i.internalPointer())
		return ids

	def	_file_open(self, fileName):
		self.__file = QtCore.QFileInfo(fileName).canonicalFilePath()
		mime = self.__magic.file(str(self.__file)).split(';')[0]	# FIXME
		self.__helper = self.__mime2helper[mime]()
		errcode, result = self.__helper.list(self.__file)
		if (errcode):
			return
		load_arch(result)
		#self.__model.setFilter("parent_id=NULL")
		self.__model.select()

	def	__add_entries(self, entrynames):
		# 1. list src
		if load_fs(entrynames):
			# 2. check folders<>files
			# 3. check on exists
			q = QtSql.QSqlQuery("SELECT fs.fullpath FROM fs JOIN arch ON fs.fullpath = arch.fullpath")
			if (q.exec_()):
				pass
			# 4. add to archive
			# select fullpath from fs where fullpath not in (select fullpath from arch) and (endpoint)
			return False
			#self.treeView.model().refresh()	# FIXME: reload
			#self.treeView.model().reset()	# FIXME: reload
