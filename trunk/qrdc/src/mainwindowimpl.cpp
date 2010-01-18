#include "mainwindowimpl.h"
#include <QtGui>
#include <QtSql>
//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f) {
	setupUi(this);
	QActionGroup *viewGroup = new QActionGroup(this);
	viewGroup->addAction(actionConnections);
	viewGroup->addAction(actionHosts);
	viewGroup->addAction(actionVariables);
	viewGroup->addAction(actionProtocols);
	actionConnections->setChecked(true);
	currentList = CONNECTION;
	setSlots();
}

void	MainWindowImpl::setSlots(void) {
	connect( actionExit,		SIGNAL( triggered() ),	this, SLOT( onActionExit() ) );
	connect( actionConnections,	SIGNAL( triggered() ),	this, SLOT( onActionConnections() ) );
	connect( actionHosts,		SIGNAL( triggered() ),	this, SLOT( onActionHosts() ) );
	connect( actionVariables,	SIGNAL( triggered() ),	this, SLOT( onActionVariables() ) );
	connect( actionProtocols,	SIGNAL( triggered() ),	this, SLOT( onActionProtocols() ) );
	connect( actionAdd,		SIGNAL( triggered() ),	this, SLOT( onActionAdd() ) );
	connect( actionEdit,		SIGNAL( triggered() ),	this, SLOT( onActionEdit() ) );
	connect( actionDel,		SIGNAL( triggered() ),	this, SLOT( onActionDel() ) );
	connect( actionOpen,		SIGNAL( triggered() ),	this, SLOT( onActionOpen() ) );
	connect( actionAbout,		SIGNAL( triggered() ),	this, SLOT( onActionAbout() ) );
	connect( actionAboutQt,		SIGNAL( triggered() ),	this, SLOT( onActionAboutQt() ) );
}

bool	MainWindowImpl::loadSql(QStringList &list) {
	QFile file("sql/data.sql");
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, tr("Error"), tr("fail to open data.sql to read"));
		return false;
	} else {
		list.append(QString(file.readAll()));
		file.close();
	}
	return true;
}

bool	MainWindowImpl::createConnection(void)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "qrdc.db");
	if (!db.open())
	{
		QMessageBox::critical(this,
			tr("Opening database error"),
			tr("Unable to establish a database connection."),
			QMessageBox::Cancel, QMessageBox::NoButton);
		return false;
	}
	else
	{
		if (db.tables().isEmpty())
		{
			QStringList list;
			loadSql(list);
			if (!list.isEmpty()) {
				QSqlQuery q;
				QListIterator<QString> itr (list);  
				while (itr.hasNext()) {
					QString current = itr.next();
					qDebug() << "{" <<  current << "}";
				}
			} else {
				QMessageBox::critical(this,
				tr("Opening database error"),
				tr("Unable to read sql."));

			}
		}
	}
	return true;
}

void	MainWindowImpl::setModels(void)
{
	createConnection();
}

void	MainWindowImpl::onActionExit(void)
{
	close();
}
void	MainWindowImpl::onActionConnections(void)
{
	
}
void	MainWindowImpl::onActionHosts(void)
{
	
}
void	MainWindowImpl::onActionVariables(void)
{
	
}
void	MainWindowImpl::onActionProtocols(void)
{
	
}
void	MainWindowImpl::onActionOpen(void)
{
	
}
void	MainWindowImpl::onActionAdd(void)
{
	
}
void	MainWindowImpl::onActionEdit(void)
{
	
}
void	MainWindowImpl::onActionDel(void)
{
	
}
void	MainWindowImpl::onActionAbout(void)
{
	QMessageBox::about(this, tr("About"), tr("QRDC"));
}
void	MainWindowImpl::onActionAboutQt(void)
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}
void	MainWindowImpl::onActionHelp(void)
{
	
}

//
