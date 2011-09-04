#include "mbtreemodel.h"

#include <QSqlQuery>
#include <QSqlError>

// item
MBTreeItem::MBTreeItem( int p_id, QString p_name, int p_qty ) :
	id(p_id), name(p_name), qty(p_qty), parent(0) {};

MBTreeItem::~MBTreeItem() {
	qDeleteAll( children );
};

int MBTreeItem::getId() const {
	return id;
}

QString MBTreeItem::getName() const {
	return name;
}

int MBTreeItem::getQty() const {
	return qty;
}

int MBTreeItem::getParentId() const {
	return parent ? parent->id : -1;
}

int MBTreeItem::row() const {
	return parent ? parent->children.indexOf( const_cast<MBTreeItem*>(this) ) : 0;
}

// model
MBTreeModel::MBTreeModel( QSqlDatabase *d, QObject* parent )
	: QAbstractItemModel( parent ), root(0), db(d) {
	root = new MBTreeItem(0, QString("Mailbox"));
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
	MBTreeItem* parentNode = itemFromIndex( parent );
	return createIndex( row, col, parentNode->children.value(row) );
}

MBTreeItem* MBTreeModel::itemFromIndex(const QModelIndex& index ) const {
	return index.isValid() ? static_cast<MBTreeItem*>(index.internalPointer()) : root;
}

int MBTreeModel::rowCount( const QModelIndex& parent ) const {
	return itemFromIndex( parent )->children.count();
}

int MBTreeModel::columnCount( const QModelIndex& /*parent*/ ) const {
	return 1;
}

QModelIndex MBTreeModel::parent( const QModelIndex& index ) const {
	MBTreeItem* node = itemFromIndex( index );
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
	MBTreeItem* node = itemFromIndex( index );
	if( !node )
		return QVariant();
	return QVariant( node->name );
}

QVariant MBTreeModel::headerData(int sec, Qt::Orientation orientation, int role ) const {
	if( orientation == Qt::Horizontal && role == Qt::DisplayRole && sec == 0 )
		return tr( "Mailbox" );
	return QVariant();
}

Qt::ItemFlags MBTreeModel::flags( const QModelIndex& index ) const {
	return !index.isValid() ? Qt::ItemIsEnabled : Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void MBTreeModel::refreshModel() {
	MBTreeItem* newRoot = new MBTreeItem(0, tr("Mailbox"), 0 );
	newRoot->parent = NULL;
	setupModel( newRoot );
	setRoot( newRoot );
}

void MBTreeModel::setupModel( MBTreeItem* root ) {
	if( !root || !root->children.isEmpty() )	// is NULL or has children
		return;
	if (!root->parent) {	// root
		QSqlQuery q(*db);
		if( q.prepare("SELECT id, name FROM account ORDER BY id")) {
			if( q.exec() ) {
				while( q.next() ) {
					MBTreeItem* account = new MBTreeItem(q.value(0).toInt(), q.value(1).toString(), 0);
					account->parent = root;
					root->children.append(account);
					// add subfolders
					QSqlQuery q1(*db);
					if( q1.prepare("SELECT id, parentid, name FROM folder WHERE accountid=:aid ORDER BY parentid, name;")) {
						q1.bindValue(":aid", account->getId());
						if( q1.exec() ) {
							QMap<int, MBTreeItem*> folders;
							MBTreeItem *parent;
							while( q1.next() ) {
								MBTreeItem* n = new MBTreeItem(q1.value(0).toInt(), q1.value(2).toString(), 0 );
								int parent_id = q1.value(1).toInt();
								if ((parent_id > 0) && (folders.contains(parent_id)))
									parent = folders.value(parent_id);
								else
									parent = account;
								n->parent = parent;
								parent->children.append(n);
								folders.insert(n->getId(), n);
							}
						}
					}
				}
			}
		}
	}
	return;
}
