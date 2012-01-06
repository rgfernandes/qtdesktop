#include "mainwindowimpl.h"
#include "rsscontainer.h"

#include <QMenu>
//#include <QTGui>
#include <QTextEdit>
#include <QInputDialog>
#include <QDockWidget>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QDateTime>
#include <QHeaderView>

//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f ) 
	: QMainWindow( parent, f )
{
	setupUi(this);
	statusBar();
	setIcons();

	createDockWindows();
	
	m_rssContainer = new RssContainer( this );
	
	setCentralWidget( &m_view );
	
	m_view.page()->setLinkDelegationPolicy( QWebPage::DelegateAllLinks );
	setConnection();
	
	QWidget::move( 100, 100 );
	QWidget::resize( 800, 600 );
	
	statusBar()->showMessage( tr( "Ready" ));
	m_rssContainer->loadData();
}

MainWindowImpl::~MainWindowImpl()
{
	delete m_rssContainer;
}

void MainWindowImpl::createDockWindows()
{
	QDockWidget* dock = new QDockWidget( tr( "Feeds "), this );
	dock->setAllowedAreas( Qt::AllDockWidgetAreas );
	dock->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	//m_channels = new QListWidget(dock);
	
	m_channelColumns << "Channel" << "Unreaded ";
	m_channelsTree = new QTreeWidget(this);
	m_channelsTree->setHeaderLabels( m_channelColumns );
	m_channelsTree->header()->resizeSection( 0, 250 );
	m_channelsTree->header()->setMovable( false );
	m_channelsTree->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_channelsTree->setSelectionMode( QAbstractItemView::SingleSelection );
	m_channelsTree->setAlternatingRowColors( true );
	m_channelsTree->setRootIsDecorated( false );	

	//TODO: add here channels + icon 	 
	dock->setWidget( m_channelsTree );
	addDockWidget( Qt::TopDockWidgetArea, dock );

	//TODO: check the second parameter to specify widget flags
	dock = new QDockWidget( tr( "feeds" ), this );
	dock->setAllowedAreas( Qt::AllDockWidgetAreas );
	dock->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );

	m_ColumnHeaders << "Title" << "Date";
	m_itemsTree = new RssItemTree( this );
	m_itemsTree->setHeaderLabels( m_ColumnHeaders );
	m_itemsTree->header()->resizeSection( 0, 250 );
	m_itemsTree->header()->setMovable( false );
	m_itemsTree->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_itemsTree->setSelectionMode( QAbstractItemView::SingleSelection );
	m_itemsTree->setAlternatingRowColors( true );
	m_itemsTree->setRootIsDecorated( false );	

	dock->setWidget( m_itemsTree );
	addDockWidget( Qt::TopDockWidgetArea, dock );
}

void MainWindowImpl::setIcons()
{
	QIcon::setThemeName("oxygen");	// FIXME
	actionExit->setIcon(QIcon::fromTheme("application-exit"));
	actionGotoEntryPrev->setIcon(QIcon::fromTheme("go-previous"));
	actionGotoEntryNext->setIcon(QIcon::fromTheme("go-next"));
	actionGotoFeedPrev->setIcon(QIcon::fromTheme("go-up"));
	actionGotoFeedNext->setIcon(QIcon::fromTheme("go-down"));
	actionFeedDelete->setIcon(QIcon::fromTheme("edit-delete"));
	actionFeedUpdate->setIcon(QIcon::fromTheme("view-refresh"));
	actionFeedUpdateAll->setIcon(QIcon::fromTheme("mail-send-receive"));
	actionEntryDelete->setIcon(QIcon::fromTheme("list-remove"));
	actionEntryMarkImportant->setIcon(QIcon::fromTheme("mail-mark-important"));
	actionEntryMarkNew->setIcon(QIcon::fromTheme("mail-mark-new"));
	actionHelpAbout->setIcon(QIcon::fromTheme("help-about"));
	/*
	QMenu* menu = new QMenu( "&Channel", this );
	QMainWindow::menuBar()->addMenu( menu );
	menu->addAction( "&Add new",this, SLOT( addChannel( )), tr( "Alt+A" ));
	menu->addAction( "&Edit",this, SLOT( editChannel( )), tr( "Alt+E" ));
	menu->addAction( "&Delete",this, SLOT( deleteChannel( )), tr( "Alt+D" ));
	menu->addAction( "&Refresh", this, SLOT( refresh( )), tr( "Alt+R" ));
	menu->addAction( "Read A&ll", this, SLOT( readAll( )),tr( "Alt+L" ));
	*/
}

void MainWindowImpl::setConnection()
{
	// menus
	connect( actionFeedAdd, SIGNAL( triggered() ), this, SLOT( onFeedAdd() ) );
	connect( actionFeedDelete, SIGNAL( triggered() ), this, SLOT( onFeedDelete() ) );
	connect( actionFeedProperties, SIGNAL( triggered() ), this, SLOT( onFeedProperties() ) );
	connect( actionFeedUpdate, SIGNAL( triggered() ), this, SLOT( onFeedUpdate() ) );
	connect( actionFeedUpdateAll, SIGNAL( triggered() ), this, SLOT( onFeedUpdateAll() ) );
	//connect( actionFeed, SIGNAL( triggered() ), this, SLOT( onFeed() ) );

	// lists
	connect( m_itemsTree, SIGNAL ( itemActivated ( QTreeWidgetItem*,int )), this, SLOT( itemClicked( QTreeWidgetItem *, int )));
	connect( m_itemsTree, SIGNAL ( itemClicked ( QTreeWidgetItem*, int )), this, SLOT( itemClicked( QTreeWidgetItem *, int )));
	connect( m_channelsTree, SIGNAL( itemClicked ( QTreeWidgetItem*, int )), this, SLOT( channelClicked( QTreeWidgetItem*, int )));
	connect( this, SIGNAL( clearItems ()), m_itemsTree, SLOT(clear()));

	//from container
	connect( m_rssContainer, SIGNAL( itemAdded( int, ItemState, QString&, QDateTime& )), this, SLOT( itemAdded( int, ItemState, QString&, QDateTime& )));
	connect( m_rssContainer, SIGNAL( channelAdded( QString&, int ,bool )), this, SLOT( channelAdded( QString&, int ,bool )));
	connect( m_rssContainer, SIGNAL( displayError( QString )), this, SLOT( displayError( QString )));
	connect( m_rssContainer, SIGNAL( unreadedCount( int, int )), this, SLOT( unreadedCount( int, int )));

	//QWebView
	connect( &m_view, SIGNAL( loadProgress( int )), this, SLOT( viewProgress( int )));
	connect( &m_view, SIGNAL( loadFinished( bool )), this, SLOT( viewLoadFinished( bool )));
	connect( &m_view, SIGNAL( linkClicked( const QUrl& )), this, SLOT( viewLinkClicked( const QUrl& )));

	connect( this, SIGNAL( errorMsg( const QString& )), &m_errorMsg, SLOT( showMessage( const QString& )));

}

void MainWindowImpl::itemClicked( QTreeWidgetItem* item, int row )
{	
	QString desc = m_rssContainer->getItemDescription( currentChannelInd(), currentItemInd( ));
	m_view.setHtml( desc );
}

void MainWindowImpl::channelClicked( QTreeWidgetItem* item, int column )
{
	emit(clearItems( ));
	int ind = currentChannelInd();
	m_rssContainer->getChannelFeeds( ind );
	m_view.setHtml( m_rssContainer->getChannelDescription( ind ));
}

void MainWindowImpl::onFeedAdd()
{
	bool ok( false );
	QString url = QInputDialog::getText( this, tr( "Add new channel" ), tr( "Enter url" ),  QLineEdit::Normal, tr( "http://" ), &ok );
	if( ok && !url.isEmpty( ))
	{
		if( m_rssContainer )
		{
			m_rssContainer->addChannel( url );
		}
	}
}

void MainWindowImpl::onFeedProperties()
{
	bool ok( false );
	int ind = currentChannelInd();
	QUrl qUrl = m_rssContainer->getChannelUrl( ind );
	QString url = QInputDialog::getText( this, tr( "Edit" ), tr( "Enter url" ), QLineEdit::Normal, qUrl.toString(), &ok );
	if( ok && !url.isEmpty() )
	{
		qUrl = url;
		if( !qUrl.isValid( ))
		{
			displayError( tr( "Invalid Url" ));
		}
		else
		{
			m_rssContainer->addChannel( url );
			onFeedDelete();
		}
	}
}

void MainWindowImpl::onFeedDelete()
{
	//delete channel
	int ind = currentChannelInd();
	int count = m_channelsTree->topLevelItemCount();
	m_rssContainer->deleteChannel( ind );
	m_itemsTree->clear();
	QTreeWidgetItem* item = m_channelsTree->currentItem();
	delete item;
	
	if( ind > 0)
		m_rssContainer->getChannelFeeds( ind-1 );
	else if( count > 1)
		m_rssContainer->getChannelFeeds( 0 );
}

void MainWindowImpl::itemAdded( int channelId, ItemState state, QString& itemTitle, QDateTime& itemDate )
{
	if( m_channelsTree->currentIndex().row() == channelId )
	{
		QTreeWidgetItem *item = new QTreeWidgetItem( m_itemsTree );
		item->setText( 0,itemTitle );
		item->setText( 1,itemDate.toString( DATEFORMAT ));
		item->setFlags( item->flags() & ~Qt::ItemIsEditable );
		item->setTextAlignment( 1, Qt::AlignHCenter );
		QBrush brush( getColor( state ));
		item->setForeground( 0, brush );
		item->setForeground( 1, brush );
	}
}

void MainWindowImpl::channelAdded( QString& channel, int unreadedCount, bool markFirst )
{
	QTreeWidgetItem *item = new QTreeWidgetItem( m_channelsTree );
	item->setText( 0,channel );
	item->setText( 1, QString::number( unreadedCount ));
	item->setFlags( item->flags() & ~Qt::ItemIsEditable );
	item->setTextAlignment( 1, Qt::AlignHCenter );
	m_channelsTree->addTopLevelItem( item );

	if(!markFirst)
	{
		m_channelsTree->setCurrentItem( item );
	}
	else
		m_channelsTree->setCurrentItem( m_channelsTree->itemAt( 0, 0 ));

	//clear items here, because new are going to be added.
	m_itemsTree->clear();
}

void MainWindowImpl::quit()
{
	m_rssContainer->storeData();
}

void MainWindowImpl::unreadedCount( int channelId, int count )
{
	//qDebug() << __LOG_FNAME__;
	//qDebug() << channelId << count;
	//TODO: change unreaded msg amount
	QTreeWidgetItem* item = m_channelsTree->topLevelItem( channelId );
	item->setText( 1, QString::number( count ));
}

void MainWindowImpl::onFeedUpdate()
{
	m_rssContainer->update();
}

void MainWindowImpl::onFeedUpdateAll()
{
	m_rssContainer->readAll( currentChannelInd( ));
	m_itemsTree->clear();
	m_rssContainer->getChannelFeeds( currentChannelInd( ));
}

void MainWindowImpl::viewProgress( int progress )
{
	QString message = QString( "Loading.. %1\%" ).arg( progress );

	statusBar()->showMessage( message );
}

void MainWindowImpl::viewLoadFinished( bool ok )
{
	int itemInd = currentItemInd();
	if( itemInd != -1 )
	{
		m_rssContainer->setItemState( currentChannelInd(), currentItemInd(), eReaded );
		QTreeWidgetItem* item = m_itemsTree->currentItem();
		
		QBrush brush(getColor( eReaded ));
		item->setForeground( 0, brush );
		item->setForeground( 1, brush );
		
		statusBar()->showMessage( tr(" Loaded" ), 2000 );
	}
}

void MainWindowImpl::viewLinkClicked( const QUrl& rUrl )
{
	QDesktopServices::openUrl( rUrl );
}


QColor MainWindowImpl::getColor( ItemState state )
{
	QColor color;
	if( state == eNew )
		color = Qt::red;
	else if ( state == eUnreaded )
		color = Qt::blue;
	else
		color = Qt::black;
	
	return color;
}

void MainWindowImpl::displayError( QString msg )
{
	emit( errorMsg( msg ));
}

int MainWindowImpl::currentChannelInd()
{
	return m_channelsTree->currentIndex().row();
}

int MainWindowImpl::currentItemInd()
{
	return m_itemsTree->currentIndex().row();
}
//eof
