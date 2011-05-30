#include "architem.h"

ArchItem::ArchItem(QString name, bool fileIsDir, ArchItem *parent, long size, QDateTime date) {
	this->name = name;
	this->fileIsDir = fileIsDir;
	this->size = size;
	this->date = date;
	this->row = 0;
	this->itemParent = parent;
	this->children = new ArchItemPack();
}

ArchItem::~ArchItem() {
	delete children;
}

ArchItem *	ArchItem::getChild(QString &name) {
	return this->children->get(name);
}

ArchItem *	ArchItem::getChild(int no) {
	return this->children->get(no);
}

void	ArchItem::addChild(ArchItem *item) {
	if (!this->children->contains(item->getName()))
		this->children->add(item);
}

void	ArchItem::addChildRecursive(QStringList &filePath, bool fileIsDir, long size, QDateTime date) {
	/* add children recursively
	 *
	 */
	 QString s = filePath.at(0);
	 if (filePath.size() == 1) {				 	// last item
	 	if (this->children->contains(s)) {			// already exists
	 		ArchItem *item = this->getChild(s);
	 		item->setIsDir(fileIsDir);
	 		item->setSize(size);
	 		item->setDateTime(date);
 		} else {
 			this->addChild(new ArchItem(s, fileIsDir, this, size, date));
		}
 	} else {							// down
 		this->addChild(new ArchItem(s, true, this));		// create sub
 		filePath.removeFirst();
 		this->getChild(s)->addChildRecursive(filePath, fileIsDir, size, date);
	}
}

QString	ArchItem::getFullPath(void) {
	if (itemParent)
		return itemParent->getFullPath() + "/" + name;
	return name;
}