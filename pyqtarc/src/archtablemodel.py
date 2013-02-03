from PyQt4 import QtCore, QtGui, QtSql

class	ArchTableModel(QtSql.QSqlTableModel):
	def	__init__(self, parent = None, db = None):
		super(ArchTableModel, self).__init__(parent, db)
		self.__iconProvider = QtGui.QFileIconProvider()

	def	data(self, index, role):
		if (not index.isValid()):
			return QtCore.QVariant()
		if (role == QtCore.Qt.DecorationRole):	#index.column() == 2
			return self.__iconProvider.icon(QtGui.QFileIconProvider.Folder) if self.get_isdir(index) else self.__iconProvider.icon(QtGui.QFileIconProvider.File)
		return super(ArchTableModel, self).data(index, role)

	def	__get_any(self, index, column):
		'''
		return: QVariant - record field
		'''
		return index.sibling(index.row(), column).data()

	def	get_id(self, index):
		return self.__get_any(index, 0).toUInt()[0]

	def	get_parent(self, index):
		return self.__get_any(index, 1).toUInt()[0]

	def	get_name(self, index):
		return self.__get_any(index, 2).toString()

	def	get_isdir(self, index):
		return self.__get_any(index, 3).toBool()

	def	get_datetime(self, index):
		return self.__get_any(index, 4).toUInt()[0]	# FIXME:

	def	get_nsize(self, index):
		return self.__get_any(index, 5).toUInt()[0]

	def	get_csize(self, index):
		return self.__get_any(index, 6).toUInt()[0]

	def	get_path(self, index):
		return self.__get_any(index, 7).toString()
