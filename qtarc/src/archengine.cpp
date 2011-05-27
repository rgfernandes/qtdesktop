#include "archengine.h"

#include <QDebug>
#include <QProcess>
#include <QRegExp>

struct	FileInfo {
	bool		type;
	QString		path;
	long		size;
	QDateTime	datetime;
};

ArchItem::ArchItem(QString name, bool fileIsDir, ArchItem *parent, long size, QDateTime date)
{
	this->name = name;
	this->fileIsDir = fileIsDir;
	this->size = size;
	this->date = date;
	this->itemParent = parent;
	this->children = new ArchItemMap();
}

ArchItem *	ArchItem::findChild(QString &name) {
	return this->children->contains(name) ? this->children->value(name) : 0;
}

void	ArchItem::addChild(ArchItem *item) {
	if (!this->children->contains(item->getName()))
		this->children->insert(item->getName(), item);
}

void	ArchItem::addChildRecursive(QStringList &filePath, bool fileIsDir, long size, QDateTime date) {
	/* add children recursively
	 *
	 */
	 QString s = filePath.at(0);
	 if (filePath.size() == 1) {				 	// last item
	 	if (this->children->contains(s)) {			// already exists
	 		this->children->value(s)->setIsDir(fileIsDir);
	 		this->children->value(s)->setSize(size);
	 		this->children->value(s)->setDateTime(date);
 		} else {
 			this->addChild(new ArchItem(s, fileIsDir, this, size, date));
		}
 	} else {							// down
 		this->addChild(new ArchItem(s, true, this));		// create sub
 		filePath.removeFirst();
 		this->findChild(s)->addChildRecursive(filePath, fileIsDir, size, date);
	}
}

Archive::Archive( const QString & fn ) : archname( fn )
{
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
}

ArchItemMap	*Archive::List() {
	return currentItem->getChildren();
}
