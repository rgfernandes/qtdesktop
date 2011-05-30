/*
 * Abstract to handle archive engines.
 * Supported formats:
 * - 7z (7za) (list: 7za t)
 * - tar, tgz, tar.gz, tbzip2, tar.bzip2, t7z/tar.7z (tar; list: tar -tvf)
 * - zip (zip/unzip, list: unzip -l)
 * - rar (unrar)
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

// helpers
// 1. archive type
enum	ArchType { TAR, LZMA, ZIP, RAR, NONE };

class	Archive {
public:
	Archive( const QString & );
	~Archive();
	ArchItemPack	*List(void);
	bool		Add( QString * );
	bool		Add( QStringList * );
	bool		Extract( QStringList *, QString * );
	bool		Delete( QStringList * );
	ArchItem *	getRoot(void) { return root; }
private:
	void		load();
	QString		archname;
	ArchType	type;
	ArchItem	*root, *currentItem;
};

#endif // __ARCHIVE_H__
