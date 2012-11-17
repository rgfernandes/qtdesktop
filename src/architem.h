#ifndef __ARCHITEM_H__
#define __ARCHITEM_H__

#include <QDateTime>
#include <QString>
#include <QStringList>

#include "architempack.h"

class	ArchItem {	// http://habrahabr.ru/blogs/qt_software/69658/
public:
	ArchItem(QString name, bool fileIsDir, ArchItem *parent = 0, long size = 0, QDateTime date = QDateTime::currentDateTime());
	~ArchItem();
	void		setSize(long what) { size = what; }
	void		setDateTime(QDateTime what) { date = what; }
	void		setIsDir(bool what) { fileIsDir = what; }
	void		setRow(int what) { row = what; }
	QString		getName() const { return name; };
	long		getSize() const { return size; }			// in bytes
	QDateTime	getDateTime() const { return date; }
	bool		getIsDir() const { return fileIsDir; }
	int		getRow() const { return row; }
	ArchItemPack *	getChildren() { return children; }
	ArchItem *	getChild(QString &);
	ArchItem *	getChild(int);
	void		addChild(ArchItem *item);
	void		addChildRecursive(QStringList &, bool, long, QDateTime);
	void		delChild(ArchItem *item);
	int		childCount() const { return children->count(); }
	ArchItem *	parent() { return itemParent; }
	QString		getFullPath(void);
private:
	void		setParent(ArchItem *parent) { itemParent = parent; }	// if parent is unknown
	QString		name;
	long		size;
	QDateTime	date;
	bool		fileIsDir;
	int		row;
	ArchItem	*itemParent;
	ArchItemPack	*children;
};

#endif // __ARCHITEM_H__
