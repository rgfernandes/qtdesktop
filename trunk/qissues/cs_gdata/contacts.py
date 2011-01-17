# -*- coding: utf-8 -*-
'''
CS_GData.Contacts - Core to GData interface.
Contact-specific gdata:
    * gd:email
    * gd:im
    * gd:organization
    * gd:phoneNumber
    * gd:postalAddress

'''

import		sys, re
from	gdata	import service
import		gdata, atom, sys, pprint

class	Contacts:
	def	__init__(self):
		self.feed = None
##		self.dst = core.blog.BlogSpot()
##		self.BlogList = []

	def	login(self, email, password):
		self.service = service.GDataService(email, password)
		self.service.source = 'Contacts_Python_GooglePIM-1.0'
		self.service.service = 'cp'
		self.service.server = 'www.google.com'
		self.service.ProgrammaticLogin()
		self.email = email

	def	load(self):
		'''
		Get list of all blogs.
		@return core.blog
		'''
##		self.feed = self.service.Get('/m8/feeds/contacts/%s/base?max-results=190' % self.email)
		self.feed = self.service.Get('/m8/feeds/contacts/%s/base/30' % self.email)
		print self.feed
