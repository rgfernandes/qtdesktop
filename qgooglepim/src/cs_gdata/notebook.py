# -*- coding: utf-8 -*-

import		sys, re
from	gdata	import service
import		gdata, atom, sys, pprint

class	Notebook:
	def	__init__(self):
		self.feed = None

	def	login(self, email, password):
		self.service = service.GDataService(email, password)
		self.service.source = 'Notebook_Python_GooglePIM-1.0'
		self.service.service = 'xapi'
		self.service.server = 'www.google.com'
		self.service.ProgrammaticLogin()
		self.email = email

	def	load(self):
		'''
		Get list of all blogs.
		@return core.blog
		'''
		self.feed = self.service.Get('/notebook/feeds/%s' % self.email)
		print self.feed
