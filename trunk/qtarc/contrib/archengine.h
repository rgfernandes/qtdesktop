/*
 * Abstract to handle archive engines.
 * Supported formats:
 * - 7z (7za)
 * - tar, tar.gz, tar.bz2 (tar)
 * - zip (zip/unzip)
 * - arj (arj/unarj)
 * - rar (rar/unrar)
 * - rpm (ro, rpm2cpio | cpio -t -v)
 * - cab (ro, cabextract)
 * - deb
 * - iso
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

#include <QFSFileEngine>
#include <QAbstractFileEngine>
#include <QAbstractFileEngineHandler>

class	ArchEngine : public QFSFileEngine
{
public:
	ArchEngine( const QString &);
};

class	ArchEngineHandler : public QAbstractFileEngineHandler
{
public:
	ArchEngine *create(const QString &) const;
};

/*
class	Archive {
public:
	Archive( QString & );
	List();
	Add( QString * src, QString * dst );
	Extract( QString * filename );
	Update( QString * filename );
	Delete( QString * filename );
private:
	QString archname;
}
*/
#endif // __ARCHENGINE_H__
