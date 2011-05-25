#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "archengine.h"
#include "architemmodel.h"

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private slots:
	void		onActionNew(void);
	void		onActionOpen(void);
	void		onActionSaveAs(void);
	void		onActionExtract(void);
	void		onActionTest(void);
	void		onActionProperties(void);
	void		onActionExit(void);
	void		onActionCut(void);
	void		onActionCopy(void);
	void		onActionPaste(void);
	void		onActionRename(void);
	void		onActionDelete(void);
	void		onActionSelectAll(void);
	void		onActionDeselect(void);
	void		onActionAddFile(void);
	void		onActionAddDirectory(void);
	void		onActionAbout(void);
	void		onActionAboutQt(void);
private:
	void		setSlots(void);
	Archive		*archive;
};

#endif




