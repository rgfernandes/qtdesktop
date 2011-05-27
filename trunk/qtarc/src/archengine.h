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

class	ArchItem;

typedef QMap<QString, ArchItem *> ArchItemMap;
typedef QList<ArchItem *> ArchItemList;

class	ArchItemPack {
	/*
	 * Class containing ArchItems
	 */
public:
	ArchItemPack();
	~ArchItemPack();
/''	void	sort();
private:
	ArchItemMap	dirs, files;	// by name
	ArchItemList	list;		// by order
};

class	ArchItem {	// http://habrahabr.ru/blogs/qt_software/69658/
public:
	ArchItem(QString name, bool fileIsDir, ArchItem *parent = 0, long size = 0, QDateTime date = QDateTime::currentDateTime());
	void		setSize(long what) { size = what; }
	void		setDateTime(QDateTime what) { date = what; }
	void		setIsDir(bool what) { fileIsDir = what; }
//	void		setChildren(ArchItemMap *what);
	QString		getName() const { return name; };
	long		getSize() const { return size; }			// in bytes
	QDateTime	getDateTime() const { return date; }
	bool		isDir() const { return fileIsDir; }
	ArchItemMap *	getChildren() { return children; }
	ArchItem *	findChild(QString &);
	void		addChild(ArchItem *item);
	void		addChildRecursive(QStringList &, bool, long, QDateTime);
	int		childCount() const { return children->count(); }
//	int		row() const;						// self no in parent's list
	ArchItem *	parent() { return itemParent; }
private:
	void		setParent(ArchItem *parent) { itemParent = parent; }	// if parent is unknown
	QString		name;
	long		size;
	QDateTime	date;
	bool		fileIsDir;
	ArchItem	*itemParent;
	ArchItemMap	*children;
};

enum	ArchType { NONE, LZMA, TAR, ZIP, ARJ, RAR, CAB, ISO, RPM, DEB };

class	Archive {
public:
	Archive( const QString & );
	ArchItemMap	*List(void);
	bool		Add( QString * src, QString * dst );
	bool		Extract( QString * src, QString * dst );	// QDir
	bool		Update( QString * src, QString * dst );
	bool		Delete( QString * filename );
private:
	void		load();
	QString		archname;
	ArchType	type;
	ArchItem	*root, *currentItem;
};

#endif // __ARCHENGINE_H__