/*
 * Abstract to handle archive engines.
 * Supported formats:
 * - 7z (7za)
 * - tar, tgz, tar.gz, tbzip2, tar.bz2 (tar)
 * - arj (arj/unarj)
 * - zip (zip/unzip, 7za)
 * - rar (rar/unrar)
 * - rpm (ro, rpm2cpio | cpio -t -v)
 * - cab (ro, cabextract)
 * - deb
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

#ifndef __ARCHIVE_H__
#define __ARCHIVE_H__

#include "architempack.h"

enum	ArchType { NONE, LZMA, TAR, ZIP, ARJ, RAR, CAB, ISO, RPM, DEB };

class	Archive {
public:
	Archive( const QString & );
	~Archive();
	ArchItemPack	*List(void);
	bool		Add( QString * );
	bool		Update( QString * );
	bool		Extract( QString * src, QString * dst );	// QDir
	bool		Delete( QString * );
	bool		Test(void);
	ArchItem *	getRoot(void) { return root; }
private:
	void		load();
	QString		archname;
	ArchType	type;
	ArchItem	*root, *currentItem;
};

#endif // __ARCHIVE_H__
