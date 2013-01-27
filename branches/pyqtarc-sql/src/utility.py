# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui, QtSql
from PyQt4.QtCore import qDebug

def	Debug(s):
		pass
		#qDebug(s)

def	db_err(self, q):
	'''
	TODO: add query string
	'''
	err = q.lastError()
	print "Err type:", err.type()
	print "Err no:", err.number()
	print "Drv txt:", err.driverText()
	print "DB txt:", err.databaseText()

def createConnection(db, mw):
	'''
	TODO: clean db
	'''
	s = QtGui.QDesktopServices.storageLocation(QtGui.QDesktopServices.DataLocation)
	d = QtCore.QDir(s)
	d.mkpath(s)
	db.setDatabaseName(s + QtCore.QDir.separator() + "pyqtarc.db")	# ~/.local/share/data/TI_Eugene/pyqtarc/qpyqtarc.db
	if (not db.open()):
		QtGui.QMessageBox.critical(0,
			QtCore.QObject.tr("Opening database error"),
			QtCore.QObject.tr("Unable to establish a database connection."))
		return False
	else:
		if (db.tables().isEmpty()):
			ok = False
			list = QtCore.QStringList()
			file = QtCore.QFile(":sql/sql/data.sql")
			if (not file.open(QtCore.QIODevice.ReadOnly | QtCore.QIODevice.Text)):
				QtGui.QMessageBox.critical(0, QtCore.QObject.tr("Error"), QtCore.QObject.tr("fail to open data.sql to read"))
				return False
			else:
				lst = QtCore.QString(file.readAll()).split("\n")
				file.close()
			if (not lst.isEmpty()):
				#print "create tables"
				q = QtSql.QSqlQuery(db)
				QtSql.QSqlDatabase.database().transaction()
				for s in lst:
					if (not s.isEmpty()):
						if (not q.exec_(s)):
							QtGui.QMessageBox.critical(mw,
								mw.tr("Creating database error"),
								mw.tr("Unable to execute query: %1").arg(s))
							return False
						else:
							ok = True;
				if (ok):
					ok = QtSql.QSqlDatabase.database().commit()
				if (ok):
					pass
					#QtGui.QMessageBox.information(mw,
					#	mw.tr("Creating database"),
					#	mw.tr("Database created OK"))
			else:
				QtGui.QMessageBox.critical(mw,
					mw.tr("Creating database error"),
					mw.tr("Unable to read sql."))
				return False
	return True

def	load_arch(result):
	'''
	Load archive into db
	'''
	def	add_record(id, parent, v, q):
		#print v[2].toTime_t()
		q.bindValue(":id", i)
		q.bindValue(":parent_id", parent)
		q.bindValue(":name", v[0])
		q.bindValue(":isdir", v[1])
		q.bindValue(":datetime", v[2].toTime_t() if v[2] else None)
		q.bindValue(":nsize", v[3])
		q.bindValue(":csize", v[4])
		q.bindValue(":fullpath", v[5])
		q.exec_()
	folderdict = dict()	# path: id for folders
	QtSql.QSqlDatabase.database().transaction()
	QtSql.QSqlQuery("DELETE FROM arch").exec_()
	q = QtSql.QSqlQuery()
	q.prepare("INSERT INTO arch (id, parent_id, name, isdir, datetime, nsize, csize, fullpath) VALUES (:id, :parent_id, :name, :isdir, :datetime, :nsize, :csize, :fullpath)")
	i = 1
	for v in result:	# name:QString, isdir:bool, datetime:QDateTime, nsize:ULong, csize:ULong
		Debug("Try to add: %s" % v[0])
		pathlist = v[0].split("/")	# FIXME: separator()
		name = pathlist.takeLast()
		parent_id = None
		path = QtCore.QString()
		sep = ""
		for n in pathlist:
			path = path + sep + n
			sep = "/"
			Debug("Try to search: %s" % path)
			id = folderdict.get(path, None)
			if (id):
				parent_id = id
				Debug("Found: %s" % path)
				continue
			Debug("Add new intermediate folder: (%d) %s (%s)" % (i, n, path))
			add_record(i, parent_id, (n, True, None, None, None, path), q)
			folderdict[path] = i
			parent_id = i
			i += 1
		if (not v[1]) or (not v[0] in folderdict):
			Debug("Add new item:(%d) %s (%s)" % (i, name, v[0]))
			add_record(i, parent_id, (name, v[1], v[2], v[3], v[4], v[0]), q)
			if (v[1]):
				Debug("Add new folder to cache:(%d) %s" % (i, v[0]))
				folderdict[v[0]] = i
			i += 1
	QtSql.QSqlDatabase.database().commit()

def	load_fs(paths):
	'''
	List folder into sql db - file/endfolder/folder, path
	@param paths:QStringList - absulute folders/files to list.
	'''
	absprefix = None
	QtSql.QSqlDatabase.database().commit()
	q = QtSql.QSqlQuery()
	q.exec_("DELETE FROM fs")
	q.prepare("INSERT INTO fs (fullpath, isdir, endpoint) VALUES (:fullpath, :isdir, :endpoint)")
	for f in paths:
		fi = QtCore.QFileInfo(f)
		if (absprefix == None):
			absprefix = fi.canonicalPath()
		relbase = fi.fileName()
		if (fi.isFile()):
			__add_entry(q, relbase, False, True)
		else:
			__walk(q, absprefix, relbase)
	q.exec_("UPDATE fs SET isinarch=1 WHERE fullpath IN (SELECT fs.fullpath FROM fs JOIN arch ON fs.fullpath = arch.fullpath)")
	return QtSql.QSqlDatabase.database().commit()

def	__walk(q, absprefix, relbase):
	'''
	Walk through filesystem tree adding dirs/files to database.
	@param absprefix:str - absulute path
	@param relbase:str - relative path
	'''
	__filter = QtCore.QDir.Dirs|QtCore.QDir.Files|QtCore.QDir.NoSymLinks|QtCore.QDir.NoDotAndDotDot|QtCore.QDir.Readable|QtCore.QDir.Hidden
	entrylist = QtCore.QDir(absprefix + "/" + relbase).entryInfoList(__filter)
	if entrylist:
		__add_entry(q, relbase, True, False)
		for f in entrylist:
			relpath = relbase + "/" + f.fileName()
			if f.isFile():
				__add_entry(q, relpath, False, True)
			else:
				__walk(q, absprefix, relpath)
	else:
		__add_entry(q, relbase, True, True)

def	__add_entry(q, path, isdir, endpoint):
	q.bindValue(":fullpath", path)
	q.bindValue(":isdir", isdir)
	q.bindValue(":endpoint", endpoint)
	q.exec_()
