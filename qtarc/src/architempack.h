#ifndef __ARCHITEMPACK_H__
#define __ARCHITEMPACK_H__

#include <QMap>
#include <QList>

class ArchItem;

typedef QMap<QString, ArchItem *> ArchItemMap;
typedef QList<ArchItem *> ArchItemList;

class	ArchItemPack {	// Class containing ArchItems
public:
	ArchItemPack();
	~ArchItemPack();
	void		add(ArchItem *);
	void		clear(void);
	int		count(void);
	bool		contains(QString);
	ArchItem *	get(QString &);
	ArchItem *	get(int);
	void		sort(void);
private:
	ArchItemMap	*dirs, *files;	// by name
	ArchItemList	*list;		// by order
};

#include "architem.h"

#endif // __ARCHITEMPACK_H__
