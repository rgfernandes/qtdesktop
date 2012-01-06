#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

//includes
#include <QMainWindow>
#include <QDebug>
#include <QListWidgetItem>
#include <QWebView>
#include <QPointer>
#include <QErrorMessage>
#include <QTreeWidget>

#include "ui_mainwindow.h"
#include "qrssglobals.h"
#include "rssitemtree.h"

//forward declaration
class QListWidget;
class QTextEdit;
class RssContainer;

//MainWindowImpl class
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	
virtual ~MainWindowImpl();
	
	private:
		void setIcons();
		void createDockWindows();
		void setConnection();
		QColor getColor( ItemState );
		int currentChannelInd();
		int currentItemInd();

	public slots:
		void itemClicked( QTreeWidgetItem *, int column );
		void channelClicked( QTreeWidgetItem*, int column );
		
		void onFeedAdd();		// addChannel
		void onFeedProperties();	// editChannel
		void onFeedDelete();		// deleteChannel
		void onFeedUpdate();		// refresh
		void onFeedUpdateAll();		// readAll
		
		void itemAdded( int channelId, ItemState type, QString& itemTitle, QDateTime& itemDate );
		void channelAdded( QString& channel,int unreadedCount, bool markFirst );
		void quit();
		void unreadedCount( int channelId, int count );
		
		void viewProgress( int );
		void viewLoadFinished( bool );
		void viewLinkClicked( const QUrl& );

		void displayError( QString msg );

	signals:
		void clearItems();
		void errorMsg( const QString& );
	
private:
	RssContainer* 		m_rssContainer;
	QWebView 		m_view;
	QStringList		m_ColumnHeaders;
	QStringList		m_channelColumns;
	QPointer<RssItemTree>	m_itemsTree;
	QPointer<QTreeWidget>	m_channelsTree;
	QErrorMessage		m_errorMsg;
};
#endif //MAINWINDOWIMPL_H
//eof
