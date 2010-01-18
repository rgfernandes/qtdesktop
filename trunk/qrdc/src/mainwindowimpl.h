#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
//

enum	LISTTYPE	{
	PROTO,
	VAR,
	HOST,
	CONNECTION
};

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	void	setModels(void);
private slots:
	void	onActionExit(void);
	void	onActionConnections(void);
	void	onActionHosts(void);
	void	onActionVariables(void);
	void	onActionProtocols(void);
	void	onActionOpen(void);
	void	onActionAdd(void);
	void	onActionEdit(void);
	void	onActionDel(void);
	void	onActionAbout(void);
	void	onActionAboutQt(void);
	void	onActionHelp(void);
private:
	LISTTYPE	currentList;
	void	setSlots(void);
	bool	createConnection(void);
	bool	loadSql(QStringList &);
};
#endif
