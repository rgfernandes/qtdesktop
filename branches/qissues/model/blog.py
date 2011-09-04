# -*- coding: utf-8 -*-
'''
'''

from	PyQt4	import QtCore, QtGui
import		core.blog

class	BlogModel(QtCore.QAbstractItemModel):
	def	__init__(self, root, parent = None):
		QtCore.QAbstractItemModel.__init__(self, parent)
		self.rootItem = core.blog._parent0(parent)
		root.Parent = self.rootItem
		self.rootItem.Entry.append(root)

	def	columnCount(self, index):
		return 2;

	def data(self, index, role):
		if not index.isValid():
			return QtCore.QVariant()
		if role != QtCore.Qt.DisplayRole:
			return QtCore.QVariant()
		item = index.internalPointer()
		col = index.column()
		if (col == 0):
			return QtCore.QVariant(QtCore.QString().fromUtf8(item.Title)) if item.Title else QtCore.QVariant()
		elif (col == 1):
			return QtCore.QVariant(QtCore.QDateTime(item.Updated))

	def flags(self, index):
		if not index.isValid():
			return QtCore.Qt.ItemIsEnabled
		return QtCore.Qt.ItemIsEnabled | QtCore.Qt.ItemIsSelectable

	def headerData(self, section, orientation, role):
		if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
			return QtCore.QVariant(("Title", "Updated")[section])
		return QtCore.QVariant()

	def index(self, row, column, parent):
		if row < 0 or column < 0 or row >= self.rowCount(parent) or column >= 2:
			return QtCore.QModelIndex()
		if not parent.isValid():
			parentItem = self.rootItem
		else:
			parentItem = parent.internalPointer()
		childItem = parentItem.child(row)
		if childItem:
			return self.createIndex(row, column, childItem)
		else:
			return QtCore.QModelIndex()

	def parent(self, index):
		if not index.isValid():
			return QtCore.QModelIndex()
		childItem = index.internalPointer()
		parentItem = childItem.parent()
		if parentItem == self.rootItem:
			return QtCore.QModelIndex()
		return self.createIndex(parentItem.row(), 0, parentItem)

	def rowCount(self, parent):
		if parent.column() > 0:
			return 0
		if not parent.isValid():
			parentItem = self.rootItem
		else:
			parentItem = parent.internalPointer()
		return parentItem.childCount()
