exists = set()
skip = set()

def	chk_dive(src, dst):
	'''
	@param src: src folder
	@param dst: ArchItemFolder
	@return:
	  - 0 - free
	  - 1 - exists
	  - 2 - all exists
	  - 3 - conflict
	'''
	src_children = os.listdir(src)
	if src_children == 0:
		add src to skip
		return 0
	if dst.children == 0:
		return 0
	exists = set()
	for s in src_children:
		d = dst.child(s)
		if not d:
			continue
		if s == file:
			if d == folder:
				store s
				return 3
			exists += s
		else:	# s is folder
			if d == file:
				store s
				return 3
			retvalue = chk_dive(s, d)
			if 0: pass
			if 1: 
			if 2: exists += s
			if 3: return 3
	if exists == 0:
		return 0
	if exists == src_children:
		return 2
