#include <QProcess>
#include <QRegExp>

#include "archive.h"

typedef QMap<QString, ArchType> ArchiverMap;
ArchiverMap *archmap;

static struct { QString ext; ArchType type; } ArchiveExt[] = {
	{".zip",	ZIP},
	{".7z",		LZMA},
	{".tar",	TAR},
	{".tar.gz",	TAR},
	{".tar.bz2",	TAR},
	{".tgz",	TAR},
	{".tbzip2",	TAR},
	{"rar",		RAR}
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

Archive::Archive( const QString & fn ) : archname( fn ) {
	/*
	 * fn - archive file name 
 	*/

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
	return true;
}

bool	Archive::Add( QStringList * names ) {	// or update ?
	return true;
}

bool	Archive::Extract(  QStringList * src, QString * dst ) {
	return true;
}

bool	Archive::Delete( QStringList * names ) {
	return true;
}