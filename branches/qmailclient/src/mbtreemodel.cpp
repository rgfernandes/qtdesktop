#include "mbtreemodel.h"

#include <QSqlQuery>
#include <QSqlError>

// item
MBTreeItem::MBTreeItem( int p_id, QString p_sid, QString p_name ) : id(p_id), sid(p_sid), name(p_name), parent(0) {};
MBTreeItem::~MBTreeItem() { qDeleteAll( children ); };
int MBTreeItem::getId() const { return id; }
QString MBTreeItem::getSid() const { return sid; }
QString MBTreeItem::getName() const { return name; }
int MBTreeItem::getParentId() const { return parent ? parent->id : -1; }
int MBTreeItem::row() const { return parent ? parent->children.indexOf( const_cast<MBTreeItem*>(this) ) : 0; }

// model
MBTreeModel::MBTreeModel( QObject* parent )
	: QAbstractItemModel( parent ), root(0) {
	root = new MBTreeItem(0, QString("ROOT"), tr("Category") );
	root->parent = NULL;
	setupModel( root );
}

MBTreeModel::~MBTreeModel() {
	delete root;
}

void MBTreeModel::setRoot( MBTreeItem* node ) {
	delete root;
	root = node;
	reset();
}

QModelIndex MBTreeModel::index(int row, int col, const QModelIndex& parent ) const {
	MBTreeItem* parentNode = categoryFromIndex( parent );
	return createIndex( row, col, parentNode->children.value(row) );
}

MBTreeItem* MBTreeModel::categoryFromIndex(const QModelIndex& index ) const {
	return index.isValid() ? static_cast<MBTreeItem*>(index.internalPointer()) : root;
}

int MBTreeModel::rowCount( const QModelIndex& parent ) const {
	return categoryFromIndex( parent )->children.count();
}

int MBTreeModel::columnCount( const QModelIndex& /*parent*/ ) const {
	return 1;
}

QModelIndex MBTreeModel::parent( const QModelIndex& index ) const
{
	MBTreeItem* node = categoryFromIndex( index );
	if( !node )
		return QModelIndex();
	MBTreeItem* parentNode = node->parent;
	if( !parentNode )
		return QModelIndex();
	return parentNode == root ? QModelIndex() : createIndex( parentNode->row(), 0, parentNode );	
}

QVariant MBTreeModel::data(const QModelIndex& index, int role ) const {
	if( role != Qt::DisplayRole || index.column() > 0 )
		return QVariant();
	MBTreeItem* node = categoryFromIndex( index );
	if( !node )
		return QVariant();
	return QVariant( node->name );
}

QVariant MBTreeModel::headerData(int sec, Qt::Orientation orientation, int role ) const {
	if( orientation == Qt::Horizontal && role == Qt::DisplayRole && sec == 0 )
		return tr( "Category" );
	return QVariant();
}

Qt::ItemFlags MBTreeModel::flags( const QModelIndex& index ) const {
	return !index.isValid() ? Qt::ItemIsEnabled : Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void MBTreeModel::refreshModel() {
	MBTreeItem* newRoot = new MBTreeItem(0, QString(), tr("Category") );
	newRoot->parent = NULL;
	setupModel( newRoot );
	setRoot( newRoot );
}

void MBTreeModel::setupModel( MBTreeItem* parent ) {
	if( !parent || !parent->children.isEmpty() )
		return;
	QSqlQuery q( QSqlDatabase::database("main") );
	if( q.prepare("select * from table( contractor_tree.GetCategoryTree(:pid,:depth) )")) {
		q.bindValue( ":pid", parent->getId() );
		q.bindValue( ":depth", 9999 );
		if( q.exec() ) {
			QList<MBTreeItem*> parents;
			QList<int> levels;
			parents << parent;
			levels << 1;

			while( q.next() ) {
				int level = q.value(0).toInt();
				if( level > levels.last() ) {
					if( parents.last()->children.count() > 0 ) {
						parents << parents.last()->children.value(parents.last()->children.count()-1 );
						levels << level;
					}
				} else {
					while( level < levels.last() && parents.count() > 0 ) {
						parents.pop_back();
						levels.pop_back();
					}
				}
				MBTreeItem* n = new MBTreeItem(q.value(1).toInt(), q.value(3).toString(), q.value(4).toString() );
				n->parent = parents.last();
				parents.last()->children.append(n);
			}
			return;
		}
	}
	//throw DatabaseException( q.lastError() );
}
