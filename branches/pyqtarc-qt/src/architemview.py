from PyQt4 import QtCore, QtGui

class	ArchItemView(QtGui.QListView):
	def	__init__(self, archfile, parent = None):
		'''
		http://www.trinitydesktop.org/docs/qt4/model-view-dnd.html
		+acceptDrops?
		DnD = DD
		+selectionMode - extended
		+selectionBehav - rows
		iswrapping - ok
		+resizeMode - ajust
		[viewMode - listview
		
		self.treeView = QtGui.QListView(self.centralwidget)
		self.treeView.setAcceptDrops(True)
		self.treeView.setDragDropMode(QtGui.QAbstractItemView.DragDrop)
		self.treeView.setAlternatingRowColors(False)
		self.treeView.setSelectionMode(QtGui.QAbstractItemView.ExtendedSelection)
		self.treeView.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)
		self.treeView.setMovement(QtGui.QListView.Free)
		self.treeView.setFlow(QtGui.QListView.TopToBottom)
		self.treeView.setProperty("isWrapping", True)
		self.treeView.setResizeMode(QtGui.QListView.Adjust)
		self.treeView.setViewMode(QtGui.QListView.ListMode)
		self.treeView.setObjectName(_fromUtf8("treeView"))
		self.verticalLayout.addWidget(self.treeView)
		'''
		super(ArchItemView, self).__init__(parent)
		self.__archfile = archfile
		self.setAcceptDrops(True)
		self.setDropIndicatorShown(True)
		self.setDragEnabled(True)
		self.setDragDropMode(QtGui.QAbstractItemView.DragDrop)
		self.setResizeMode(QtGui.QListView.Adjust)
		self.setMovement(QtGui.QListView.Free)
		self.setSelectionBehavior (QtGui.QAbstractItemView.SelectRows)
		self.setSelectionMode (QtGui.QAbstractItemView.ExtendedSelection)
		self.setWrapping(True)

	def	dragEnterEvent(self, event):
		event.acceptProposedAction()

	def	dragMoveEvent(self, event):
		event.acceptProposedAction()

	def	dragLeaveEvent(self, event):
		event.accept()

	def	dropEvent(self, event):	# QDropEvent
		print "drop:"
		mimeData = event.mimeData()	# QMimeData
		print "Formats:"
		for i in mimeData.formats():	# Xarchiver: XdndDirectSave0; ark: application/x-kde-ark-dndextract-service; application/x-kde-ark-dndextract-path
			print i
		print mimeData.data("XdndDirectSave0")
		if mimeData.hasUrls():
			fileNames = list()
			for i in mimeData.urls():	# QtCore.QUrl
				if i.isLocalFile():
					fileNames.append(i.toLocalFile())
				else:
					print "Non-file url:", i
			self.__archfile.add(fileNames)
			self.model().refresh()
		if mimeData.hasText():
			print "Text:"
			print mimeData.text()
		if mimeData.hasHtml():
			print "Html:"
			print mimeData.html()


