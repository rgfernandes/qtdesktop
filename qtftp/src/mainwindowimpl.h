#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include <QtCore>
#include <QtGui>
#include "ui_mainwindow.h"
#include "ftphandler.h"

class QFSModel : public QFileSystemModel {
	/* To change:
	 * static QString qt_GetLongPathName(const QString &strShortPath) (WIN32)
	 * QFileSystemModelPrivate::QFileSystemNode *QFileSystemModelPrivate::node(const QString &path, bool fetch) const
	 * QString QFileSystemModelPrivate::filePath(const QModelIndex &index) const
	 * QModelIndex QFileSystemModel::setRootPath(const QString &newPath)
	 * QFileSystemModelPrivate::QFileSystemNode* QFileSystemModelPrivate::addNode(QFileSystemNode *parentNode, const QString &fileName, const QFileInfo& info) (WIN*)
	 * void QFileSystemModelPrivate::init() (due FSWatcher)
	 */
};

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private slots:
	void		onActionNew(void);
	void		onActionOpen(void);
	void		onActionSaveAs(void);
	void		onActionProperties(void);
	void		onActionExit(void);
	void		onActionCut(void);
	void		onActionCopy(void);
	void		onActionPaste(void);
	void		onActionSelectAll(void);
	void		onActionDeselect(void);
	void		onActionAddFile(void);
	void		onActionAddDirectory(void);
	void		onActionDelete(void);
	void		onActionAbout(void);
	void		onActionAboutQt(void);
	void		onItemSelected(const QModelIndex &, const QModelIndex &);
	void		onGo(void);
private:
	void		setSlots(void);
	FtpEngineHandler ftp;
	QDirModel *model;
};

#endif
