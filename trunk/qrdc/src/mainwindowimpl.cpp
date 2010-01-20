#include "mainwindowimpl.h"
#include <QtGui>
#include <QtSql>
//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f) {
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	QActionGroup *viewGroup = new QActionGroup(this);
	viewGroup->addAction(actionConnections);
	viewGroup->addAction(actionHosts);
	viewGroup->addAction(actionVariables);
	viewGroup->addAction(actionProtocols);
	setSlots();
	// edit dialogs
	dialogC = new DialogConnectionImpl;
	dialogH = new DialogHostImpl;
	dialogP = new DialogProtocolImpl;
	dialogV = new DialogVarImpl;
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

void	MainWindowImpl::setModels(void)
{
	modelP = new QSqlTableModel();	// create model _after_ opening db
	modelP->setTable("p");
	modelP->setSort(1, Qt::AscendingOrder);
	//modelP->setEditStrategy(QSqlTableModel::OnManualSubmit);
	modelP->setHeaderData(0, Qt::Horizontal, tr("#"));
	modelP->setHeaderData(1, Qt::Horizontal, tr("Name"));
	modelP->setHeaderData(2, Qt::Horizontal, tr("Executable"));
	modelP->setHeaderData(3, Qt::Horizontal, tr("Port"));
	modelP->setHeaderData(4, Qt::Horizontal, tr("Console"));
	modelP->setHeaderData(5, Qt::Horizontal, tr("CmdLine"));
	modelP->setHeaderData(6, Qt::Horizontal, tr("Logo"));
	modelP->select();

	modelV = new QSqlTableModel();
	modelV->setTable("v");
	modelV->setSort(1, Qt::AscendingOrder);
	//modelV->setEditStrategy(QSqlTableModel::OnManualSubmit);
	modelV->select();
	modelV->setHeaderData(0, Qt::Horizontal, tr("#"));
	modelV->setHeaderData(1, Qt::Horizontal, tr("Name"));
	modelV->setHeaderData(2, Qt::Horizontal, tr("Value"));

	modelH = new QSqlTableModel();
	modelH->setTable("h");
	modelH->setSort(1, Qt::AscendingOrder);
	//modelH->setEditStrategy(QSqlTableModel::OnManualSubmit);
	modelH->setHeaderData(0, Qt::Horizontal, tr("#"));
	modelH->setHeaderData(1, Qt::Horizontal, tr("Name"));
	modelH->setHeaderData(2, Qt::Horizontal, tr("Value"));
	modelH->select();

	modelC = new QSqlRelationalTableModel();
	modelC->setTable("c");
	modelC->setSort(1, Qt::AscendingOrder);
	modelC->setRelation(modelC->fieldIndex("protoid"),	QSqlRelation("p", "id", "name")); 
	modelC->setRelation(modelC->fieldIndex("hostid"),	QSqlRelation("h", "id", "name")); 
	modelC->setRelation(modelC->fieldIndex("varid"),	QSqlRelation("v", "id", "name"));
	modelC->setHeaderData(0, Qt::Horizontal, tr("#"));
	modelC->setHeaderData(1, Qt::Horizontal, tr("Name"));
	modelC->setHeaderData(2, Qt::Horizontal, tr("Protocol"));
	modelC->setHeaderData(3, Qt::Horizontal, tr("Host"));
	modelC->setHeaderData(4, Qt::Horizontal, tr("Port"));
	modelC->setHeaderData(5, Qt::Horizontal, tr("Var"));
	modelC->setHeaderData(6, Qt::Horizontal, tr("CmdLine"));
	modelC->setHeaderData(7, Qt::Horizontal, tr("Comments"));
	modelC->select();

	actionConnections->setChecked(true);
	onActionConnections();

	dialogC->setModel(modelC);
	dialogH->setModel(modelH);
	dialogP->setModel(modelP);
	dialogV->setModel(modelV);
}

void	MainWindowImpl::onActionExit(void)
{
	close();
}

void	MainWindowImpl::onActionConnections(void)
{
	currentList = CONNECTION;
	tableView->setModel(modelC);
	tableView->resizeColumnsToContents();
	tableView->hideColumn(0);
	tableView->hideColumn(5);
	tableView->hideColumn(6);
	tableView->hideColumn(7);
}

void	MainWindowImpl::onActionHosts(void)
{
	currentList = HOST;
	tableView->setModel(modelH);
	tableView->resizeColumnsToContents();
	tableView->hideColumn(0);
}

void	MainWindowImpl::onActionVariables(void)
{
	currentList = VAR;
	tableView->setModel(modelV);
	tableView->resizeColumnsToContents();
	tableView->hideColumn(0);
}

void	MainWindowImpl::onActionProtocols(void)
{
	currentList = PROTO;
	tableView->setModel(modelP);
	tableView->resizeColumnsToContents();
	tableView->hideColumn(0);
	tableView->hideColumn(2);
	tableView->hideColumn(4);
	tableView->hideColumn(5);
	tableView->hideColumn(6);
}

void	MainWindowImpl::onActionAdd(void)
{
	switch (currentList) {
		case CONNECTION:
			dialogC->Add();
			modelC->select();
			break;
		case HOST:
			dialogH->Add();
			modelH->select();
			break;
		case PROTO:
			dialogP->Add();
			modelP->select();
			break;
		case VAR:
			dialogV->Add();
			modelV->select();
			break;
		default:
			break;
	}
}

void	MainWindowImpl::onActionEdit(void)
{
	QModelIndex idx = tableView->currentIndex();
	switch (currentList) {
		case CONNECTION:
			dialogC->Edit(idx);
			break;
		case HOST:
			dialogH->Edit(idx);
			break;
		case PROTO:
			dialogP->Edit(idx);
			break;
		case VAR:
			dialogV->Edit(idx);
			break;
		default:
			break;
	}
}

void	MainWindowImpl::onActionDel(void) {
	if (QMessageBox::question(this, QObject::tr("Deleting record"), QObject::tr("Are you sure?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
		int row = tableView->currentIndex().row();
		switch (currentList) {
			case CONNECTION:
				modelC->removeRows(row, 1);
				break;
			case HOST:
				modelH->removeRows(row, 1);
				break;
			case PROTO:
				modelP->removeRows(row, 1);
				break;
			case VAR:
				modelV->removeRows(row, 1);
				break;
			default:
				break;
		}
	}
}

void	MainWindowImpl::onActionSettings(void)
{
	;
}

void	MainWindowImpl::onActionAbout(void)
{
	QMessageBox::about(this, tr("About"), QString("%1 v.%2\n(c) %3").arg(QCoreApplication::applicationName()).arg(QCoreApplication::applicationVersion()).arg(QCoreApplication::organizationName()));
}

void	MainWindowImpl::onActionAboutQt(void)
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void	MainWindowImpl::onActionHelp(void)
{
	
}

void	MainWindowImpl::onActionOpen(void)
{
	if (currentList == CONNECTION) {
		// 1. get data
		QModelIndex idx = tableView->currentIndex();
		int id = modelC->data(idx.sibling(idx.row(), 0)).toInt();
		QSqlQuery q(QString("SELECT * FROM view_c WHERE id = %1").arg(id));
		if (q.next()) {
			int port	= q.value( 4).toInt();
			QString args	= q.value( 6).toString(); 
			QString program	= q.value( 9).toString(); 
			int p_port	= q.value(10).toInt();
			bool term	= q.value(11).toBool();
			QString p_args	= q.value(12).toString(); 
			QString host	= q.value(15).toString(); 
			QString var	= q.value(17).toString();
		// 2. process
			if (port == 0)
				port = p_port;
		// 3. lets go
			// [<term>] <program> <var> <args> <p_args(host, port)>
			QString cmdline = QString("%1 %2 %3 %4").arg(program).arg(var).arg(args).arg(p_args.arg(host).arg(port));
			//qDebug() << cmdline;
			if (term)
				cmdline = "konsole -e " + cmdline;
			QProcess::startDetached(cmdline);
		}
	}
}

//
