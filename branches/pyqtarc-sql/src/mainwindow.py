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
		self.connect(self.action_AddFile,	QtCore.SIGNAL( "triggered()" ), self.__onActionAddFiles )
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

	def	__onActionAddFiles(self):
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
		entries = self.__getSelectedIDs()
		if (entries):
			dest = QtGui.QFileDialog.getExistingDirectory(caption=self.tr("Extract to folder"),)
			#dest = QtCore.QString("/home/eugene/Version/SVN/qtdesktop/trunk/pyqtarc/tmp")
			if (not dest.isEmpty()):
				# get current path
				if len(self.__addressStack):
					q = QtSql.QSqlQuery("SELECT fullpath FROM arch WHERE id=%d" % self.__addressStack[-1:][0])
					if q.next():
						srcdir = q.value(0).toString()
				else:
					srcdir = QtCore.QString()
				self.__extract_entries(srcdir, dest, entries)

	def	__onActionDelete(self):
		'''
		Delete entries in archive.
		'''
		pathlist = QtCore.QStringList()
		idlist = list()
		for index in self.treeView.selectedIndexes():
			#print index.column()
			if (index.column() == 7):
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

	def	__getSelectedIDs(self):
		'''
		'''
		retvalue = dict()
		for index in self.treeView.selectedIndexes():
			id = self.__model.get_id(index)
			retvalue[self.__model.get_id(index)] = True
		return retvalue.keys()

	def	__getSelectedPaths(self):
		'''
		Called from __onActionExtract
		@return: [ArchItem*,]
		'''
		retvalue = list()
		for index in self.treeView.selectedIndexes():
			if (index.column() == 7):	# 1, 2, 3, 5, 7
				retvalue.append(index.data().toString())
		return retvalue

	def	__reload(self):
		errcode, result = self.__helper.list(self.__file)
		if (errcode):
			return
		load_arch(result)
		#self.__model.setFilter("parent_id=NULL")
		self.__model.select()

	def	_file_open(self, fileName):
		self.__file = QtCore.QFileInfo(fileName).canonicalFilePath()
		mime = self.__magic.file(str(self.__file)).split(';')[0]	# FIXME
		self.__helper = self.__mime2helper[mime]()
		self.__reload()

	def	__add_entries(self, entrynames):
		'''
		@param entrynames:QStringList - absolute paths of entries to add
		'''
		# 1. list src
		load_fs(entrynames)	# FIXME: false now
		# 2. check folders<>files
		# 3. check on exists
		q = QtSql.QSqlQuery("SELECT fullpath FROM fs WHERE alive=1")
		result = QtCore.QString()
		while (q.next()):
			result += (q.value(0).toString() + "\n")
		if result.isEmpty():
			action = 0
		else:
			msg = QtGui.QMessageBox(
				QtGui.QMessageBox.Question,
				self.tr("John, I need help"),
				self.tr("Some folders and/or files to add exist in archive"),
				QtGui.QMessageBox.Yes|QtGui.QMessageBox.No|QtGui.QMessageBox.Cancel
			)
			msg.setInformativeText(self.tr("Do you want to replace them?"))
			msg.setDetailedText(result)
			responce = msg.exec_()
			action = {QtGui.QMessageBox.Yes:1, QtGui.QMessageBox.No:2, QtGui.QMessageBox.Cancel:3}[responce]
		# 4. add to archive
		# here we have: 0 (simply add => add selected), 1 (replace => add selected), 2 (skip => add custom list (endpoints)), 3 (break => return)
		if (action == 3):
			return
		absprefix = QtCore.QFileInfo(entrynames[0]).absolutePath()
		relpaths = QtCore.QStringList()
		if (action < 2):
			for i in entrynames:
				relpaths << QtCore.QFileInfo(i).fileName()
		else:
			q = QtSql.QSqlQuery("SELECT fullpath FROM fs WHERE alive=0 and endpoint=1")
			while (q.next()):
				relpaths << q.value(0).toString()
		errcode, out, err = self.__helper.add(self.__file, absprefix, relpaths)
		if (errcode) or (err):
			print "error:", err
		else:
			self.__reload()	# FIXME:

	def	__extract_entries(self, src, dest, idlist):
		'''
		@param dest:QString - absolute path extract to
		@param entrynames:list[int,] - full paths of entries to extract
		'''
		# 0. prepare
		#QtSql.QSqlDatabase.database().transaction()
		QtSql.QSqlQuery("DELETE FROM fs").exec_()
		#basearchname = entrynames[0].
		# 1. prepare db
		# - insert selected into
		QtSql.QSqlQuery("INSERT INTO fs (fullpath, isdir, endpoint, alive) SELECT fullpath, isdir, endpoint, 0 FROM arch WHERE arch.id IN (%s)" % ",".join(map(str, idlist))).exec_()
		# - insert children
		QtSql.QSqlQuery("INSERT INTO fs (fullpath, isdir, endpoint, alive) SELECT arch.fullpath, arch.isdir, arch.endpoint, 0 FROM arch JOIN fs ON (arch.fullpath LIKE fs.fullpath||'/%') WHERE arch.endpoint=1").exec_()
		# - remove not endpoint
		QtSql.QSqlQuery("DELETE FROM fs WHERE endpoint=0").exec_()
		# 2. check exists
		q = QtSql.QSqlQuery("SELECT arch.id, fs.fullpath FROM fs JOIN arch ON (fs.fullpath = arch.fullpath)")
		toskip = list()
		toextract = list()
		ask = QtCore.QString()
		tocut = src.length() + 1 if src.length() else 0
		while (q.next()):
			id = q.value(0).toUInt()[0]
			s = q.value(1).toString()
			f = QtCore.QFileInfo(dest + "/" + s.mid(tocut))
			if (f.exists()):
				toskip.append(id)
				ask += (s + "\n")
			else:
				toextract.append(id)
		# 3. ask
		if (ask.length()):
			msg = QtGui.QMessageBox(
				QtGui.QMessageBox.Question,
				self.tr("John, I need help"),
				self.tr("Some folders and/or files to extract exist in filesystem"),
				QtGui.QMessageBox.Yes|QtGui.QMessageBox.No|QtGui.QMessageBox.Cancel
			)
			msg.setInformativeText(self.tr("Do you want to replace them?"))
			msg.setDetailedText(ask)
			responce = msg.exec_()
			action = {QtGui.QMessageBox.Yes:1, QtGui.QMessageBox.No:2, QtGui.QMessageBox.Cancel:3}[responce]
		else:
			action = 0
		# 4. go
		if (action == 3):
			return
		if (action == 1):	# replace
			toextract.extend(toskip)
		q = QtSql.QSqlQuery("SELECT fullpath FROM arch WHERE id IN (%s)" % ",".join(map(str, toextract)))
		while (q.next()):
			print q.value(0).toString()
