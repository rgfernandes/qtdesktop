# -*- coding: utf-8 -*-
'''
label: Private, Organization[ / Mobile, ...
So - need split label and out label of entry
'''

class	Organization:
	def	__init__(self):
		self.Name			= None	# str[]
		self.Title			= None	# str[]

class	Email:
	def	__init__(self):
		self.Address		= None
		self.Label			= None

class	Email:
	def	__init__(self):
		self.Proto			= None
		self.Address		= None
		self.Label			= None

class	_labeled:
	def	__init__(self):
		self.Value			= None
		self.Label			= None

class	Phone:
	def	__init__(self):
		self.No			= None
		self.Type			= None

class	Contact:
	def	__init__(self):
		self._label		= {}
		self.Id			= None	# int
		self.Content		= None	# comments
		self.Updated		= None	# datetime
		self.Title			= None	# FIO
		self.Organization	= None
		self.PrimaryEmail	= None	# Email link
		self.Email			= []		# labeled
		self.IM			= []		# labeled
		self.Phone			= []		# labeled
		self.Address		= []		# labeled
		self.Image			= None
