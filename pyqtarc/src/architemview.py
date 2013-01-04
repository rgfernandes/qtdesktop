from PyQt4 import QtCore, QtGui

class	ArchItemView(QtCore.QListView):
	def	__init__(self, parent = None):
		'''
		acceptDrops?
		DnD = DD
		selectionMode - extended
		selectionBehav - rows
		iswrapping - ok
		resizeMode - ajust
		viewMode - listview
		
		'''
		super(ArchItemView, self).__init__(parent)
	