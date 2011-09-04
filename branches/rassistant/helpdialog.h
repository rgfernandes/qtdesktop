/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on Trolltech Qt Assistant 4.3, http://trolltech.com
****************************************************************************/
#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QList>
#include <QPair>
#include <QListWidget>
#include <QTreeWidget>
#include <QMap>
#include <QStringList>
#include <QValidator>
#include <QMenu>
#include <QHash>

#include "ui_helpdialog.h"
#include "index.h"
#include "helpwindow.h"
#include "docuparser.h"
#include "autoitems.h"

class QProgressBar;
class MainWindow;
class raEdit;
class IndexListModel;

//==================== class HelpNavigationListItem ====================

class HelpNavigationListItem : public QListWidgetItem
{
public:
	HelpNavigationListItem(QListWidget *ls, const QString &txt);

	void addLink(const QString &link);  //adds link to linkList
	QStringList links() const { return linkList; }

private:
	QStringList linkList;
}; // class HelpNavigationListItem

//==================== class SearchValidator ====================

class SearchValidator : public QValidator
{
	Q_OBJECT
public:
	SearchValidator(QObject *parent)
		: QValidator(parent) {}
	~SearchValidator() {}
	QValidator::State validate(QString &str, int &) const;
}; // class SearchValidator

//==================== class HelpDialog ====================

class HelpDialog : public QWidget
{
	Q_OBJECT
public:
	HelpDialog(QWidget *parent, MainWindow *h);

	inline QTabWidget *tabWidget() const  { return ui.tabWidget; }
	QString titleOfLink(const QString &link);
	bool eventFilter(QObject *, QEvent *);
	bool lastWindowClosed() { return lwClosed; }

	void timerEvent(QTimerEvent *e);
	void saveProject(QString projectFN);  //save contents and keywords to xml
	void enableProjectButtons();
	void newItem();
	QString copyImage(QString imgFN);

	AutoItems *dialogAutoItems;

signals:
	void showLink(const QString &s);
	void showSearchLink(const QString &s, const QStringList &terms);
	void fileRenamed(QString newFN);
	void setStatCountImages(int total, int unique, int external);
	void setStatImageLists(QStringList total, QStringList unique, QStringList external);
    void setStatDocLists(QStringList total, QStringList unique, QStringList external);
    void setStat(QString listName, QStringList list);
    //void setStat(QString listName, QTreeWidget tw);

public slots:
	void initialize();	//setup connections and actions and call initTabs()
	void initTabs();	//clear all tabs
	void startSearch();
	void addBookmark();
	void currentTabChanged(int index);
	void locateContents(const QString &link);  //find and select contents item corresponding to link
	void insertContents();
	void InsertContentsItem(QString title, QString fileName, QString iconFN); //insert new document in contents
	void updateItemProperties(QString title, QString fileName, QString iconFN); //update current item properties
	void showItemProperties();  //open item property dialog
	void removeItem(); 			//remove item from contents, ask for sure
	void removeItemDontAsk();	//remove item without asking
	void deleteItem(); 			//remove item from contents and delete linked file
	void showContentsAV(bool b);
	void showContentsAVHeader(bool b);
	void saveProject(); 		//save current project
	void updateItemFN(QString oldFullFN);
	void updateItemFN(QString oldFullFN, QString newBaseFN);
	void statCountImages();
    void getAllStat();
	void copyAllImages();
	void copyFileImages(QString fileName);

private slots:
//	void on_listContents_itemClicked(QTreeWidgetItem* item, int column);
	void on_BProjectAdd_clicked();
	void on_BProjectDelete_clicked();
	void on_buttonAdd_clicked();
	void on_buttonRemove_clicked();
	void on_termsEdit_returnPressed();
	void on_searchButton_clicked();
	void on_resultBox_itemActivated(QListWidgetItem*);
	void updateSearchButton(const QString &txt);

	void showResultPage(QListWidgetItem *);

	void loadIndexFile();
	void setupFullTextIndex();
	void showTopic();
	void searchInIndex(const QString &s);
	void toggleContents();
	void toggleIndex();
	void toggleBookmarks();
	void toggleSearch();
	void lastWinClosed();
	void setIndexingProgress(int prog);
	void showListItemMenu(const QPoint &pos);
	void showIndexItemMenu(const QPoint &pos);
	void showContentsItemMenu(const QPoint &pos);
	void showTreeItemMenu(const QPoint &pos);
	void insertBookmarks();
	void processEvents();
	void loadProjectFromList(int prjIindex);
	void fillSubList();
	void cloneItem(QTreeWidgetItem *item, bool subItem);
	void cloneDone();
	void showTopic(QTreeWidgetItem *);
	void currentItemChanged(QTreeWidgetItem* curItem, QTreeWidgetItem* prevItem);
	void itemChanged(QTreeWidgetItem* item, int column);
	void sortDisable();
	void sortAscending();
	void sortDescending();
	void locateItemByName(QString name);
	void nextMatchedItem();
   // void prevMatchedItem();
	//void GoToMatchedItem(int n);

private:
	void removeOldCacheFiles(bool onlyFulltextSearchIndex = false);
	void buildKeywordDB();
	quint32 getFileAges();
	void showIndexTopic();
	void showBookmarkTopic();
	void setupTitleMap();
	void saveBookmarks();
	void store(QTreeWidget *tw, QTextStream &ts); 	//for saving bookmarks
	void store(QTreeWidgetItem *i, QTextStream &ts); //for saving bookmarks
	void showContentsTopic();
	void showInitDoneMessage();
	void buildContentDict();
	QTreeWidgetItem * locateLink(QTreeWidgetItem *item, const QString &link);
	void CreateAutoItems();
	bool findItemByName(QString &name);
	bool findItemByName(QTreeWidgetItem *i, QString &name);
	void triggerAction(QTreeWidgetItem *item, QAction *action);
    void prepearFileList(QStringList &fileList);

	Ui::HelpDialog ui;
	MainWindow *mw;

	typedef QList<ContentItem> ContentList;	//ContentItem structure is described in docuparser.h
	ContentList contentList;
	IndexListModel *indexModel;
	Index *fullTextIndex;
	QMap<QString, QString> titleMap;
	bool indexDone, bookmarksInserted, titleMapDone, contentsInserted;
	bool lwClosed;
	QString documentationPath;
	QStringList terms, foundDocs; //for search
	QStringList contentFiles;	// list of unique files in project
	QStringList filesWithExternalImages; // list of files with external images
	bool initDoneMsgShown;
	void getAllContents();	//fill up contents from profile
	QString cacheFilesPath;	//path to store cache for search - it is user home dir
	QStringList keywordDocuments;	//for full text search
	int liTopLevelIndex; 			//for locate item procedures
	int liChildIndex, curChildIndex;//for locate item procedures
	QString str_NewItemTitle;
	bool newSameLevelItem;	//create a new item as a subitem or on the same level


	QMenu *itemPopup;
	QMenu *itemPopupContents;
	QMenu *itemPopupSubItems;
	QAction *actionItemProperties;
	QAction *actionItemInsert;
	QAction *actionItemAddFile;
	QAction *actionItemInsertSameLevel;
	QAction *actionItemAddFileSameLevel;
	QAction *actionItemRemove;
	QAction *actionItemDelete;
	QAction *actionItemAuto;
	QAction *actionOpenCurrentTab;
	QAction *actionOpenLinkInNewWindow;
	QAction *actionOpenLinkInNewTab;
	QAction *actionOpenLinkInExtEditor;
	QAction *actionOpenLinkInExtBrowser;

}; //class HelpDialog

#endif // HELPDIALOG_H
