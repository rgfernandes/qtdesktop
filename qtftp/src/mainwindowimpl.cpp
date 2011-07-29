#include "mainwindowimpl.h"

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f) {
	setupUi(this);
	model = new QFileSystemModel;
	model->setRootPath(QDir::currentPath());
	treeView->setModel(model);
	setSlots();
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
	connect( actionDelete,		SIGNAL( triggered() ),	this, SLOT( onActionDelete() ) );
	connect( actionAbout,		SIGNAL( triggered() ),	this, SLOT( onActionAbout() ) );
	connect( actionAboutQt,		SIGNAL( triggered() ),	this, SLOT( onActionAboutQt() ) );
	connect( treeView->selectionModel(),	SIGNAL( currentChanged(const QModelIndex &, const QModelIndex &) ),	this, SLOT( onItemSelected(const QModelIndex &, const QModelIndex &) ) );
	connect( pbGo,			SIGNAL( clicked() ),	this, SLOT( onGo() ) );
}

void	MainWindowImpl::onActionNew(void) {
}

void	MainWindowImpl::onActionOpen(void) {
	//ftp.setHost("mirror.yandex.ru");	// ftp.qt.nokia.com
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
	treeView->selectAll();
}

void	MainWindowImpl::onActionDeselect(void) {
	treeView->clearSelection();
}

void	MainWindowImpl::onActionAddFile(void) {
}

void	MainWindowImpl::onActionAddDirectory(void) {
}

void	MainWindowImpl::onActionDelete(void) {
}

void	MainWindowImpl::onActionAbout(void) {
	QMessageBox::about(this, tr("About"), QString("%1 v.%2\n(c) %3").arg(QCoreApplication::applicationName()).arg(QCoreApplication::applicationVersion()).arg(QCoreApplication::organizationName()));
}

void	MainWindowImpl::onActionAboutQt(void) {
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void	MainWindowImpl::onItemSelected(const QModelIndex & selected, const QModelIndex &) {
	QFileInfo item = model->fileInfo(selected);
	//qDebug() << item.absoluteFilePath();
	leAddressBar->setText(model->filePath(selected));
}

void	MainWindowImpl::onGo() {
	QModelIndex item = model->setRootPath(leAddressBar->text());
	if (!item.isValid())
		qDebug() << "Wrong path";
	else {
		treeView->setCurrentIndex(item);
	}
}
