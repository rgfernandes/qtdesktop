# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/mnt/shares/data/eugene/Projects/GooglePIM/src/ui/main.ui'
#
# Created: Wed Apr  9 19:54:41 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(QtCore.QSize(QtCore.QRect(0,0,411,322).size()).expandedTo(MainWindow.minimumSizeHint()))
        MainWindow.setWindowIcon(QtGui.QIcon("../../../../../../home/eugene/Projects/GooglePIM/src/ui/img/google.gif"))

        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        MainWindow.setCentralWidget(self.centralwidget)

        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0,0,411,21))
        self.menubar.setObjectName("menubar")

        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")

        self.menuEdit = QtGui.QMenu(self.menubar)
        self.menuEdit.setObjectName("menuEdit")

        self.menuService = QtGui.QMenu(self.menubar)
        self.menuService.setObjectName("menuService")

        self.menuHelp = QtGui.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        MainWindow.setMenuBar(self.menubar)

        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.actionBase = QtGui.QAction(MainWindow)
        self.actionBase.setObjectName("actionBase")

        self.actionBlog = QtGui.QAction(MainWindow)
        self.actionBlog.setIcon(QtGui.QIcon("../../../../../../home/eugene/Projects/GooglePIM/src/ui/img/35x35/blogger.gif"))
        self.actionBlog.setObjectName("actionBlog")

        self.actionCalendar = QtGui.QAction(MainWindow)
        self.actionCalendar.setIcon(QtGui.QIcon("../../../../../../home/eugene/Projects/GooglePIM/src/ui/img/35x35/calendar.gif"))
        self.actionCalendar.setObjectName("actionCalendar")

        self.actionDocs = QtGui.QAction(MainWindow)
        self.actionDocs.setIcon(QtGui.QIcon("../../../../../../home/eugene/Projects/GooglePIM/src/ui/img/35x35/dns.gif"))
        self.actionDocs.setObjectName("actionDocs")

        self.actionMail = QtGui.QAction(MainWindow)
        self.actionMail.setIcon(QtGui.QIcon("img/35x35/gmail.gif"))
        self.actionMail.setObjectName("actionMail")

        self.actionPhoto = QtGui.QAction(MainWindow)
        self.actionPhoto.setIcon(QtGui.QIcon("../../../../../../home/eugene/Projects/GooglePIM/src/ui/img/35x35/picasa.gif"))
        self.actionPhoto.setObjectName("actionPhoto")

        self.actionContacts = QtGui.QAction(MainWindow)
        self.actionContacts.setObjectName("actionContacts")

        self.actionSettings = QtGui.QAction(MainWindow)
        self.actionSettings.setObjectName("actionSettings")

        self.actionConnect = QtGui.QAction(MainWindow)
        self.actionConnect.setObjectName("actionConnect")

        self.actionExit = QtGui.QAction(MainWindow)
        self.actionExit.setObjectName("actionExit")

        self.actionAbout = QtGui.QAction(MainWindow)
        self.actionAbout.setObjectName("actionAbout")

        self.actionNotebook = QtGui.QAction(MainWindow)
        self.actionNotebook.setIcon(QtGui.QIcon("../../../../../../home/eugene/Projects/GooglePIM/src/ui/img/35x35/notebook.gif"))
        self.actionNotebook.setObjectName("actionNotebook")
        self.menuFile.addAction(self.actionExit)
        self.menuService.addAction(self.actionBase)
        self.menuService.addAction(self.actionBlog)
        self.menuService.addAction(self.actionCalendar)
        self.menuService.addAction(self.actionContacts)
        self.menuService.addAction(self.actionDocs)
        self.menuService.addAction(self.actionNotebook)
        self.menuService.addAction(self.actionMail)
        self.menuService.addAction(self.actionPhoto)
        self.menuService.addSeparator()
        self.menuService.addAction(self.actionSettings)
        self.menuService.addAction(self.actionConnect)
        self.menuHelp.addAction(self.actionAbout)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuEdit.menuAction())
        self.menubar.addAction(self.menuService.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QObject.connect(self.actionExit,QtCore.SIGNAL("triggered()"),MainWindow.close)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "GooglePIM", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "&File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuEdit.setTitle(QtGui.QApplication.translate("MainWindow", "&Edit", None, QtGui.QApplication.UnicodeUTF8))
        self.menuService.setTitle(QtGui.QApplication.translate("MainWindow", "&Service", None, QtGui.QApplication.UnicodeUTF8))
        self.menuHelp.setTitle(QtGui.QApplication.translate("MainWindow", "&Help", None, QtGui.QApplication.UnicodeUTF8))
        self.actionBase.setText(QtGui.QApplication.translate("MainWindow", "&Base", None, QtGui.QApplication.UnicodeUTF8))
        self.actionBlog.setText(QtGui.QApplication.translate("MainWindow", "B&log", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCalendar.setText(QtGui.QApplication.translate("MainWindow", "&Calendar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDocs.setText(QtGui.QApplication.translate("MainWindow", "&Docs", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMail.setText(QtGui.QApplication.translate("MainWindow", "&Mail", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPhoto.setText(QtGui.QApplication.translate("MainWindow", "&Photo", None, QtGui.QApplication.UnicodeUTF8))
        self.actionContacts.setText(QtGui.QApplication.translate("MainWindow", "&Contacts", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSettings.setText(QtGui.QApplication.translate("MainWindow", "&Settings", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConnect.setText(QtGui.QApplication.translate("MainWindow", "Co&nnect", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("MainWindow", "&Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAbout.setText(QtGui.QApplication.translate("MainWindow", "&About", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNotebook.setText(QtGui.QApplication.translate("MainWindow", "&Notebook", None, QtGui.QApplication.UnicodeUTF8))



if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    MainWindow = QtGui.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
