# -*- coding: utf-8 -*-
'''
'''

import	sys
from	PyQt4	import QtCore, QtGui

def	InitTree():
	root = Node()

	group1 = Node(root, QtGui.QApplication.translate("Main", "1"), QtGui.QApplication.translate("Main", "Administer"), "" )
	root.children.append(group1)

	group11 = Node(group1, QtGui.QApplication.translate("Main", "1.1"), QtGui.QApplication.translate("Main", "Accounting"), "" )
	group1.children.append(group11)

	group12 = Node(group1, QtGui.QApplication.translate("Main", "1.2"), QtGui.QApplication.translate("Main", "Planning"), "" )
	group1.children.append(group12);

	group2 = Node(root, QtGui.QApplication.translate("Main", "2"), QtGui.QApplication.translate("Main", "Enterprise"), "" )
	root.children.append(group2)

	group21 = Node(group2, QtGui.QApplication.translate("Main", "2.1"), QtGui.QApplication.translate("Main", "Shop 1"), "" )
	group2.children.append(group21)

	group22 = Node(group2, QtGui.QApplication.translate("Main", "2.2"), QtGui.QApplication.translate("Main", "Shop 2"), "" )
	group2.children.append(group22)

	group11.children.append( Node(
		group11, QtGui.QApplication.translate("Main", "1.1.1"),
		QtGui.QApplication.translate("Main", "Ivanov A.P."),
		QtGui.QApplication.translate("Main", "12 345 rub.") ) )
##    group11.children.append( new Node(
##                    group11, QtGui.QApplication.translate("1.1.2"),
##                    QtGui.QApplication.translate("Петрова Е.Н."),
##                    QtGui.QApplication.translate("11 234 руб.") ) );
##
##    group12.children.append( new Node(
##                    group12, QtGui.QApplication.translate("1.2.1"),
##                    QtGui.QApplication.translate("Сергеев К.А."),
##                    QtGui.QApplication.translate("14 310 руб.") ) );
##    group12.children.append( new Node(
##                    group12, QtGui.QApplication.translate("1.2.2"),
##                    QtGui.QApplication.translate("Константинов Р.Б."),
##                    QtGui.QApplication.translate("12 497 руб.") ) );
##
##    group21.children.append( new Node(
##                    group21, QtGui.QApplication.translate("2.1.1"),
##                    QtGui.QApplication.translate("Николаев Б.К."),
##                    QtGui.QApplication.translate("16 235 руб.") ) );
##    group21.children.append( new Node(
##                    group21, QtGui.QApplication.translate("2.1.2"),
##                    QtGui.QApplication.translate("Павлов О.В."),
##                    QtGui.QApplication.translate("13 751 руб.") ) );
##
##    group22.children.append( new Node(
##                    group22, QtGui.QApplication.translate("2.2.1"),
##                    QtGui.QApplication.translate("Григорьев В.Р."),
##                    QtGui.QApplication.translate("13 793 руб.") ) );
##    group22.children.append( new Node(
##                    group22, QtGui.QApplication.translate("2.2.2"),
##                    QtGui.QApplication.translate("Сидоров Н.К."),
##                    QtGui.QApplication.translate("12 645 руб.") ) );
	return root

if (__name__ == '__main__'):
	app = QtGui.QApplication(sys.argv)
	codec = QtCore.QTextCodec.codecForName("CP1251")
	QtCore.QTextCodec.setCodecForTr(codec)

	model = TreeModel()
	model.setRootNode(InitTree())

	view = QtGui.QTreeView()
	view.setSelectionMode(QtGui.QAbstractItemView.SingleSelection)
	view.setSelectionBehavior(QtGui.QAbstractItemView.SelectItems)
	view.setModel(model)
	view.expandAll()
	view.header().setStretchLastSection(True)
	view.resizeColumnToContents(1)

	view.setWindowTitle(QtGui.QApplication.translate("Main", "TreeModel и QTreeView"))
	view.resize(350, 300)
	view.show()

	app.exec_()
