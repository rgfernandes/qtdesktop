#include <QDebug>

#include "architempack.h"

ArchItemPack::ArchItemPack() :
	dirs(new ArchItemMap),
	files(new ArchItemMap),
	list(new ArchItemList)
	{}

ArchItemPack::~ArchItemPack() {
	clear();
	delete dirs;
	delete files;
	delete list;
}

void		ArchItemPack::add(ArchItem * item) {
	if (item->getIsDir())
		dirs->insert(item->getName(), item);
	else
		files->insert(item->getName(), item);
}

void		ArchItemPack::del(ArchItem * item) {
	if (item->getIsDir())
		dirs->remove(item->getName());
	else
		files->remove(item->getName());
	list->removeAt(item->getRow());
	//qDebug() << item->getRow();
	for (int i = item->getRow(); i < list->count(); i++)
		list->at(i)->setRow(i);
	//delete item; FIXME: !!! - crash after this
}

void		ArchItemPack::clear(void) {
	// 1. clear and delete subs (dirs)
	QMapIterator <QString, ArchItem *> i(*dirs), j(*files);
	while (i.hasNext()) {
		i.next();
		i.value()->getChildren()->clear();
		delete i.value();
	}
	dirs->clear();
	// 2. clear and delete subs (files)
	while (j.hasNext()) {
		j.next();
		j.value()->getChildren()->clear();
		delete j.value();
	}
	files->clear();
	// 3. list
	list->clear();
}

int		ArchItemPack::count(void) {
	return dirs->size() + files->size();
}

bool		ArchItemPack::contains(QString name) {
	return (dirs->contains(name) || files->contains(name));
}

ArchItem *	ArchItemPack::get(QString & name) {
	return (dirs->contains(name)) ? dirs->value(name) : ((files->contains(name)) ? files->value(name) : 0);
}

ArchItem *	ArchItemPack::get(int no) {
	return ((no >=0) && (no < list->size())) ? list->at(no) : 0;
}

void		ArchItemPack::sort(void) {
	list->clear();
	list->append(dirs->values());
	list->append(files->values());
	for (int i = 0; i < list->size(); i++) {
		list->at(i)->setRow(i);
		list->at(i)->getChildren()->sort();
	}
}
