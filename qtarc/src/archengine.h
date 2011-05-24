/*
 * Abstract to handle archive engines.
 * Supported formats:
 * - 7z (7za)
 * - tar, tar.gz, tar.bz2 (tar, 7za) - or: tar, gzip, gz, bz2, bzip2
 * - arj (arj/unarj)
 * - zip (zip/unzip, 7za)
 * - rar (rar/unrar)
 * - rpm (ro, rpm2cpio | cpio -t -v)
 * - cab (ro, cabextract)
 * - deb
 * - iso (7za)
 * 
 * Supported actions:
 * - list
 * - copyout
 * - copyin
 * - moveout
 * - movein
 * - mkdir
 * - rm
 * - rmdir
 * 
 * List columns:
 * - type (dir/file)
 * - name
 * - size
 * - datetime modified
 * - attributes: DRHSA
 * 
 * To use:
 * - QFileInfoList
 * 
 * To reimplement:
 * - fileFlags
 * - entryList
 */

#ifndef __ARCHENGINE_H__
#define __ARCHENGINE_H__

#include <QString>
#include <QDateTime>
#include <QMap>

class	FileInfo {
public:
	FileInfo() {};
	FileInfo(bool t, QString n, long s, QDateTime dt) :
		type(t), name(n), size(s), datetime(dt) {};
	bool		getType()	{ return type; };
	QString		*getName()	{ return &name; };
	long		getSize()	{ return size; };
	QDateTime	getDateTime()	{ return datetime; };
private:
	bool		type;
	QString		name;
	long		size;
	QDateTime	datetime;
};

typedef	QMap<QString, FileInfo *> FileInfoList;

enum	ArchType {
	NONE,
	LZMA,
	TAR,
	ZIP,
	ARJ,
	RAR,
	CAB,
	ISO,
	RPM,
	DEB
};

class	Archive {
public:
	Archive( const QString & );
	FileInfoList	*List( QString * dir = 0);
	bool		Add( QString * src, QString * dst );
	bool		Extract( QString * src, QString * dst );	// QDir
	bool		Update( QString * src, QString * dst );
	bool		Delete( QString * filename );

private:
	QString		archname, curPath;
	FileInfoList	filelist;
	ArchType	type;
};

#endif // __ARCHENGINE_H__
