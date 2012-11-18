class	ArchItemModel(QtCore.QAbstractItemModel):
	def	__init__(self,  archfile,  parent = None):
		super(ArchItemModel, self).__init__(parent)
		self.__archfile = archfile
		self.__iconProvider = QtCore.QFileIconProvider()

	def	index(self, row, column, index):
		if not self.hasIndex(index):
			return QModelIndex()
		if not index.isValid():
			parentItem = self.__archfile.getRoot()
		else:
			parentItem = index.internalPoiner()
		childItem = parentItem.getChild(row)
		if (childItem):
			return self.createIndex(row, column, childItem)
		else:
			return QModelIndex()

	def	parent(self, index):
		if (index.isValid()):
			n = index.internalPointer()
			p = n.parent()
			if (p != self.__archfile.getRoot()):
				return self.createIndex(p.getRow(), 0, p)
		return QModelIndex()

	def	rowCount(self, index):
		if (index.column() > 0):
			return 0
		if (index.isValid()):
			item = index.internalPointer()
		else:
			item = archive.getRoot()
		return item.childCount()

	def	columntCount(self, index):
		return 3

	def	data(self, index, role):
		if (not index.isValid()):
			return QtCore.QVariant()
		item = index.internalPointer()
		if (role == Qt.DisplayRole):
			section = index.column()
			if (section == 0):
				return item.getName()
			if (section == 1):
				return QString.number(item.getSize())
			if (section == 2):
				return item.getDateTime().toString("yyyy-MM-dd HH:mm:ss")
			else:
				return QtCore.QVariant()
		if (role == Qt.DecorationRole):
			if (index.column() == 0):
				return iconProvider.icon(QFileIconProvider.Folder) if item.isDir() else iconProvider.icon(QFileIconProvider.File)
		return QtCore.QVariant()

	def	headerData(self, section, orientation, role):
		if ((role == Qt.DisplayRole) and (orientation == Qt.Horizontal)):
			if (section == 0):
				return QtCore.QString(self.tr("Name"))
			elif (section == 1):
				return QtCore.QString(self.tr("Size"))
			elif (section == 2):
				return QtCore.QString(self.tr("Date"))
			else:
				return QtCore.QVariant()
		return QtCore.QVariant()

	def	flags(self, index):
		return (Qt.ItemIsSelectable|Qt.ItemIsEnabled|Qt.ItemIsDragEnabled|Qt.ItemIsDropEnabled) if index.isValid() else Qt.NoItemFlags

	def	item(self, index):
		return index.internalPointer()

	def	refresh(self):
		self.layoutChanged()
