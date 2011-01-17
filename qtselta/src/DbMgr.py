# -*- coding: utf-8 -*-
'''
DbMgr: database "manager"
'''

from PyQt4 import QtCore, QtGui, QtSql
import Var

class	DbMgr:
	def	__init__(self, mw):
		self.mw = mw
		self.maindb = None
		self.dbdict = dict()

	def	init(self):
		'''
		Creates db connection
		@return - bool: True if success
		'''
		self.maindb = self.__addDB('postgres')
		if not self.maindb.open():
			err = self.maindb.lastError()
			QtGui.QMessageBox.critical(self.mw,
				QtGui.QApplication.translate("DbMgr", "Opening database error"),
				QtGui.QApplication.translate("DbMgr", "Err no: %1\nDriver err: %2\nDB err: %3").arg(err.number()).arg(err.driverText()).arg(err.databaseText()),
				QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton)	#Unable to establish a database connection: $1
			self.maindb.removeDatabase("postgres")
			return False
		return True

	def	__loadSql(self, name):
		'''
		Load sql string from file
		'''
		txt = QtCore.QString()
		file = QtCore.QFile(":/sql/sql/" + name)
		if file.open(QtCore.QFile.ReadOnly | QtCore.QFile.Text):
			txt = QtCore.QString(file.readAll())
			file.close()
		return txt

	def	__addDB(self, dbname):
		'''
		Add new DB to db list
		'''
		db = QtSql.QSqlDatabase.addDatabase("QPSQL", dbname)
		db.setDatabaseName(dbname)
		db.setHostName(Var.Setting.Host)
		db.setUserName(Var.Setting.Login)
		db.setPassword(Var.Setting.Password)
		self.dbdict[dbname] = [db, None, ]	# db, version
		return db

	def	__doquery(self, q, s):
		'''
		@param wm:QMainWindow - the subj
		@param q:QSqlQuery - empty (?) query
		@param s:QString - SQL expression
		@return 
		'''
		retvalue = q.exec_(s)
		if (not retvalue):
			err = q.lastError()
			QtGui.QMessageBox.critical(self.mw,
				QtGui.QApplication.translate("DbMgr", "Saving error"),
				QtGui.QApplication.translate("DbMgr", "Driver error: %1;\nDatabase error: %2;\nSQL string: %3").arg(err.driverText()).arg(err.databaseText()).arg(s),
				QtGui.QMessageBox.Cancel,
				QtGui.QMessageBox.NoButton)
		return retvalue

	def	fillOut(self):
		'''
		Fill out inner array w/ PGSQL dbs
		@return: bool
		'''
		retvalue = False
		q = self.maindb.exec_(self.__loadSql('listdbs.sql'))
		if q:
			# ":/sql/sql/listdbs.sql"
			while q.next():
				dbname = q.value(0).toString()
				if (dbname != 'postgres'):
					db = self.__addDB(dbname)
			retvalue = True
		return retvalue

	def	askSeltaVer(self):
		'''
		Ask each db on Selta version
		'''
		qs = self.__loadSql('getver.sql')
		for i in self.dbdict.keys():
			# check ver
			db = self.dbdict[i][0]
			if db.open():
				q = QtSql.QSqlQuery(db)
				if q.exec_(qs):
					if q.next():
						self.dbdict[i][1] = q.value(0).toString()
				db.close()
			else:
				print "Can't open db"

	def	dbCreate(self, dbname):
		retvalue = False
		if self.maindb.open():
			if self.maindb.exec_(self.__loadSql('createdb.sql').arg(dbname).arg(Var.Setting.Login)):
				# FIXME: lastErr
				print self.maindb.lastError().number()
				self.__addDB(dbname)
				retvalue = True
			else:
				print "Can't create db"
			self.maindb.close()
		else:
			print "Can't open maindb"
		return retvalue

	def	dbDelete(self, dbname):
		retvalue = False
		if self.maindb.open():
			if self.maindb.exec_(self.__loadSql('dropdb.sql').arg(dbname)):
				# FIXME: lastErr
				del self.dbdict[dbname]
				retvalue = True
			else:
				print "Can't delete db"
			self.maindb.close()
		else:
			print "Can't open maindb"
		return retvalue

	def	dbRename(self, dbname, newdbname):
		retvalue = False
		if self.maindb.open():
			if self.maindb.exec_(self.__loadSql('renamedb.sql').arg(dbname).arg(newdbname)):
				# FIXME: lastErr
				self.dbdict[newdbname] = self.dbdict[dbname]
				del self.dbdict[dbname]
				retvalue = True
			else:
				print "Can't rename db"
			self.maindb.close()
		else:
			print "Can't open maindb"
		return retvalue
