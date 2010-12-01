# -*- coding: utf-8 -*-
'''
'''

from	PyQt4	import QtCore, QtGui
import		core.blog

class	BlogModel(QtCore.QAbstractItemModel):
	def	__init__(self, root, parent = None):
		QtCore.QAbstractItemModel.__init__(self, parent)
		self.rootNode = core.blog._parent0()
		root.Parent = self.rootNode
		self.rootNode.Entity.append(root)

	def	columnCount(self, index):
		return 2;

	def	flags(self, index):
		if ( index.isValid() ):
			return QtCore.Qt.ItemIsEnabled | QtCore.Qt.ItemIsSelectable
		else:
			return QtCore.Qt.ItemIsEnabled

	def	headerData(self, section, orientation, role):
		if (orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole):
			if (section == 0):
			    return QtCore.QVariant(QtGui.QApplication.translate("Main", "Title"))
			elif (section == 1):
			    return QtCore.QVariant(QtGui.QApplication.translate("Main", "Updated"))
		return QtCore.QVariant()

	def	data(self, index, role):
		if not index.isValid():
			return QtCore.QVariant()
		if role != QtCore.Qt.DisplayRole:
			return QtCore.QVariant()
		col = index.column()
		node = index.internalPointer()
		if (col == 0):
			retvalue = QtCore.QVariant(QtCore.QString().fromUtf8(unicode(node.Title).encode('utf-8')))
		elif (col == 1):
			retvalue = QtCore.QVariant(QtCore.QString(node.Updated))
		else:
			retvalue = QtCore.QVariant()
##		print "role=%d, col=%d, data=%s" % (role, col, str(retvalue.toString()))
		return retvalue

	def	rowCount(self, index):
		if not index.isValid():
			node = self.rootNode
		else:
			node = index.internalPointer()
##		print "rows:", len(node.Entity)
		return len(node.Entity)

	def	index(self, row, column, parent):
		if row < 0 or column < 0 or row >= self.rowCount(parent) or column >= 2:
			print "nothing 1"
			return QtCore.QModelIndex()
		if not parent.isValid():
			parentNode = self.rootNode
		else:
			parentNode = parent.internalPointer()
		childNode = parentNode.Entity[row]
		if childNode:
##			print "index: row=%d, col=%d, node=%s" % (row, column, childNode.Id)
			return self.createIndex(row, column, childNode)
		else:
			print "nothing 2"
			return QtCore.QModelIndex()

	def	parent(self, index):
		if not index.isValid():
			return QtCore.QModelIndex()
		parentNode = index.internalPointer().Parent
		if (parentNode == None):	# root
			return QtCore.QModelIndex()
		grandparentNode = parentNode.Parent
		if (grandparentNode == None):
			return QtCore.QModelIndex()
		row = grandparentNode.Entity.index(parentNode)
##		print "parent: row=%d, node=%s" % (row, parentNode.Id)
		return self.createIndex(row, index.column(), parentNode)
