# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/mnt/shares/home/eugene/Version/SVN/qtdesktop/trunk/qgooglepim/src/ui/settings.ui'
#
# Created: Wed Dec  1 22:09:06 2010
#      by: PyQt4 UI code generator 4.7.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_SettingsDialog(object):
    def setupUi(self, SettingsDialog):
        SettingsDialog.setObjectName("SettingsDialog")
        SettingsDialog.resize(226, 169)
        self.vboxlayout = QtGui.QVBoxLayout(SettingsDialog)
        self.vboxlayout.setObjectName("vboxlayout")
        self.gbBlog = QtGui.QGroupBox(SettingsDialog)
        self.gbBlog.setObjectName("gbBlog")
        self.gridlayout = QtGui.QGridLayout(self.gbBlog)
        self.gridlayout.setObjectName("gridlayout")
        self.lLogin = QtGui.QLabel(self.gbBlog)
        self.lLogin.setObjectName("lLogin")
        self.gridlayout.addWidget(self.lLogin, 0, 0, 1, 1)
        self.leLogin = QtGui.QLineEdit(self.gbBlog)
        self.leLogin.setObjectName("leLogin")
        self.gridlayout.addWidget(self.leLogin, 0, 1, 1, 1)
        self.lPassword = QtGui.QLabel(self.gbBlog)
        self.lPassword.setObjectName("lPassword")
        self.gridlayout.addWidget(self.lPassword, 1, 0, 1, 1)
        self.lePassword = QtGui.QLineEdit(self.gbBlog)
        self.lePassword.setObjectName("lePassword")
        self.gridlayout.addWidget(self.lePassword, 1, 1, 1, 1)
        self.vboxlayout.addWidget(self.gbBlog)
        self.hboxlayout = QtGui.QHBoxLayout()
        self.hboxlayout.setSpacing(6)
        self.hboxlayout.setMargin(0)
        self.hboxlayout.setObjectName("hboxlayout")
        spacerItem = QtGui.QSpacerItem(131, 31, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.hboxlayout.addItem(spacerItem)
        self.okButton = QtGui.QPushButton(SettingsDialog)
        self.okButton.setObjectName("okButton")
        self.hboxlayout.addWidget(self.okButton)
        self.cancelButton = QtGui.QPushButton(SettingsDialog)
        self.cancelButton.setObjectName("cancelButton")
        self.hboxlayout.addWidget(self.cancelButton)
        self.vboxlayout.addLayout(self.hboxlayout)
        self.lLogin.setBuddy(self.leLogin)
        self.lPassword.setBuddy(self.lePassword)

        self.retranslateUi(SettingsDialog)
        QtCore.QObject.connect(self.okButton, QtCore.SIGNAL("clicked()"), SettingsDialog.accept)
        QtCore.QObject.connect(self.cancelButton, QtCore.SIGNAL("clicked()"), SettingsDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(SettingsDialog)

    def retranslateUi(self, SettingsDialog):
        SettingsDialog.setWindowTitle(QtGui.QApplication.translate("SettingsDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.gbBlog.setTitle(QtGui.QApplication.translate("SettingsDialog", "&Blog", None, QtGui.QApplication.UnicodeUTF8))
        self.lLogin.setText(QtGui.QApplication.translate("SettingsDialog", "&Login:", None, QtGui.QApplication.UnicodeUTF8))
        self.lPassword.setText(QtGui.QApplication.translate("SettingsDialog", "&Password:", None, QtGui.QApplication.UnicodeUTF8))
        self.okButton.setText(QtGui.QApplication.translate("SettingsDialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.cancelButton.setText(QtGui.QApplication.translate("SettingsDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    SettingsDialog = QtGui.QDialog()
    ui = Ui_SettingsDialog()
    ui.setupUi(SettingsDialog)
    SettingsDialog.show()
    sys.exit(app.exec_())

