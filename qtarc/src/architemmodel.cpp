#include <QtCore>
#include "architemmodel.h"

ArchItemModel::ArchItemModel(Archive *arch, QObject *parent) :
	QAbstractItemModel(parent),
	archive(arch) {
}

ArchItemModel::~ArchItemModel() {
}

QModelIndex	ArchItemModel::index(int row, int column, const QModelIndex &index) const {
	//qDebug() << "index";
	if (!hasIndex(row, column, index))
		return QModelIndex();
	ArchItem *parentItem;
	if (!index.isValid())
		parentItem = archive->getRoot();
	else
		parentItem = static_cast<ArchItem*>(index.internalPointer());
	
	ArchItem *childItem = parentItem->getChild(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex	ArchItemModel::parent(const QModelIndex &index) const {
	//qDebug() << "parent";
	if (index.isValid()) {
		ArchItem *n = static_cast<ArchItem *>(index.internalPointer());
		ArchItem *p = n->parent();
		if (p != archive->getRoot())
			return createIndex(p->getRow(), 0, p);
	}
	return QModelIndex();
}

int		ArchItemModel::rowCount(const QModelIndex &index) const {
	//qDebug() << "Row count:";
	if (index.column() > 0)
		return 0;
	ArchItem *item;
	if (index.isValid())
		item = static_cast<ArchItem *>(index.internalPointer());
	else
		item = archive->getRoot();
	//qDebug() << item->childCount();
	return item->childCount();
}

int		ArchItemModel::columnCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return ColumnCount;
}

QVariant	ArchItemModel::data(const QModelIndex &index, int role) const {
	//qDebug() << "data";
	if (!index.isValid())
		return QVariant();
	ArchItem *item = static_cast<ArchItem *>(index.internalPointer());
	if (role == Qt::DisplayRole)
		switch(index.column()) {
			case (0):
				return item->getName();
				break;
			case (1):
				return QString::number(item->getSize());
				break;
			case (2):
				return item->getDateTime().toString("yyyy-MM-dd HH:mm:ss");
				break;
			default:
				return QVariant();
		}
	if (role == Qt::DecorationRole) {
		if (index.column() == 0)
			return item->isDir() ? iconProvider.icon(QFileIconProvider::Folder) : iconProvider.icon(QFileIconProvider::File);
	}
	return QVariant();
}

QVariant	ArchItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if ((role == Qt::DisplayRole) && (orientation == Qt::Horizontal)) {
		switch (section) {
			case (0) :
				return QString(tr("Name"));
				break;
			case (1) :
				return QString(tr("Size"));
				break;
			case (2) :
				return QString(tr("Date"));
				break;
			default :
				return QVariant();
		}
	}
	return QVariant();
}

Qt::ItemFlags	ArchItemModel::flags(const QModelIndex &index) const {
	//qDebug() << "flags";
	return index.isValid() ? (Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled) : Qt::NoItemFlags;
}
/*
bool		ArchItemModel::hasChildren(const QModelIndex &index) const {
	qDebug() << "hasChildren";
	if (index.isValid())
		qDebug() << static_cast<ArchItem *>(index.internalPointer())->childCount();
	return index.isValid() ? static_cast<ArchItem *>(index.internalPointer())->childCount() : false;
}
*/
ArchItem *	ArchItemModel::item(QModelIndex index) const {
	return static_cast<ArchItem *>(index.internalPointer());
}

//bool		canFetchMore(const QModelIndex &index) const { return index.isValid() ? !item(index)->getFetched() : true; }
