#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
CS_GData.Blog - Core to GData interface.
Blogs:	/feeds/default/blogs
Posts:	/feeds/<blog_id>/posts/default
Comments:	/feeds/<blog_id>/<post_id>/comments/default'
'''

import		sys, re
from datetime	import datetime, tzinfo
from time		import strptime
from	gdata	import service
import		gdata, atom, sys, pprint
import		core.blog

def	__s2d(s):
	a = re.split("^(\d+)-(\d+)-(\d+)T(\d+):(\d+):(\d+).(\d+)", s)[1:]
	return datetime(int(a[0]), int(a[1]), int(a[2]), int(a[3]), int(a[4]), int(a[5]), int(a[6]), tzinfo(a[7]))

def	_load_parent0 ( src, dst, parent = None ) :
	dst.Parent		= parent
	dst.Id		= src.id.text
##	dst.Updated	= src.updated.text
	dst.Updated	= __s2d(src.updated.text)
	dst.Title		= src.title.text
	for a in src.author:
		author = core.blog.Person()
		author.Name = a.name.text
		dst.Author.append(author)

def	_load_parent1 ( src, dst, parent ) :
	_load_parent0 ( src, dst, parent )
	dst.Published	= __s2d(src.published.text)

def	_load_parent2 ( src, dst, parent ) :
	_load_parent1 ( src, dst, parent )
	dst.Content	= src.content.text

def	_load_Comment ( src, dst, parent ) :
	_load_parent2 ( src, dst, parent )
	dst.Id		= re.split("^tag:blogger\.com,1999\:blog-(\d+)\.post-(\d+)", dst.Id)[2]

def	_load_Post ( src, dst, parent ) :
	_load_parent2 ( src, dst, parent )
	dst.Id		= re.split("^tag:blogger\.com,1999\:blog-(\d+)\.post-(\d+)", dst.Id)[2]
	if (src.control):
		dst.Draft		= (src.control.draft.text == 'yes')

def	_load_Blog ( src, dst, parent ) :
	_load_parent1 ( src, dst, parent )
	dst.Id		= re.split("^tag:blogger\.com,1999:user-(\d+)\.blog-(\d+)", dst.Id)[2]
	dst.Summary	= src.summary.text

def	_load_BlogSpot ( src, dst ) :
	_load_parent0 ( src, dst )
	dst.Id		= re.split("^tag:blogger\.com,1999:user-(\d+)\.blogs", dst.Id)[1]

class	BlogSpot:
	def	__init__(self):
		self.feed = None
		self.dst = core.blog.BlogSpot()
		self.BlogList = []

	def	login(self, email, password):
		self.service = service.GDataService(email, password)
		self.service.source = 'Blogger_Python_GooglePIM-1.0'
		self.service.service = 'blogger'
		self.service.server = 'www.blogger.com'
		self.service.ProgrammaticLogin()

	def	load(self):
		'''
		Get list of all blogs.
		@return core.blog
		'''
		# 1. blogspot
		self.feed0 = self.service.Get('/feeds/default/blogs')
		_load_BlogSpot(self.feed0, self.dst)
		# 2. blogs
		for b in self.feed0.entry:
			blog = core.blog.Blog(self.dst)
			_load_Blog(b, blog, self.dst)
		# 3. posts
			self.feed1 = self.service.Get('/feeds/' + blog.Id + '/posts/default')
			for p in self.feed1.entry:
				post = core.blog.Post(blog)
				_load_Post(p, post, blog)
		# 4. comments
				try:
					self.feed2 = self.service.Get('/feeds/' + blog.Id + '/' + post.Id + '/comments/default')
					ok = True
				except:
					ok = False
##				if (ok):
##					for c in self.feed2.entry:
##						comment = core.blog.Comment(post)
##						_load_Comment(c, comment, post)
##						post.Entry.append(comment)
				blog.Entry.append(post)
				break
			self.dst.Entry.append(blog)
		return self.dst

	def	CreateComment(self, post_id, comment_text):
		# Build the comment feed URI
		feed_uri = '/feeds/' + self.blog_id + '/' + post_id + '/comments/default'

		# Create a new entry for the comment and submit it to the GDataService
		entry = gdata.GDataEntry()
		entry.content = atom.Content(content_type='xhtml', text=comment_text)
		return self.service.Post(entry, feed_uri)

	def	DeleteComment(self, post_id, comment_id):
		feed_uri = '/feeds/' + self.blog_id + '/' + post_id + '/comments/default/' + comment_id
		self.service.Delete(feed_uri)

	def	CreatePost(self, title, content, author_name, is_draft):
		# Create the entry to insert.
		entry = gdata.GDataEntry()
		entry.author.append(atom.Author(atom.Name(text=author_name)))
		entry.title = atom.Title(title_type='xhtml', text=title)
		entry.content = atom.Content(content_type='html', text=content)
		if is_draft:
			control = atom.Control()
			control.draft = atom.Draft(text='yes')
			entry.control = control
		return self.service.Post(entry, '/feeds/' + self.blog_id + '/posts/default')	# Ask the service to insert the new entry.

	def	UpdatePostTitle(self, entry_to_update, new_title):
		entry_to_update.title = atom.Title('xhtml', new_title)	# Set the new title in the Entry object
		edit_uri = entry_to_update.GetEditLink().href  			# Grab the edit URI
		return self.service.Put(entry_to_update, edit_uri)

	def	DeletePost(self, edit_link_href):
		self.service.Delete(edit_link_href)
