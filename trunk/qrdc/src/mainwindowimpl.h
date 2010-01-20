#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include <QtSql>
#include "ui_mainwindow.h"
#include "connectionimpl.h"
#include "hostimpl.h"
#include "protocolimpl.h"
#include "varimpl.h"
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
	void				setModels(void);
private slots:
	void				onActionExit(void);
	void				onActionConnections(void);
	void				onActionHosts(void);
	void				onActionVariables(void);
	void				onActionProtocols(void);
	void				onActionOpen(void);
	void				onActionAdd(void);
	void				onActionEdit(void);
	void				onActionDel(void);
	void				onActionSettings(void);
	void				onActionAbout(void);
	void				onActionAboutQt(void);
	void				onActionHelp(void);
private:
	LISTTYPE			currentList;
	QSqlRelationalTableModel	*modelC;
	QSqlTableModel			*modelP, *modelV, *modelH;
	void				setSlots(void);
	DialogConnectionImpl		*dialogC;
	DialogHostImpl			*dialogH;
	DialogProtocolImpl		*dialogP;
	DialogVarImpl			*dialogV;
};
#endif
