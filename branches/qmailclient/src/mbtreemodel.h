#ifndef __MBTREEMODEL_H__
#define __MBTREEMODEL_H__

// model to display mailbox->folder

#include <QAbstractItemModel>
#include <QList>
#include <QString>

class MBTreeItem {
public:
	MBTreeItem( int , QString p_sid, QString p_name );
	~MBTreeItem();
public:
	int getId() const;
	QString getSid() const;
	QString getName() const;
	int getParentId() const;
	int row() const;
protected:
	int id;
	QString sid;
	QString name;
private:
	MBTreeItem* parent;
	QList<MBTreeItem*> children;
	friend class MBTreeModel;
};

class MBTreeModel : public QAbstractItemModel {
	Q_OBJECT
public:
	MBTreeModel( QObject* parent = 0 );
	~MBTreeModel();

	int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	Qt::ItemFlags flags( const QModelIndex& index ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QModelIndex index( int row, int col, const QModelIndex& parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex& index ) const;
	//bool removeRows( int row, int count, const QModelIndex& parent = QModelIndex() );
	int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	// own methods
	void setRoot( MBTreeItem* node );
	MBTreeItem* categoryFromIndex( const QModelIndex& index ) const;
	void refreshModel();
private:
	void setupModel( MBTreeItem* parent );
	MBTreeItem* root;
};
#endif // __MBTREEMODEL_H__
