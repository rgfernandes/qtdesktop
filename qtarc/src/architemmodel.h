#ifndef __ARCHITEMMODEL_H__
#define __ARCHITEMMODEL_H__

class ArchItemModel : public QAbstractItemModel
{
public:
	enum Columns
	{
		NameColumn = 0,
		SizeColumn,
		TypeColumn,
		DateColumn
	};
	ArchItemModel();
	~ArchItemModel();
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;				// (virtual, mandatory)
	Qt::ItemFlags flags(const QModelIndex &index) const;							// (virtual, mandatory)
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;	// column headers (virtual, mandatory)
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;		// item index (virtual, mandatory)
	QModelIndex parent(const QModelIndex &index) const;							// parent index (virtual, mandatory)
	int rowCount(const QModelIndex &parent = QModelIndex()) const;						// (virtual, mandatory)
	int columnCount(const QModelIndex &parent = QModelIndex()) const;					// (virtual, mandatory)
	ArchItem *item(QModelIndex index) const { return static_cast<ArchItem *>(index.internalPointer()); }	// item by index
	ArchItem *getRootItem() { return rootItem; }
	bool hasChildren(const QModelIndex &index) const { return index.isValid() ? item(index)->childCount() : true; }
	bool canFetchMore(const QModelIndex &index) const { return index.isValid() ? !item(index)->getFetched() : true; }
	void fetchMore(const QModelIndex &index);								// run when click on -/+ (virtual, mandatory)
	void refresh() { emit layoutChanged(); }
private:
	ArchItem *rootItem;
	static const int ColumnCount = 4;
	QFileIconProvider iconProvider;										// standart file/dir icons
protected:
	void readDir(ArchItem *item);										// load children
};

#endif // __ARCHITEMMODEL_H__
