/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on Trolltech Qt Assistant 4.3, http://trolltech.com
** and demo application TextEdit by Trolltech
****************************************************************************/
#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QMap>
#include <QPointer>
#include <QToolBar>
#include "raedit.h"
#include "itemproperty.h"
#include "linkproperty.h"
#include "docproperty.h"
#include "tableproperty.h"
#include "cellsplit.h"
#include "imageproperty.h"

class MainWindow;
class QKeyEvent;
class QMime;
class QMouseEvent;
class QMenu;
//------- taken from TextEdit -------
class QAction;
class QComboBox;
class QFontComboBox;
class QTextCharFormat;
//------- end of TextEdit section -------

//==================== class HelpWindow ====================

class HelpWindow : public raEdit
{
    Q_OBJECT
public:
    HelpWindow( MainWindow *m, QWidget *parent = 0);
    void setSource( const QUrl &name );
    void blockScrolling( bool b );
    void openLinkInNewWindow( const QString &link );
    void openLinkInNewPage( const QString &link );
    void addMimePath( const QString &path );

    QString getTagTitle();

    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *);
    
//    bool ModeSourceHTML;

signals:
    void chooseWebBrowser();
    void choosePDFReader();
    void insertContentsItem(QString title, QString fileName, QString iconFN);
    void updateContentsItem(QString title, QString fileName, QString iconFN);
    
public slots:
	void updateItem(QString title, QString fileName, QString iconFN);
	void setTagTitle(QString title);
    void fileNew();
    void fileOpen();
	bool fileSave();
	void ViewSource();
	//void insertTable(int rows, int columns, const QTextTableFormat & format);
	void tableInsert(int rows, int columns, QTextTableFormat format);
	void tableUpdate(int rows, int columns, QTextTableFormat tableFormat);
	void imageInsert(QImage image);
	void imageUpdate(QString html);
	void htmlInsert(QString html);
	//void imageInsert(int height, int width, QImage img);
	//void imageUpdate(int height, int width, QImage img);
	void insertRichText(QString text);	//insert HTML at cursor. Used from other modules, f.e. MainWindow::setMenuSign()
	void insertClipboard();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
	//------- taken from TextEdit -------
	virtual void closeEvent(QCloseEvent *e);

protected slots:
    void ensureCursorVisible();

private:

    bool hasAnchorAt(const QPoint& pos);

    MainWindow *mw;
	ItemProperties *itemprop;
	LinkProperties *linkprop;
	DocProperties *docprop;
	ImageProperties *imageprop;
	TableProperties *tableprop;
	DialogCellSplit *cellsplit;
		
    QString lastAnchor; //url for document in helpwindow
    bool blockScroll;
    bool shiftPressed;
    //bool ctrlPressed;
    bool newWindow;
    bool fwdAvail;
    bool backAvail;
    QMenu *menuTable, *menuRow, *menuColumn, *menuCell;
	
	void setupFileActions();
    void setupEditActions();
    void setupTextActions();
	void setupTableActions();
	void setupImageActions();
    bool load(const QString &f);
    bool maybeSave();
    void setCurrentFileName(const QString fName);	
	void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);

    QComboBox *comboStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;
    QToolBar *tb;
    int selCur, selStart, selEnd;
    
private slots:
    void openLinkInNewWindow();
    void openLinkInNewPage();
	void showLinkProperties();
	void removeLink();
	void updateLink(QString lText, QString lLocation);
	void loadNewItemFile();
	void showDocProperties();
    void addToSignatures();

    //------- taken from TextEdit -------
    bool fileSaveAs();
    void filePrint();
    void filePrintPreview();
    void filePrintPdf();

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textStyle(int styleIndex);
    void textColor();
//    void textAlign(QAction *a);
    void textAlignLeft();
    void textAlignCenter();
    void textAlignRight();
    void textAlignJustify();

    void currentCharFormatChanged(const QTextCharFormat &format); 
    void cursorPositionChanged();
    void clipboardDataChanged();

	void setupTableMenu();
	void tableNew();
	void tableProperties();
	void tableDelete();
	void rowInsertAbove();
	void rowInsertBelow();
	void rowDelete();
	void columnInsertLeft();
	void columnInsertRight();
	void columnDelete();
	void cellMerge();
	void cellSplit();	
	void cellSplit(int rows, int columns);

	void imageNew();
}; // class HelpWindow

#endif // HELPWINDOW_H
