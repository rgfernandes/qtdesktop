# -*- coding: utf-8 -*-

import os
from PyQt4 import QtCore, QtGui
import Var

def	About() :
	about = QtGui.QMessageBox()
	about.setText(QtGui.QApplication.translate("Help", "About %1").arg(Var.AppName))
	about.setInformativeText(QtGui.QApplication.translate("Help",
			"<center><a href=\"%1\">%2</a></b> version %3 - Selta@Etersoft GUIi frontend.<br/>\n"
			"Designed by <a href=\"mailto:%4\">%5</a> for <a href=\"http://www.etersoft.ru/\">\"Etersoft\", Ltd.</a><br/>\n"
			"Powered with <a href=\"http://www.python.org\">Python</a> and <a href=\"http://www.riverbankcomputing.co.uk/pyqt\">PyQt4</a>.</center>"
			)
			.arg(Var.AppHomePage)
			.arg(Var.AppName)
			.arg(Var.AppVersion)
			.arg(Var.AppAuthorEMail)
			.arg(Var.AppAuthorName)
		)
	about.setStandardButtons(QtGui.QMessageBox.Ok)
	about.setIconPixmap(QtGui.QPixmap(":/icons/etersoft_logo.png"))
	about.exec_()

def	AboutQt4() :
	QtGui.QApplication.aboutQt()

def	Help():
	help = QtGui.QMessageBox()
	help.setText(QtGui.QApplication.translate("Help", "Help"))
	help.setStandardButtons(QtGui.QMessageBox.Ok)
	file = QtCore.QFile(QtCore.QString(os.getcwd()) + QtCore.QDir.separator() + Var.DirHelp + QtCore.QDir.separator() + "help.html")
	if (file.open(QtCore.QIODevice.ReadOnly | QtCore.QIODevice.Text)):
		help.setInformativeText(QtCore.QString().fromUtf8(file.readAll()))
		help.exec_()
	file.close()
