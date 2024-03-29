#ifndef __ARCHITEMMODEL_H__
#define __ARCHITEMMODEL_H__

#include <QAbstractItemModel>
#include <QFileIconProvider>
#include "archive.h"

class ArchItemModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	/*enum Columns
	{
		NameColumn = 0,
		SizeColumn = 1,
		DateColumn = 2;
	};*/
	ArchItemModel(Archive *arch, QObject *parent = 0);
	~ArchItemModel();
	QModelIndex	index(int row, int column, const QModelIndex &parent = QModelIndex()) const;		// item index (virtual, mandatory)
	QModelIndex	parent(const QModelIndex &index) const;							// parent index (virtual, mandatory)
	int		rowCount(const QModelIndex &parent = QModelIndex()) const;				// (virtual, mandatory)
	int		columnCount(const QModelIndex &parent = QModelIndex()) const;				// (virtual, mandatory)
	QVariant	data(const QModelIndex &index, int role = Qt::DisplayRole) const;			// (virtual, mandatory)
	QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;	// column headers (virtual, mandatory)
	Qt::ItemFlags	flags(const QModelIndex &index) const;							// (virtual, mandatory)
	ArchItem *	item(QModelIndex index) const;								// item by index (UDF)
//	Archive *	getArchive( void ) const { return archive; }
//	ArchItem *	getRootItem() { return rootItem; }
	void		refresh() { emit layoutChanged(); }
private:
	Archive			*archive;
	static const int	ColumnCount = 3;
	QFileIconProvider	iconProvider;									// standart file/dir icons
};

#endif // __ARCHITEMMODEL_H__
