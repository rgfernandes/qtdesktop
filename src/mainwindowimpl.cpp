#include "mainwindowimpl.h"
#include <QtGui>

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f), archive(new Archive()) {
	setupUi(this);
	setSlots();
	this->treeView->setModel(new ArchItemModel(archive));
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
	QString fileName = QFileDialog::getOpenFileName(0, tr("Open file"), QString(), tr("Archive") + " (*.7z *.arj *.rar *.zip *.tar *.tar.gz *.tgz *.tar.bz *.tar.bz2 *.tbz *.tbz2  *.tbz *.tar.7z *.tar.lzma *.tlzma *.tar.xz *.txz)");
	if (!fileName.isEmpty()) {
		archive->load(fileName);
		static_cast<ArchItemModel *>(this->treeView->model())->refresh();
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
	this->treeView->selectAll();
}

void	MainWindowImpl::onActionDeselect(void) {
	this->treeView->clearSelection();
}

void	MainWindowImpl::onActionAddFile(void) {
	QStringList files = QFileDialog::getOpenFileNames (0, tr("Add file(s)"));
	if (!files.isEmpty()) {
		archive->Add(&files);
		static_cast<ArchItemModel *>(this->treeView->model())->refresh();
	}
}

void	MainWindowImpl::onActionAddDirectory(void) {
	QString dir = QFileDialog::getExistingDirectory (0, tr("Add directory"));
	if (!dir.isEmpty()) {
		archive->Add(&dir);
		static_cast<ArchItemModel *>(this->treeView->model())->refresh();
	}
}

void	MainWindowImpl::onActionExtract(void) {
	// TODO: control dir/subfile selection
	QModelIndexList selected = this->treeView->selectionModel()->selectedRows();
	if (!selected.isEmpty()) {
		QString dir = QFileDialog::getExistingDirectory (0, tr("Add directory"));
		if (!dir.isEmpty()) {
			archive->Extract(selected, &dir);
			static_cast<ArchItemModel *>(this->treeView->model())->refresh();
		}
	}
}

void	MainWindowImpl::onActionDelete(void) {
	QModelIndexList selected = this->treeView->selectionModel()->selectedRows();
	if (!selected.isEmpty()) {
		archive->Delete(selected);
		static_cast<ArchItemModel *>(this->treeView->model())->refresh();
	}
}

void	MainWindowImpl::onActionAbout(void) {
	QMessageBox::about(this, tr("About"), QString("%1 v.%2\n(c) %3").arg(QCoreApplication::applicationName()).arg(QCoreApplication::applicationVersion()).arg(QCoreApplication::organizationName()));
}

void	MainWindowImpl::onActionAboutQt(void) {
	QMessageBox::aboutQt(this, tr("About Qt"));
}
