#include <QProcess>
#include <QRegExp>

#include "archive.h"

typedef QMap<QString, ArchType> ArchiverMap;
ArchiverMap *archmap;

static struct { QString ext; ArchType type; } ArchiveExt[] = {
	{".tar",	TAR},
	{".tar.7z",	TAR},
	{".tar.gz",	TAR},
	{".tgz",	TAR},
	{".tar.bz",	TAR},
	{".tar.bz2",	TAR},
	{".tbz",	TAR},
	{".tbz2",	TAR},
	{".tar.lzma",	TAR},
	{".tlzma",	TAR},
	{".tar.xz",	TAR},
	{".txz",	TAR},
	{".7z",		LZMA},
	{".zip",	LZMA},
	{".arj",	ARJ},
	{".rar",	RAR},
};

/*struct {
	int	type;
	QString	archiver;
	QString	unarchiver;
	QString	list;
	QString add;
	QString extract;
	QString del;
	QString regex;
}*/

struct	FileInfo {
	bool		type;
	QString		path;
	long		size;
	QDateTime	datetime;
};

Archive::Archive( void ) : type(NONE), root(new ArchItem("", true)) {
	//currentItem = root;
}

Archive::~Archive() {
	delete root;
}

bool	Archive::load( const QString & fn ) {
	/*
	 * Load archive contents
	 * TODO:
	 * - handle exit status
	 */
	FileInfo fi;
	QProcess arch;

	if (fn.isEmpty())
		return false;
	archname = fn;
	QString fnl = fn.toLower();
	for (unsigned int i = 0; i < sizeof(ArchiveExt); i++)
		if (fnl.endsWith(ArchiveExt[i].ext)) {
			type = ArchiveExt[i].type;
			break;
		}
	if (type != LZMA)
		return false;
	// 7z start
	arch.start("7za", QStringList() << "l" << archname);
	// 7z end
	arch.waitForFinished();
	QString out = QString(arch.readAllStandardOutput());
	//QString err = QString(arch.readAllStandardError());
	root->getChildren()->clear();
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
	sort();
	return true;
}

void	Archive::sort( void ) {
	root->getChildren()->sort();
}

ArchItemPack	*Archive::List(void) {
	return root->getChildren();
}

bool	Archive::Add( QString * name ) {	// or update ?
	return Add(new QStringList(*name));
}

bool	Archive::Add( QStringList * names ) {	// or update ?
	// TODO: proceess errcode
	QProcess arch;
	if (!names->count())
		return false;
	// 7z start
	arch.start("7za", QStringList() << "a" << archname << *names);
	// 7z end
	arch.waitForFinished();
	return load(archname);
}

bool	Archive::Extract( QModelIndexList & selected, QString * dst ) {
	QProcess arch;
	if (selected.size()) {
		QStringList arglist;
		for (int i = 0; i < selected.size(); i++) {
			ArchItem *item = static_cast<ArchItem*>(selected.at(i).internalPointer());
			arglist << item->getFullPath();
		}
		//qDebug() << arglist;
		// 7z start
		arch.start("7za", QStringList() << "x" << (QString("-o") + *dst) << archname << arglist);
		// 7z end
		arch.waitForFinished();
	}
	return true;
}

bool	Archive::Delete( QModelIndexList & selected ) {
	QProcess arch;
	if (selected.size()) {
		QStringList arglist;
		for (int i = 0; i < selected.size(); i++) {
			ArchItem *item = static_cast<ArchItem*>(selected.at(i).internalPointer());
			arglist << item->getFullPath();
			ArchItem *parent = item->parent();
			parent->delChild(item);
		}
		// 7z start
		arch.start("7za", QStringList() << "d" << archname << arglist);
		// 7z end
		arch.waitForFinished();
		//sort();
	}
	return true;
}
