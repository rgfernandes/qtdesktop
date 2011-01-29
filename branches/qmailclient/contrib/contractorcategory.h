#include <QAbstractItemModel>
#include <QList>
#include <QString>
#include <QSqlQuery>

class ContractorCategory {
public:
    ContractorCategory( int p_id, QString p_sid, QString p_name )
	: id(p_id), sid(p_sid), name(p_name), parent(0) {};
    ~ContractorCategory() { qDeleteAll( children ); };

public:
    int getId() const { return id; }
    QString getSid() const { return sid; }
    QString getName() const { return name; }
    int getParentId() const { return parent ? parent->id : -1; }

    int row() const
    {
	return parent
	    ? parent->children.indexOf( const_cast<ContractorCategory*>(this) )
	    : 0;
    }
protected:
    int id;
    QString sid;
    QString name;
private:
    ContractorCategory* parent;
    QList<ContractorCategory*> children;

    friend class ContractorCategoryModel;
};

class QSqlQuery;

class ContractorCategoryModel : public QAbstractItemModel {
    Q_OBJECT
public:
    ContractorCategoryModel( QObject* parent = 0 );
    ~ContractorCategoryModel();

    int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    QVariant headerData( int section, Qt::Orientation orientation,
			 int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int col,
		       const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& index ) const;
    bool removeRows( int row, int count,
		     const QModelIndex& parent = QModelIndex() );
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    
    // собственные функции
    void setRoot( ContractorCategory* node );
    ContractorCategory* categoryFromIndex( const QModelIndex& index ) const;
    void refreshModel();
private:
    void setupModel( ContractorCategory* parent );
    ContractorCategory* root;
};