import sys
from PyQt4 import QtCore, QtGui
from mainwindow import MainWindow

def main():
	app = QtGui.QApplication(sys.argv)
	# translation
	translator = QtCore.QTranslator()
	trpath = QtCore.QLibraryInfo.location(QtCore.QLibraryInfo.TranslationsPath)
	trfile = QtCore.QString('pyqtarc') + '_' + QtCore.QLocale.system().name().section('_', 0, 0)
	if (not QtCore.QFile.exists(trpath + QtCore.QDir.separator() + trfile)):
		trpath = QtCore.QString('i18n')
	translator.load(trpath + QtCore.QDir.separator() + trfile)
	app.installTranslator(translator)
	# letsgo
	mw = MainWindow()
	mw.show()
	sys.exit(app.exec_())
