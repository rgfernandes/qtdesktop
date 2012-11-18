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

#include <QModelIndexList>
#include <QDebug>
#include "architem.h"

// helpers
// 1. archive type
enum	ArchType { LZMA, ARJ, RAR, ZIP, TAR, NONE };

class	Archive {
public:
	Archive( void );
	~Archive();
	bool		load( const QString & fn = QString() );
	void		sort( void );
	ArchItemPack	*List(void);
	bool		Add( QString * );
	bool		Add( QStringList * );
	bool		Extract( QModelIndexList &, QString * );
	bool		Delete( QModelIndexList & );
	ArchItem *	getRoot(void) { return root; }
private:
	QString		archname;
	ArchType	type;
	ArchItem	*root;	//, *currentItem;
};

#endif // __ARCHIVE_H__
