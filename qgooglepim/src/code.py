# -*- coding: utf-8 -*-
'''
Account: Blogs > Posts > Comments.
TODO: QTreeView + QAbstractItemModel derived model.
'''

from	gdata	import service
import		gdata, atom, sys, pprint
from	PyQt4	import QtCore, QtGui
from	ui 		import Ui_code

import		cs_gdata.code, core.code, model.code

dCode	= None

class	CodeDialog(QtGui.QDialog, Ui_code.Ui_CodeDialog):
	def	__init__(self, email, password):
		"""
		Creates a GDataService and provides ClientLogin auth details to it.
		The email and password are required arguments for ClientLogin.
		The 'source' defined below is an arbitrary string, but should be used to reference your name or the name of your organization, the app name and version, with '-' between each of the three values.
		"""
		QtGui.QDialog.__init__(self)

		srcfeed = cs_gdata.code.BlogSpot()
		srcfeed.login(email, password)
		dstfeed = srcfeed.load()
		self.blogmodel = model.code.CodeModel(dstfeed)

		self.setupUi(self)
		#self.okButton.connect( self.okButton, QtCore.SIGNAL( "clicked()" ), self.slAccept )

		self.treeView.setSelectionMode(QtGui.QAbstractItemView.SingleSelection)
		self.treeView.setSelectionBehavior(QtGui.QAbstractItemView.SelectItems)
		self.treeView.setModel(self.blogmodel)
##		self.treeView.expandAll()
		self.treeView.setRootIsDecorated ( True )
		self.treeView.header().setStretchLastSection(True)
		self.treeView.resizeColumnToContents(1)

		self.treeView.connect(self.treeView, QtCore.SIGNAL("clicked(QModelIndex)"), self.showdetails)
		self.treeView.connect(self.treeView, QtCore.SIGNAL("activated(QModelIndex)"), self.showdetails)

	def	showdetails(self, index):
		feed = index.internalPointer()
		self.lineEditId.setText(feed.Id)
		if feed.Title:
			self.lineEditTitle.setText(QtCore.QString.fromUtf8(feed.Title))
		else:
			self.lineEditTitle.clear()
		self.dateTimeEditUpdated.setDateTime(feed.Updated)
		if (feed.Type >= core.blog.BLOG):		# Blog, Post, Comment
			self.dateTimeEditPublished.setEnabled(True)
			self.dateTimeEditPublished.setDateTime(feed.Published)
		else:
			self.dateTimeEditPublished.clear()
			self.dateTimeEditPublished.setEnabled(False)
		if (feed.Type == core.blog.POST):		# Post
			self.checkBoxDraft.setEnabled(True)
			self.checkBoxDraft.setCheckState(QtCore.Qt.Checked if feed.Draft else QtCore.Qt.Unchecked)
		else:
			self.checkBoxDraft.setCheckState(QtCore.Qt.PartiallyChecked)
			self.checkBoxDraft.setEnabled(False)
		if (feed.Type >= core.blog.POST):		# Post, Comment
			self.tbContent.setEnabled(True)
			if (feed.Content):
				self.tbContent.setText(QtCore.QString.fromUtf8(feed.Content))
			else:
				self.tbContent.clear()
		else:
			self.tbContent.clear()
			self.tbContent.setEnabled(False)
		self.twAuthor.setRowCount(len(feed.Author))
		for r, a in enumerate(feed.Author):
			self.twAuthor.setItem(r, 0, QtGui.QTableWidgetItem(a.Name))
