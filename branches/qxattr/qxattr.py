#!/bin/env python
# -*- coding: utf-8 -*-
# Handling xattrs

import	sys, xattr
from PyQt4 import QtCore, QtGui

class	MainDialog(QtGui.QDialog):
	'''
	Main window.
	'''
	def __init__(self, file):
		self.file = file
		QtGui.QDialog.__init__(self)
		self.resize(QtCore.QSize(QtCore.QRect(0,0,315,300).size()).expandedTo(self.minimumSizeHint()))
		self.vbl = QtGui.QVBoxLayout(self)
		self.tw = QtGui.QTableWidget(self)
		self.tw.setColumnCount(2)
		headerItem = QtGui.QTableWidgetItem()
		headerItem.setText(QtGui.QApplication.translate("qxattr", "Attribute", None, QtGui.QApplication.UnicodeUTF8))
		self.tw.setHorizontalHeaderItem(0,headerItem)
		headerItem1 = QtGui.QTableWidgetItem()
		headerItem1.setText(QtGui.QApplication.translate("qxattr", "Value", None, QtGui.QApplication.UnicodeUTF8))
		self.tw.setHorizontalHeaderItem(1,headerItem1)
		self.vbl.addWidget(self.tw)
		self.hbl = QtGui.QHBoxLayout(self)
		self.addButton = QtGui.QPushButton("+", self)
		self.hbl.addWidget(self.addButton)
		self.delButton = QtGui.QPushButton("-", self)
		self.hbl.addWidget(self.delButton)
		self.hbl.addItem(QtGui.QSpacerItem(40,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum))
		self.buttonBox = QtGui.QDialogButtonBox(QtGui.QDialogButtonBox.Apply|QtGui.QDialogButtonBox.Ok|QtGui.QDialogButtonBox.Close, QtCore.Qt.Horizontal, self)
		self.hbl.addWidget(self.buttonBox)
		self.vbl.addLayout(self.hbl)
		QtCore.QObject.connect(self.buttonBox.button(QtGui.QDialogButtonBox.Apply), QtCore.SIGNAL("clicked()"), self.__apply)
		QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("accepted()"),self.accept)
		QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("rejected()"),self.reject)
		QtCore.QObject.connect(self.addButton, QtCore.SIGNAL("clicked()"),self.__add)
		QtCore.QObject.connect(self.delButton, QtCore.SIGNAL("clicked()"),self.__del)
		self.__load()

	def	__add(self):
		'''
		Add new xattr - new empty line into attr table;
		'''
		self.tw.setRowCount(self.tw.rowCount() + 1)

	def	__del(self):
		'''
		Delete existent attr.
		'''
		r = self.tw.currentRow()
		if (r >= 0):
			self.tw.removeRow(r)
	
	def	__load(self):
		'''
		Load all xattrs into mem.
		'''
		self.xattr0 = self.__file2hash(self.file)		# 1. load data
		self.__hash2table(self.xattr0, self.tw)		# 3. paint

	def	__apply(self):
		'''
		Save attrs and continue edititng.
		'''
		h = self.__table2hash(self.tw)			# 1. get new values
		h1 = self.__file2hash(self.file)		# 2. check that xattrs not changed.
		#print h, h1
		if (self.xattr0 != h1):				# FIXME: Force | Reload
			QtGui.QMessageBox.warning(self,\
				QtGui.QApplication.translate("qxattr", "Xattrs changed", None, QtGui.QApplication.UnicodeUTF8),\
				QtGui.QApplication.translate("qxattr", "File attributes changed due editing new. They will be rewrited.", None, QtGui.QApplication.UnicodeUTF8),\
			)
		self.__hash2file(h, h1, self.file)
		self.xattr0 = h1

	def	__file2hash(self, f):
		'''
		Load file's xattrs into hash
		@param f:file
		@return QHash
		'''
		h = {}
		alist = xattr.listxattr(self.file)
		for r, a in enumerate(alist):
			h[a[5:]] = xattr.getxattr(f, a)
		return h

	def	__hash2table(self, h, t):
		'''
		Put hash into table
		@param h:QHash - source
		@param t:QTableWidget - dest
		@return None
		'''
		if (len(h)):
			t.setRowCount(len(h))
			for r, a in enumerate(h.keys()):
				t.setItem(r, 0, QtGui.QTableWidgetItem(QtCore.QString().fromUtf8(a)))
				t.setItem(r, 1, QtGui.QTableWidgetItem(QtCore.QString().fromUtf8(h[a])))

	def	__table2hash(self, t):
		'''
		Get data from GUI into inner dict.
		@param f:QTableWidget
		@return dict
		'''
		h = {}
		for r in xrange(t.rowCount()):
			if ((t.item(r, 0) == None) or (t.item(r, 0).text().isEmpty())):		# 1. check empty key
				QtGui.QMessageBox.critical(self,\
					QtGui.QApplication.translate("qxattr", "Empty key", None, QtGui.QApplication.UnicodeUTF8),\
					QtGui.QApplication.translate("qxattr", "Attribute name is empty", None, QtGui.QApplication.UnicodeUTF8),\
				)
				return None
			a = str(t.item(r, 0).text().toUtf8())
			if h.has_key(a):										# 2. check double keys
				QtGui.QMessageBox.critical(self,\
					QtGui.QApplication.translate("qxattr", "Double key", None, QtGui.QApplication.UnicodeUTF8),\
					QtGui.QApplication.translate("qxattr", "Attribute name already exists", None, QtGui.QApplication.UnicodeUTF8),\
				)
				return None
			if ((t.item(r, 1) == None) or (t.item(r, 1).text().isEmpty())):		# 3. check empty value
				QtGui.QMessageBox.critical(self,\
					QtGui.QApplication.translate("qxattr", "Empty value", None, QtGui.QApplication.UnicodeUTF8),\
					QtGui.QApplication.translate("qxattr", "Attribute value is empty", None, QtGui.QApplication.UnicodeUTF8),\
				)
				return None
			h[a] = str(t.item(r, 1).text().toUtf8())
		return h

	def	__hash2file(self, h, h1, f):
		'''
		Save dict into file's xattrs.
		@param h:dict new values
		@param h1:dict old values
		@param f:file
		@return None
		'''
		# 1. delete deleted
		for k in h1.keys():
			if k not in h:
				xattr.removexattr(f, "user."+k)
		for r, k in enumerate(h.keys()):
			#print type(a).__name__, a, type(h[a]).__name__, h[a]
			if ((k not in h1) or (h[k] != h1[k])):
				xattr.setxattr(f, "user."+k, h[k])
				print k, "=", h[k]

if (__name__ == '__main__'):
	if (len(sys.argv) != 2):
		print "Usage: %s <filename>" % sys.argv[0]
		sys.exit(0)
	aMain = QtGui.QApplication( sys.argv )
	translator	= QtCore.QTranslator()
	translator.load("tr_" + QtCore.QLocale().system().name().left(2))
	aMain.installTranslator(translator)
	mwMain	= MainDialog(sys.argv[1])
	mwMain.show()
	sys.exit(aMain.exec_())
