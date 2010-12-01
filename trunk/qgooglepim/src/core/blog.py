# -*- coding: utf-8 -*-
'''
Core.Blog - _Caching_ blog class.
Classes: Blogger = blog[= post[= comment[]]]
'''

import	pprint
#import	core.feed

BLOGSPOT	= 1
BLOG		= 2
POST		= 3
COMMENT	= 4

def	_print(arr, shift = 0):
	'''
	'''
	for a in arr:
		if (a[1]):
			try:
				print "%s%s:\t%s" % ("-" * shift, a[0], a[1])
			except:
				print "%s%s:\t%s" % ("-" * shift, a[0], a[1].encode('utf-8'))

def	_printList(title, subtitle, lst, shift = 0):
	'''
	'''
	if (len(lst)):
		print "%s%s:" % ("-" * shift, title)
		for i, v in enumerate(lst):
			print "%s%s #%d:" % ("-" * (shift + 1), subtitle, i)
			v._print(shift + 2)

# ===

class	Person:
	def	__init__(self):
		self.Name		= None	# str
		self.Email		= None	# str
		self.Uri		= None	# Uri

	def	_print(self, shift):
		_print((
			("Name",	self.Name),
			("Email",	self.Email),
			("Uri",	self.Uri)
		), shift)

class	_parent0:
	'''
	Common parent class for all others.
	'''
	def	__init__ ( self, parent ) :
		self.Type		= None
		self.Parent	= parent
		self.Id		= None	# int
		self.Updated	= None	# datetime
		self.Title		= None	# str
		self.Author	= []		# Person[]
		self.Entry		= []

	def	_print ( self, shift = 0) :
		_print ( (
			( "Id",		self.Id),
			( "Updated",	self.Updated),
			( "Title",		self.Title)
		), shift )
		_printList ( "Authors", "Author", self.Author, shift )

	def	child(self, row):
		return self.Entry[row]

	def	childCount(self):
		return len(self.Entry)

	def	parent(self):
		return self.Parent

	def	row(self):
		if self.Parent:
			return self.Parent.Entry.index(self)
		return 0

class	_parent1 ( _parent0 ) :
	'''
	Common parent class for blog, post and comment.
	'''
	def	__init__ ( self, parent ) :
		_parent0.__init__ ( self, parent )
		self.Published	= None	# datetime

	def	_print ( self, shift = 0 ) :
		_parent0._print ( self, shift )
		_print ( (
			( "Published",	self.Published ),
		), shift )

class	_parent2 ( _parent1 ) :
	'''
	Common parent class for blog, post and comment.
	'''
	def	__init__ ( self, parent ) :
		_parent1.__init__ ( self, parent )
		self.Content	= None	# str

	def	_print ( self, shift = 0 ) :
		_parent1._print ( self, shift )
		_print ( (
			( "Content",	self.Content ),
		), shift )

# ---

class	Comment ( _parent2 ) :
	'''
	'''
	def	__init__ ( self, parent ) :
		_parent2.__init__ ( self, parent )
		self.Type		= COMMENT
##		self.Entry	= None

class	Post ( _parent2 ) :
	'''
	'''
	def	__init__ ( self, parent ) :
		_parent2.__init__ ( self, parent )
		self.Type		= POST
		self.Draft		= None	# bool

	def	_print ( self, shift = 0 ) :
		_parent2._print ( self, shift )
		_print ( (
			( "Draft",		self.Draft ),
		), shift )
		_printList( "Comments", "Comment", self.Entry, shift )

class	Blog ( _parent1 ) :
	'''
	'''
	def	__init__ ( self, parent ) :
		_parent1.__init__ ( self, parent )
		self.Type		= BLOG

	def	_print ( self, shift = 0 ) :
		_parent1._print ( self, shift )
		_printList("Posts", "Post", self.Entry, shift)

class	BlogSpot ( _parent0 ) :
	'''
	'''
	def	__init__ ( self, parent = None ) :
		_parent0.__init__ ( self, parent )
		self.Type		= BLOGSPOT

	def	_print ( self, shift = 0 ) :
		_parent0._print ( self, shift )
		_printList("Blogs", "Blog", self.Entry, shift)
