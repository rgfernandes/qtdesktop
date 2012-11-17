#!/bin/env python
# -*- coding: utf-8 -*-

import sys
from PyQt4 import QtCore, QtGui
from ui.Ui_mainwindow import Ui_MainWindow
#from    Ui_Main                 import Ui_Main
#from    Main_rc                 import *

def main():
	app = QtGui.QApplication(sys.argv)
	mwMain = QtGui.QMainWindow()
	uiMain = Ui_MainWindow()
	uiMain.setupUi( mwMain )
	mwMain.show()
	sys.exit(app.exec_())

if __name__ == '__main__':
    main()
