# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/mnt/shares/data/eugene/Projects/GooglePIM/src/ui/settings.ui'
#
# Created: Sat Mar 29 14:17:03 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_SettingsDialog(object):
    def setupUi(self, SettingsDialog):
        SettingsDialog.setObjectName("SettingsDialog")
        SettingsDialog.resize(QtCore.QSize(QtCore.QRect(0,0,226,169).size()).expandedTo(SettingsDialog.minimumSizeHint()))

        self.vboxlayout = QtGui.QVBoxLayout(SettingsDialog)
        self.vboxlayout.setObjectName("vboxlayout")

        self.gbBlog = QtGui.QGroupBox(SettingsDialog)
        self.gbBlog.setObjectName("gbBlog")

        self.gridlayout = QtGui.QGridLayout(self.gbBlog)
        self.gridlayout.setObjectName("gridlayout")

        self.lLoginBlog = QtGui.QLabel(self.gbBlog)
        self.lLoginBlog.setObjectName("lLoginBlog")
        self.gridlayout.addWidget(self.lLoginBlog,0,0,1,1)

        self.leLoginBlog = QtGui.QLineEdit(self.gbBlog)
        self.leLoginBlog.setObjectName("leLoginBlog")
        self.gridlayout.addWidget(self.leLoginBlog,0,1,1,1)

        self.lPasswordBlog = QtGui.QLabel(self.gbBlog)
        self.lPasswordBlog.setObjectName("lPasswordBlog")
        self.gridlayout.addWidget(self.lPasswordBlog,1,0,1,1)

        self.lePasswordBlog = QtGui.QLineEdit(self.gbBlog)
        self.lePasswordBlog.setObjectName("lePasswordBlog")
        self.gridlayout.addWidget(self.lePasswordBlog,1,1,1,1)
        self.vboxlayout.addWidget(self.gbBlog)

        self.hboxlayout = QtGui.QHBoxLayout()
        self.hboxlayout.setSpacing(6)
        self.hboxlayout.setMargin(0)
        self.hboxlayout.setObjectName("hboxlayout")

        spacerItem = QtGui.QSpacerItem(131,31,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum)
        self.hboxlayout.addItem(spacerItem)

        self.okButton = QtGui.QPushButton(SettingsDialog)
        self.okButton.setObjectName("okButton")
        self.hboxlayout.addWidget(self.okButton)

        self.cancelButton = QtGui.QPushButton(SettingsDialog)
        self.cancelButton.setObjectName("cancelButton")
        self.hboxlayout.addWidget(self.cancelButton)
        self.vboxlayout.addLayout(self.hboxlayout)
        self.lLoginBlog.setBuddy(self.leLoginBlog)
        self.lPasswordBlog.setBuddy(self.lePasswordBlog)

        self.retranslateUi(SettingsDialog)
        QtCore.QObject.connect(self.okButton,QtCore.SIGNAL("clicked()"),SettingsDialog.accept)
        QtCore.QObject.connect(self.cancelButton,QtCore.SIGNAL("clicked()"),SettingsDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(SettingsDialog)

    def retranslateUi(self, SettingsDialog):
        SettingsDialog.setWindowTitle(QtGui.QApplication.translate("SettingsDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.gbBlog.setTitle(QtGui.QApplication.translate("SettingsDialog", "&Blog", None, QtGui.QApplication.UnicodeUTF8))
        self.lLoginBlog.setText(QtGui.QApplication.translate("SettingsDialog", "&Login:", None, QtGui.QApplication.UnicodeUTF8))
        self.lPasswordBlog.setText(QtGui.QApplication.translate("SettingsDialog", "&Password:", None, QtGui.QApplication.UnicodeUTF8))
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
