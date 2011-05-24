#include "archengine.h"

#include <QDebug>
#include <QProcess>
#include <QRegExp>

Archive::Archive( const QString & fn ) :
	archname( fn ),
	curPath("/"),
	type(NONE)
{
	// 1. define arc type
	if (archname.toLower().endsWith(".7z"))
		type = LZMA;
	else if (
		archname.toLower().endsWith(".tar") ||
		archname.toLower().endsWith(".tgz") ||
		archname.toLower().endsWith(".tbz2") ||
		archname.toLower().endsWith(".tar.gz") ||
		archname.toLower().endsWith(".tar.bz2")
	)
		type = TAR;
	else if (archname.toLower().endsWith(".zip"))
		type = ZIP;
	else
		type = NONE;
}

/*
2011-05-24 12:12:10 ....A     24277504      5113141  1CV7.MD
 */
FileInfoList	*Archive::List( QString * dir) {
	QProcess arch;
	arch.start("7za", QStringList() << "l" << archname);
	arch.waitForFinished();
	QString out = QString(arch.readAllStandardOutput());
	QString err = QString(arch.readAllStandardError());
	//qDebug() << "Exit status:" << arch.exitStatus();
	//qDebug() << "StdErr:" << err;
	//qDebug() << "StdOut:" << out;
	//qDebug() << "splitted:" << out.split('\n');
	QStringList outlist = out.split('\n');
	QRegExp rx("^\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2} [D.][R.][H.][S.][A.] [ 0-9]{12} [ 0-9]{12}  .*$");
	QStringList::const_iterator ci;
	for (ci = outlist.constBegin(); ci != outlist.constEnd(); ++ci) {
		if (rx.indexIn(*ci) == 0) {
			//qDebug() << *ci;
			FileInfo *fi = new FileInfo (
				(*ci).mid(20, 5).startsWith('D'),
				(*ci).mid(53),
				(*ci).mid(26, 12).trimmed().toLong(),
				QDateTime::fromString((*ci).mid(0, 19), "yyyy-MM-dd HH:mm:ss")
			);
			filelist[(*ci).mid(53)] = fi; 
		}
	}
	return &filelist;
}
