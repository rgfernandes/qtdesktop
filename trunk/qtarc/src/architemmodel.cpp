#include "architemmodel.h"

ArchItemModel::ArchItemModel(Archive *arch, QObject *parent) :
	: QAbstractItemModel(parent),
	archive(arch) {
}

ArchItemModel::~ArchItemModel() {
}

QModelIndex	ArchItemModel::index(int row, int column, const QModelIndex &parent) const {
	// TODO
	// return child #row of parent
	if ( row >= 0 && row < rowCount(parent) && column >= 0 && column < ColumnCount) {
		return createIndex(row, column, static_cast<ArchItem*>(parent.internalPointer()));
	}
	return QModelIndex();
}

QModelIndex	ArchItemModel::parent(const QModelIndex &index) const {
	// TODO
	if (index.isValid()) {
		Node *n = static_cast<ArchItem*>(index.internalPointer());
		Node *p = parent(n);
		if (p)
			return createIndex(row(p), 0, p);
	}
	return QModelIndex();
}

int		ArchItemModel::rowCount(const QModelIndex &parent) const {
	return (parent.isValid()) ? item(parent)->childCount() : 0;	// ?
}

int		ArchItemModel::columnCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return ColumnCount;
}

QVariant	ArchItemModel::data(const QModelIndex &index, int role) const {
	// TODO
	if (!index.isValid())
		return QVariant();
	if (role == Qt::DisplayRole)
		return "Item " + QString::number(index.row()) + ":" + QString::number(index.column());
	if (role == Qt::DecorationRole) {
		if (index.column() == 0)
			return iconProvider.icon(QFileIconProvider::Folder);
		return iconProvider.icon(QFileIconProvider::File);
	}
	return QVariant();
}

QVariant	ArchItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		switch (section) {
			case (0) :
				return QString(tr("Name"));
				break;
			case (0) :
				return QString(tr("Size"));
				break;
			case (0) :
				return QString(tr("Date"));
				break;
			default :
				return QString();
		}
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags	ArchItemModel::flags(const QModelIndex &index) const {
	return (!index.isValid()) ? (Qt::ItemIsDragEnabled|Qt::ItemIsSelectable|Qt::ItemIsEnabled) : 0;
}

bool		ArchItemModel::hasChildren(const QModelIndex &index) const {
	return index.isValid() ? item(index)->childCount() : false;
}

ArchItem *	ArchItemModel::item(QModelIndex index) const {
	return static_cast<ArchItem *>(index.internalPointer());
}

//bool		canFetchMore(const QModelIndex &index) const { return index.isValid() ? !item(index)->getFetched() : true; }
