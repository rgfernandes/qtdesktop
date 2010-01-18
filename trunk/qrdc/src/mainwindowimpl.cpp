#include "mainwindowimpl.h"
#include <QtGui>
//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	setupUi(this);
	setSlots();
}

void	MainWindowImpl::setSlots(void)
{
	connect( actionExit, SIGNAL( triggered() ),	this, SLOT( onActionExit() ) );
	connect( actionAbout, SIGNAL( triggered() ),	this, SLOT( onActionAbout() ) );
	connect( actionAboutQt, SIGNAL( triggered() ),	this, SLOT( onActionAboutQt() ) );
}

void	MainWindowImpl::setModels(void)
{
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
