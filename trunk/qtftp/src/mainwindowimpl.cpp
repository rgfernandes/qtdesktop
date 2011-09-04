#include "mainwindowimpl.h"
#include <QFileDialog>

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f) {
	setupUi(this);
	model = new QDirModel;
	//model->setRootPath(QDir::currentPath());
	//model->setRootPath("ftp://mirror.yandex.ru/");
	//model->setRootPath(":/");
	treeView->setModel(model);
	treeView->setRootIndex(model->index(QDir::currentPath()));
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
	/*
	 * QFileDialog: localfs - ok, :/ - not, ftp - not
	 * QDir: local - ok, :/ - ok, ftp: tries
	 * QFile: local - ok, 
	 * QFileSystemModel: local - ok, :/ - not
	 * QDirModel: 
	 */
	//ftp.setHost("mirror.yandex.ru");	// ftp.qt.nokia.com
	//QString s("/mnt/shares/");
	//QString s(":/icons/icons/16x16/");	// file: empty.png
	//QString s("ftp://mirror.yandex.ru/fedora/");
	//QFileDialog::getOpenFileName(this, tr("Open file"), s,  tr("Images (*.png *.xpm *.jpg)"));
	//qDebug() << QDir(s).entryList();	//ok
	//QFileDialog::getExistingDirectory(this, tr("Open Directory"), "ftp://mirror.yandex.ru/fedora/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
	//model = new QFileSystemModel;
	//QModelIndex item = model->setRootPath(leAddressBar->text());
	//treeView->setModel(model);
	//QModelIndex item = model->setRootPath(leAddressBar->text());
	QModelIndex item = model->index(leAddressBar->text());
	treeView->setRootIndex(item);
	if (!item.isValid())
		qDebug() << "Wrong path";
	//else {
		qDebug() << model->filePath(item);
		treeView->setCurrentIndex(item);
		//treeView->setRootIndex(item);
		//model->reset();
	//}
}
