# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/mnt/shares/home/eugene/Version/SVN/qtdesktop/trunk/qgooglepim/src/ui/code.ui'
#
# Created: Wed Dec  1 22:09:09 2010
#      by: PyQt4 UI code generator 4.7.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_mwCode(object):
    def setupUi(self, mwCode):
        mwCode.setObjectName("mwCode")
        mwCode.setWindowModality(QtCore.Qt.ApplicationModal)
        mwCode.resize(259, 320)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/img/ui/img/google/code.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        mwCode.setWindowIcon(icon)
        self.centralWidget = QtGui.QWidget(mwCode)
        self.centralWidget.setObjectName("centralWidget")
        self.gridLayout = QtGui.QGridLayout(self.centralWidget)
        self.gridLayout.setObjectName("gridLayout")
        self.lvIssues = QtGui.QListView(self.centralWidget)
        self.lvIssues.setObjectName("lvIssues")
        self.gridLayout.addWidget(self.lvIssues, 0, 0, 1, 1)
        mwCode.setCentralWidget(self.centralWidget)
        self.menuBar = QtGui.QMenuBar(mwCode)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 259, 22))
        self.menuBar.setObjectName("menuBar")
        self.menu_Edit = QtGui.QMenu(self.menuBar)
        self.menu_Edit.setObjectName("menu_Edit")
        self.menu_File = QtGui.QMenu(self.menuBar)
        self.menu_File.setObjectName("menu_File")
        mwCode.setMenuBar(self.menuBar)
        self.statusBar = QtGui.QStatusBar(mwCode)
        self.statusBar.setObjectName("statusBar")
        mwCode.setStatusBar(self.statusBar)
        self.toolBar = QtGui.QToolBar(mwCode)
        self.toolBar.setObjectName("toolBar")
        mwCode.addToolBar(QtCore.Qt.ToolBarArea(QtCore.Qt.TopToolBarArea), self.toolBar)
        self.actionAdd = QtGui.QAction(mwCode)
        self.actionAdd.setObjectName("actionAdd")
        self.actionEdit = QtGui.QAction(mwCode)
        self.actionEdit.setObjectName("actionEdit")
        self.actionDel = QtGui.QAction(mwCode)
        self.actionDel.setObjectName("actionDel")
        self.actionExit = QtGui.QAction(mwCode)
        self.actionExit.setObjectName("actionExit")
        self.menu_Edit.addAction(self.actionAdd)
        self.menu_Edit.addAction(self.actionEdit)
        self.menu_Edit.addAction(self.actionDel)
        self.menu_File.addAction(self.actionExit)
        self.menuBar.addAction(self.menu_File.menuAction())
        self.menuBar.addAction(self.menu_Edit.menuAction())

        self.retranslateUi(mwCode)
        QtCore.QMetaObject.connectSlotsByName(mwCode)

    def retranslateUi(self, mwCode):
        mwCode.setWindowTitle(QtGui.QApplication.translate("mwCode", "Code", None, QtGui.QApplication.UnicodeUTF8))
        self.menu_Edit.setTitle(QtGui.QApplication.translate("mwCode", "&Edit", None, QtGui.QApplication.UnicodeUTF8))
        self.menu_File.setTitle(QtGui.QApplication.translate("mwCode", "&File", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("mwCode", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAdd.setText(QtGui.QApplication.translate("mwCode", "&Add", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEdit.setText(QtGui.QApplication.translate("mwCode", "&Edit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDel.setText(QtGui.QApplication.translate("mwCode", "&Del", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("mwCode", "E&xit", None, QtGui.QApplication.UnicodeUTF8))

import googlepim_rc

if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    mwCode = QtGui.QMainWindow()
    ui = Ui_mwCode()
    ui.setupUi(mwCode)
    mwCode.show()
    sys.exit(app.exec_())

