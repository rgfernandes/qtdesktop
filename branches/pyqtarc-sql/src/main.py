import sys
from PyQt4 import QtCore, QtGui, QtSql
from mainwindow import MainWindow

def createConnection(db, mw):
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
				print "create tables"
				q = QtSql.QSqlQuery(db)
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
					QtGui.QMessageBox.information(mw,
						mw.tr("Creating database"),
						mw.tr("Database created OK"))
			else:
				QtGui.QMessageBox.critical(mw,
					mw.tr("Creating database error"),
					mw.tr("Unable to read sql."))
				return False
	return True

def main():
	app = QtGui.QApplication(sys.argv)
	QtCore.QCoreApplication.setApplicationName("pyqtarc")
	QtCore.QCoreApplication.setApplicationVersion("0.0.1")
	QtCore.QCoreApplication.setOrganizationName("TI_Eugene")
	QtCore.QCoreApplication.setOrganizationDomain("eap.su")
	# <i18n>
	translator = QtCore.QTranslator()
	trpath = QtCore.QLibraryInfo.location(QtCore.QLibraryInfo.TranslationsPath)
	trfile = QtCore.QString('pyqtarc') + '_' + QtCore.QLocale.system().name().section('_', 0, 0)
	if (not QtCore.QFile.exists(trpath + QtCore.QDir.separator() + trfile)):
		trpath = QtCore.QString('i18n')
	translator.load(trpath + QtCore.QDir.separator() + trfile)
	app.installTranslator(translator)
	# </i18n>
	# letsgo
	#QtCore.QTextCodec.setCodecForCStrings(QTextCodec.codecForName("UTF-8"))
	mw = MainWindow()
	mw.show()
	# <db>
	db = QtSql.QSqlDatabase.addDatabase("QSQLITE")
	if (not createConnection(db, mw)):
		return 1
	# </db>
	mw.setModel(db)
	argv = app.arguments()
	if (argv.count() == 2):
		mw._file_open(argv[1])
	retvalue = app.exec_()
	del mw
	db.close()
	sys.exit(retvalue)
