#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "contractorcategorymodel.h"
#include "dbexception.h"

ContractorCategoryModel::ContractorCategoryModel( QObject* parent )
    : QAbstractItemModel( parent ), root(0)
{
    root = new ContractorCategory(
	0, QString("ROOT"), tr("Category") );
    root->parent = NULL;
    setupModel( root );
}

ContractorCategoryModel::~ContractorCategoryModel()
{
    delete root;
}

void ContractorCategoryModel::setRoot( ContractorCategory* node )
{
    delete root;
    root = node;
    reset();
}

QModelIndex ContractorCategoryModel::index(
    int row, int col, const QModelIndex& parent ) const
{
    ContractorCategory* parentNode = categoryFromIndex( parent );
    return createIndex( row, col, parentNode->children.value(row) );
}

ContractorCategory* ContractorCategoryModel::categoryFromIndex(
    const QModelIndex& index ) const
{
    return index.isValid()
	? static_cast<ContractorCategory*>(index.internalPointer())
	: root;
}

int ContractorCategoryModel::rowCount( const QModelIndex& parent ) const
{
    return categoryFromIndex( parent )->children.count();
}

int ContractorCategoryModel::columnCount( const QModelIndex& /*parent*/ ) const
{
    return 1;
}

QModelIndex ContractorCategoryModel::parent( const QModelIndex& index ) const
{
    ContractorCategory* node = categoryFromIndex( index );
    if( !node )
	return QModelIndex();
    ContractorCategory* parentNode = node->parent;
    if( !parentNode )
	return QModelIndex();
    return parentNode == root
	? QModelIndex()
	: createIndex( parentNode->row(), 0, parentNode );	
}

QVariant ContractorCategoryModel::data(
    const QModelIndex& index, int role ) const
{
    if( role != Qt::DisplayRole || index.column() > 0 )
	return QVariant();
    ContractorCategory* node = categoryFromIndex( index );
    if( !node )
	return QVariant();
    return QVariant( node->name );
}

QVariant ContractorCategoryModel::headerData(
    int sec, Qt::Orientation orientation, int role ) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole && sec == 0 )
	return tr( "Category" );
    return QVariant();
}

Qt::ItemFlags ContractorCategoryModel::flags( const QModelIndex& index ) const
{
    return !index.isValid() 
	? Qt::ItemIsEnabled
	: Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void ContractorCategoryModel::setupModel( ContractorCategory* parent )
{
    if( !parent || !parent->children.isEmpty() )
	return;
    QSqlQuery q( QSqlDatabase::database("main") );
    if( q.prepare(
	"select * from table( contractor_tree.GetCategoryTree(:pid,:depth) )"))
    {
	q.bindValue( ":pid", parent->getId() );
	q.bindValue( ":depth", 9999 );
	if( q.exec() ) {
	    QList<ContractorCategory*> parents;
	    QList<int> levels;
	    parents << parent;
	    levels << 1;

	    while( q.next() ) {
		int level = q.value(0).toInt();
		if( level > levels.last() ) {
		    if( parents.last()->children.count() > 0 ) {
			parents << parents.last()->children.value(
			    parents.last()->children.count()-1 );
			levels << level;
		    }
		} else {
		    while( level < levels.last() && parents.count() > 0 ) {
			parents.pop_back();
			levels.pop_back();
		    }
		}
		ContractorCategory* n = new ContractorCategory(
		    q.value(1).toInt(), q.value(3).toString(),
		    q.value(4).toString() );
		n->parent = parents.last();
		parents.last()->children.append(n);
	    }
	    return;
	}
    }
    throw DatabaseException( q.lastError() );
}

void ContractorCategoryModel::refreshModel()
{
    ContractorCategory* newRoot = new ContractorCategory(
	0, QString(), tr("Category") );
    newRoot->parent = NULL;
    setupModel( newRoot );
    setRoot( newRoot );
}