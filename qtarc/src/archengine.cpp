#include "archengine.h"

#include <QDebug>
#include <QProcess>
#include <QRegExp>

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
	if (item->isDir())
		dirs->insert(item->getName(), item);
	else
		files->insert(item->getName(), item);
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

// ----

struct	FileInfo {
	bool		type;
	QString		path;
	long		size;
	QDateTime	datetime;
};

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

// ----

Archive::Archive( const QString & fn ) : archname( fn ) {
	/*
	 * fn - archive file name 
 	*/
	// 1. define arc type
	QString fnl = fn.toLower();
	if (fnl.endsWith(".7z"))
		type = LZMA;
	else if (
		fnl.endsWith(".tar") ||
		fnl.endsWith(".tgz") ||
		fnl.endsWith(".tbz2") ||
		fnl.endsWith(".tar.gz") ||
		fnl.endsWith(".tar.bz2")
	)
		type = TAR;
	else if (fnl.endsWith(".zip"))
		type = ZIP;
	else
		type = NONE;
	root = new ArchItem("", true);
	currentItem = root;
	load();
}

Archive::~Archive() {
	delete root;
}

void	Archive::load(void) {
	/*
	 * Load archive contents
	 * TODO:
	 * - handle exit status
	 */
	FileInfo fi;

	if (archname.isEmpty())
		return;
	QProcess arch;
	// 7z start
	arch.start("7za", QStringList() << "l" << archname);
	// 7z end
	arch.waitForFinished();
	QString out = QString(arch.readAllStandardOutput());
	QString err = QString(arch.readAllStandardError());
	QStringList outlist = out.split('\n');
	// 7z start
	QRegExp rx("^\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2} [D.][R.][H.][S.][A.] [ 0-9]{12} [ 0-9]{12}  .*$");
	// 7z end
	QStringList::const_iterator ci;
	root->getChildren()->clear();
	for (ci = outlist.constBegin(); ci != outlist.constEnd(); ++ci) {
		if (rx.indexIn(*ci) == 0) {
	// 7z start
			fi.type =	(*ci).mid(20, 5).startsWith('D');
			fi.path =	(*ci).mid(53);
			fi.size =	(*ci).mid(26, 12).trimmed().toLong();
			fi.datetime =	QDateTime::fromString((*ci).mid(0, 19), "yyyy-MM-dd HH:mm:ss");
	// 7z end
			QStringList pathlist = fi.path.split('/');
			root->addChildRecursive(pathlist, fi.type, fi.size, fi.datetime);
		}
	}
	root->getChildren()->sort();	// !
}

ArchItemPack	*Archive::List(void) {
	return currentItem->getChildren();
}

bool	Archive::Add( QString * name ) {	// or update ?
	// a
	;
}

bool	Archive::Update( QString * name ) {
	// a
	;
}

bool	Archive::Test(void) {
	// t key
	;
}

