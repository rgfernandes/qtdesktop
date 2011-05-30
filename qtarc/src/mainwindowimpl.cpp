#include "mainwindowimpl.h"
#include <QtGui>

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f),
	archive(0)
{
	setupUi(this);
	setSlots();
	//this->tableWidget->setColumnCount(3);
	//this->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Size") << tr("Date"));
}

void	MainWindowImpl::setSlots(void) {
	connect( actionNew,		SIGNAL( triggered() ),	this, SLOT( onActionNew() ) );
	connect( actionOpen,		SIGNAL( triggered() ),	this, SLOT( onActionOpen() ) );
	connect( actionSaveAs,		SIGNAL( triggered() ),	this, SLOT( onActionSaveAs() ) );
	connect( actionProperties,	SIGNAL( triggered() ),	this, SLOT( onActionProperties() ) );
	connect( actionExit,		SIGNAL( triggered() ),	this, SLOT( onActionExit() ) );
	connect( actionCut,		SIGNAL( triggered() ),	this, SLOT( onActionCut() ) );
	connect( actionCopy,		SIGNAL( triggered() ),	this, SLOT( onActionCopy() ) );
	connect( actionPaste,		SIGNAL( triggered() ),	this, SLOT( onActionPaste() ) );
	connect( actionSelectAll,	SIGNAL( triggered() ),	this, SLOT( onActionSelectAll() ) );
	connect( actionDeselect,	SIGNAL( triggered() ),	this, SLOT( onActionDeselect() ) );
	connect( actionAddFile,		SIGNAL( triggered() ),	this, SLOT( onActionAddFile() ) );
	connect( actionAddDirectory,	SIGNAL( triggered() ),	this, SLOT( onActionAddDirectory() ) );
	connect( actionExtract,		SIGNAL( triggered() ),	this, SLOT( onActionExtract() ) );
	connect( actionDelete,		SIGNAL( triggered() ),	this, SLOT( onActionDelete() ) );
	connect( actionAbout,		SIGNAL( triggered() ),	this, SLOT( onActionAbout() ) );
	connect( actionAboutQt,		SIGNAL( triggered() ),	this, SLOT( onActionAboutQt() ) );
}

void	MainWindowImpl::onActionNew(void) {
}

void	MainWindowImpl::onActionOpen(void) {
	QString fileName = QFileDialog::getOpenFileName(0, tr("Open file"), QString(), tr("Archive (*.7z *.tar *.tgz *.tbz2 *.tar.gz *.tar.bz2 *.zip *.rar)"));
	if (!fileName.isEmpty()) {
		if (archive)
			delete archive;
		archive = new Archive(fileName);
		//qDebug() << archive->List()->count();
		ArchItemModel *model = new ArchItemModel(archive);
		//QItemSelectionModel *selections = new QItemSelectionModel(model);
		//this->tableView->setModel(model);
		//this->tableView->setSelectionModel(selections);
		this->treeView->setModel(model);
		//this->treeView->setSelectionModel(selections);
	}
}

void	MainWindowImpl::onActionSaveAs(void) {
}

void	MainWindowImpl::onActionProperties(void) {
}

void	MainWindowImpl::onActionExit(void) {
	close();
}

void	MainWindowImpl::onActionCut(void) {
}

void	MainWindowImpl::onActionCopy(void) {
}

void	MainWindowImpl::onActionPaste(void) {
}

void	MainWindowImpl::onActionSelectAll(void) {
}

void	MainWindowImpl::onActionDeselect(void) {
}

void	MainWindowImpl::onActionAddFile(void) {
}

void	MainWindowImpl::onActionAddDirectory(void) {
}

void	MainWindowImpl::onActionExtract(void) {
}

void	MainWindowImpl::onActionDelete(void) {
}

void	MainWindowImpl::onActionAbout(void) {
	QMessageBox::about(this, tr("About"), QString("%1 v.%2\n(c) %3").arg(QCoreApplication::applicationName()).arg(QCoreApplication::applicationVersion()).arg(QCoreApplication::organizationName()));
}

void	MainWindowImpl::onActionAboutQt(void) {
	QMessageBox::aboutQt(this, tr("About Qt"));
}

//
