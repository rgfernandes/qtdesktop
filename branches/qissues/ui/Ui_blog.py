# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/mnt/shares/data/eugene/Projects/GooglePIM/src/ui/blog.ui'
#
# Created: Wed Apr  9 14:49:13 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_BlogDialog(object):
    def setupUi(self, BlogDialog):
        BlogDialog.setObjectName("BlogDialog")
        BlogDialog.resize(QtCore.QSize(QtCore.QRect(0,0,554,402).size()).expandedTo(BlogDialog.minimumSizeHint()))
        BlogDialog.setWindowIcon(QtGui.QIcon("../../../../../../home/eugene/Projects/GooglePIM/src/ui/img/35x35/blogger.gif"))

        self.vboxlayout = QtGui.QVBoxLayout(BlogDialog)
        self.vboxlayout.setObjectName("vboxlayout")

        self.splitter_2 = QtGui.QSplitter(BlogDialog)
        self.splitter_2.setOrientation(QtCore.Qt.Horizontal)
        self.splitter_2.setObjectName("splitter_2")

        self.splitter = QtGui.QSplitter(self.splitter_2)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setObjectName("splitter")

        self.treeView = QtGui.QTreeView(self.splitter)
        self.treeView.setObjectName("treeView")

        self.tbContent = QtGui.QTextBrowser(self.splitter)
        self.tbContent.setObjectName("tbContent")

        self.layoutWidget = QtGui.QWidget(self.splitter_2)
        self.layoutWidget.setObjectName("layoutWidget")

        self.gridlayout = QtGui.QGridLayout(self.layoutWidget)
        self.gridlayout.setObjectName("gridlayout")

        self.labelId = QtGui.QLabel(self.layoutWidget)
        self.labelId.setObjectName("labelId")
        self.gridlayout.addWidget(self.labelId,0,0,1,1)

        self.lineEditId = QtGui.QLineEdit(self.layoutWidget)
        self.lineEditId.setObjectName("lineEditId")
        self.gridlayout.addWidget(self.lineEditId,0,1,1,1)

        self.labelTitle = QtGui.QLabel(self.layoutWidget)
        self.labelTitle.setObjectName("labelTitle")
        self.gridlayout.addWidget(self.labelTitle,1,0,1,1)

        self.lineEditTitle = QtGui.QLineEdit(self.layoutWidget)
        self.lineEditTitle.setObjectName("lineEditTitle")
        self.gridlayout.addWidget(self.lineEditTitle,1,1,1,1)

        self.labelPublished = QtGui.QLabel(self.layoutWidget)
        self.labelPublished.setObjectName("labelPublished")
        self.gridlayout.addWidget(self.labelPublished,2,0,1,1)

        self.labelUpdated = QtGui.QLabel(self.layoutWidget)
        self.labelUpdated.setObjectName("labelUpdated")
        self.gridlayout.addWidget(self.labelUpdated,3,0,1,1)

        self.labelAuthor = QtGui.QLabel(self.layoutWidget)
        self.labelAuthor.setObjectName("labelAuthor")
        self.gridlayout.addWidget(self.labelAuthor,4,0,1,1)

        self.checkBoxDraft = QtGui.QCheckBox(self.layoutWidget)
        self.checkBoxDraft.setObjectName("checkBoxDraft")
        self.gridlayout.addWidget(self.checkBoxDraft,4,1,1,1)

        self.twAuthor = QtGui.QTableWidget(self.layoutWidget)
        self.twAuthor.setObjectName("twAuthor")
        self.gridlayout.addWidget(self.twAuthor,5,0,2,2)

        self.dateTimeEditPublished = QtGui.QDateTimeEdit(self.layoutWidget)
        self.dateTimeEditPublished.setObjectName("dateTimeEditPublished")
        self.gridlayout.addWidget(self.dateTimeEditPublished,2,1,1,1)

        self.dateTimeEditUpdated = QtGui.QDateTimeEdit(self.layoutWidget)
        self.dateTimeEditUpdated.setObjectName("dateTimeEditUpdated")
        self.gridlayout.addWidget(self.dateTimeEditUpdated,3,1,1,1)
        self.vboxlayout.addWidget(self.splitter_2)

        self.hboxlayout = QtGui.QHBoxLayout()
        self.hboxlayout.setSpacing(6)
        self.hboxlayout.setMargin(0)
        self.hboxlayout.setObjectName("hboxlayout")

        self.pushButtonAdd = QtGui.QPushButton(BlogDialog)
        self.pushButtonAdd.setObjectName("pushButtonAdd")
        self.hboxlayout.addWidget(self.pushButtonAdd)

        self.pushButtonDelete = QtGui.QPushButton(BlogDialog)
        self.pushButtonDelete.setObjectName("pushButtonDelete")
        self.hboxlayout.addWidget(self.pushButtonDelete)

        self.pushButtonSubmit = QtGui.QPushButton(BlogDialog)
        self.pushButtonSubmit.setObjectName("pushButtonSubmit")
        self.hboxlayout.addWidget(self.pushButtonSubmit)

        spacerItem = QtGui.QSpacerItem(131,31,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum)
        self.hboxlayout.addItem(spacerItem)

        self.okButton = QtGui.QPushButton(BlogDialog)
        self.okButton.setObjectName("okButton")
        self.hboxlayout.addWidget(self.okButton)

        self.cancelButton = QtGui.QPushButton(BlogDialog)
        self.cancelButton.setObjectName("cancelButton")
        self.hboxlayout.addWidget(self.cancelButton)
        self.vboxlayout.addLayout(self.hboxlayout)
        self.labelId.setBuddy(self.lineEditId)
        self.labelTitle.setBuddy(self.lineEditTitle)
        self.labelPublished.setBuddy(self.dateTimeEditPublished)
        self.labelUpdated.setBuddy(self.dateTimeEditUpdated)
        self.labelAuthor.setBuddy(self.twAuthor)

        self.retranslateUi(BlogDialog)
        QtCore.QObject.connect(self.okButton,QtCore.SIGNAL("clicked()"),BlogDialog.accept)
        QtCore.QObject.connect(self.cancelButton,QtCore.SIGNAL("clicked()"),BlogDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(BlogDialog)

    def retranslateUi(self, BlogDialog):
        BlogDialog.setWindowTitle(QtGui.QApplication.translate("BlogDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.labelId.setText(QtGui.QApplication.translate("BlogDialog", "IId:", None, QtGui.QApplication.UnicodeUTF8))
        self.labelTitle.setText(QtGui.QApplication.translate("BlogDialog", "&Title:", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPublished.setText(QtGui.QApplication.translate("BlogDialog", "&Published:", None, QtGui.QApplication.UnicodeUTF8))
        self.labelUpdated.setText(QtGui.QApplication.translate("BlogDialog", "&Updated:", None, QtGui.QApplication.UnicodeUTF8))
        self.labelAuthor.setText(QtGui.QApplication.translate("BlogDialog", "&Authors:", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBoxDraft.setText(QtGui.QApplication.translate("BlogDialog", "&Draft", None, QtGui.QApplication.UnicodeUTF8))
        self.twAuthor.clear()
        self.twAuthor.setColumnCount(1)
        self.twAuthor.setRowCount(0)

        headerItem = QtGui.QTableWidgetItem()
        headerItem.setText(QtGui.QApplication.translate("BlogDialog", "Name", None, QtGui.QApplication.UnicodeUTF8))
        self.twAuthor.setHorizontalHeaderItem(0,headerItem)
        self.pushButtonAdd.setText(QtGui.QApplication.translate("BlogDialog", "&Add", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButtonDelete.setText(QtGui.QApplication.translate("BlogDialog", "&Delete", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButtonSubmit.setText(QtGui.QApplication.translate("BlogDialog", "&Submit", None, QtGui.QApplication.UnicodeUTF8))
        self.okButton.setText(QtGui.QApplication.translate("BlogDialog", "&OK", None, QtGui.QApplication.UnicodeUTF8))
        self.cancelButton.setText(QtGui.QApplication.translate("BlogDialog", "&Cancel", None, QtGui.QApplication.UnicodeUTF8))



if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    BlogDialog = QtGui.QDialog()
    ui = Ui_BlogDialog()
    ui.setupUi(BlogDialog)
    BlogDialog.show()
    sys.exit(app.exec_())
