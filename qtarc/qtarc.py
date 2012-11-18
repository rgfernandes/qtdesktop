#!/bin/env python
# -*- coding: utf-8 -*-

import sys
from PyQt4 import QtCore, QtGui
from ui.Ui_mainwindow import Ui_MainWindow
#from    Ui_Main                 import Ui_Main
#from    Main_rc                 import *

class	MainWindow(Ui_MainWindow):
	def init_signals(self):
		'''
		Setup window slots
		'''

def main():
	app = QtGui.QApplication(sys.argv)
	mwMain = QtGui.QMainWindow()
	uiMain = MainWindow()
	uiMain.setupUi( mwMain )
	mwMain.show()
	sys.exit(app.exec_())

if __name__ == '__main__':
    main()
