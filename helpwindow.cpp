/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on Trolltech Qt Assistant 4.3, http://trolltech.com
** and demo application TextEdit by Trolltech
****************************************************************************/
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QStatusBar>
#include <QTextCodec>
#include <QTextCursor>
#include <QTextLayout>
#include <QTextObject>
#include <QTextStream>
#include <QTextCursor>
#include <QTextTable>
#include <QUrl>
#include <QtDebug>
#include <QtEvents>
#include <QTextDocumentFragment>
#include <qdesktopservices.h>
//------- taken from TextEdit -------
//#include "printpreview.h"
#include <QCloseEvent>
#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextList>
#include <QToolBar>
//------- end of TextEdit section -------

#include "helpwindow.h"
#include "mainwindow.h"
#include "tabbedbrowser.h"
#include "helpdialog.h"
#include "config.h"
#include "pcommon.h"

bool needConnection = true;

//-------------------------------------------------
HelpWindow::HelpWindow(MainWindow *w, QWidget *parent)
	: raEdit(parent)
	, mw(w)
	, blockScroll(false)
	, shiftPressed(false)
	, newWindow(false)
{
	FontSettings settings = Config::configuration()->fontSettings();
	setFont(settings.browserFont);

	// connect MainMenu functions
	if (needConnection) {
		itemprop = new ItemProperties(this);
		linkprop = new LinkProperties(this);
		docprop  = new DocProperties(this);
		imageprop = new ImageProperties(this);
		tableprop = new TableProperties(this);
		cellsplit = new DialogCellSplit(this);

		setupFileActions();
		setupEditActions();
		setupTextActions();
		setupTableActions();
		setupImageActions();

		//signals from raEdit class
		connect(this, SIGNAL(insertImageFromClipboard(QImage)), this, SLOT(imageInsert(QImage)));
		connect(this, SIGNAL(insertHtmlFromClipboard(QString)), this, SLOT(htmlInsert(QString)));

		connect(this, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
		connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
		connect(itemprop, SIGNAL(insertContentsItem(QString, QString, QString)), this, SLOT(loadNewItemFile()));  //open file of edited contents item
		connect(itemprop, SIGNAL(insertContentsItem(QString, QString, QString)), mw->helpDialog(), SLOT(InsertContentsItem(QString, QString, QString)));  //add item to contents
		connect(itemprop, SIGNAL(updateContentsItem(QString, QString, QString)), mw->helpDialog(), SLOT(updateItemProperties(QString, QString, QString)));
		connect(itemprop, SIGNAL(updateItemFN(QString)), mw->helpDialog(), SLOT(updateItemFN(QString)));
		connect(mw->helpDialog(), SIGNAL(fileRenamed(QString)), itemprop, SLOT(fileRenamed(QString)));
		connect(linkprop, SIGNAL(removeLink()), this, SLOT(removeLink()));
		connect(linkprop, SIGNAL(updateLink(QString, QString)), this, SLOT(updateLink(QString, QString)));
		connect(mw->browsers(), SIGNAL(showDocProperties()), this, SLOT(showDocProperties()));
		connect(docprop, SIGNAL(updateTitle(QString)), this, SLOT(setTagTitle(QString)));
		connect(docprop, SIGNAL(updateItemFN(QString)), mw->helpDialog(), SLOT(updateItemFN(QString)));
		connect(mw->helpDialog(), SIGNAL(fileRenamed(QString)), docprop, SLOT(fileRenamed(QString)));
		connect(tableprop, SIGNAL(createTable(int, int, QTextTableFormat)), this, SLOT(tableInsert(int, int, QTextTableFormat)));
		connect(tableprop, SIGNAL(updateTable(int, int, QTextTableFormat)), this, SLOT(tableUpdate(int, int, QTextTableFormat)));
		connect(cellsplit, SIGNAL(applySplit(int, int)), this, SLOT(cellSplit(int, int)));
		connect(imageprop, SIGNAL(newImage(QString)), this, SLOT(htmlInsert(QString)));
		connect(imageprop, SIGNAL(updateImage(QString)), this, SLOT(imageUpdate(QString)));
		//connect(imageprop, SIGNAL(newImage(int, int, QImage)), this, SLOT(imageInsert(int, int, QImage)));
		//connect(imageprop, SIGNAL(updateImage(int, int, QImage)), this, SLOT(imageUpdate(int, int, QImage)));

		needConnection = false;
	}
	// create submenus for table operations
	menuTable = new QMenu(tr("Table"));
		//menuTable->addAction( tr("Select"), this, SLOT(tableSelect()) );
		menuTable->addAction( tr("Delete"), this, SLOT(tableDelete()) );

	menuRow = new QMenu(tr("Row"));
		menuRow->addAction( tr("Insert above"), this, SLOT(rowInsertAbove()) );
		menuRow->addAction( tr("Insert below"), this, SLOT(rowInsertBelow()) );
		menuRow->addAction( tr("Delete"), this, SLOT(rowDelete()) );

	menuColumn = new QMenu(tr("Column"));
		menuColumn->addAction( tr("Insert befor"), this, SLOT(columnInsertLeft()) );
		menuColumn->addAction( tr("Insert after"), this, SLOT(columnInsertRight()) );
		menuColumn->addAction( tr("Delete"), this, SLOT(columnDelete()) );

	menuCell = new QMenu(tr("Cell"));
		menuCell->addAction( tr("Merge"), this, SLOT(cellMerge()) );
		menuCell->addAction( tr("Split...(Does not work yet!)"), this, SLOT(cellSplit()) );
}

//-------------------------------------------------
// open link in this window or pass url to OS
void HelpWindow::setSource(const QUrl &name)
{
	qDebug() << "- HelpWindow::setSource = " << name.toString();
	if (name.isValid()) {
		// pass URL to OS
		if (name.scheme() == QLatin1String("http")
		 || name.scheme() == QLatin1String("ftp")
		 || name.scheme() == QLatin1String("mailto")
		 || name.path().endsWith(QLatin1String("pdf"))) {
			bool launched = QDesktopServices::openUrl(name);
			if (!launched) {
				QMessageBox::information(mw, tr("Help"), tr("Unable to launch web browser.\n"), tr("OK"));
			}
			return;
		}
		//Open in active window or create new window
		QFileInfo fi(name.toLocalFile());
		if (name.scheme() == QLatin1String("file") && fi.exists() && fi.isFile() ) {
			if (newWindow || (shiftPressed && hasFocus())) {   //open in new window
				shiftPressed = false;
				mw->saveSettings();
				MainWindow *nmw = new MainWindow;
				nmw->move(mw->geometry().topLeft());
				nmw->show();

				if (mw->isMaximized())
					nmw->showMaximized();

				nmw->setup();
				nmw->showLink(name.toString());
			} else {   // open in active window
				// app loops in HelpWindow::setSource() if use this-> or just setSource()
				raEdit::setSource(name);
			}
			setCurrentFileName(name.toLocalFile());
			mw->statusBar()->showMessage(tr("Source: %1").arg(raEdit::source().toString()), 5000);
			return;
		}
	}
	//display error
	mw->statusBar()->showMessage(tr("Failed to open link: '%1'").arg(name.toString()), 5000);
	raEdit::setSource( Config::configuration()->ErrPage() );
	setHtml(tr("<div align=\"center\"><h1>The page could not be found</h1><br><h3>'%1'</h3></div>").arg(name.toString()));
	mw->browsers()->updateTitle(tr("Error..."));
}

//-------------------------------------------------
void HelpWindow::openLinkInNewWindow()
{
	if (lastAnchor.isEmpty())
		return;
	newWindow = true;
	setSource(lastAnchor);
	newWindow = false;
}

//-------------------------------------------------
void HelpWindow::openLinkInNewWindow(const QString &link)
{
	lastAnchor = link;
	openLinkInNewWindow();
}

//-------------------------------------------------
void HelpWindow::openLinkInNewPage()
{
	if(lastAnchor.isEmpty())
		return;
	mw->browsers()->newTab(lastAnchor);
	lastAnchor.clear();
}

//-------------------------------------------------
void HelpWindow::openLinkInNewPage(const QString &link)
{
	lastAnchor = link;
	openLinkInNewPage();
}

//-------------------------------------------------
bool HelpWindow::hasAnchorAt(const QPoint& pos)
{
	lastAnchor = anchorAt(pos);
	if (lastAnchor.isEmpty())
		return false;
	lastAnchor = source().resolved(lastAnchor).toString();
	if (lastAnchor.at(0) == QLatin1Char('#')) {
		QString src = source().toString();
		int hsh = src.indexOf(QLatin1Char('#'));
		lastAnchor = (hsh>=0 ? src.left(hsh) : src) + lastAnchor;
	}
	return true;
}

//-------------------------------------------------
void HelpWindow::contextMenuEvent(QContextMenuEvent *e)
{
	QMenu *m = new QMenu(0);
	if (hasAnchorAt(e->pos())) {
		m->addAction( tr("Open Link in New Window\tShift+LMB"), this, SLOT(openLinkInNewWindow()) );
		m->addAction( tr("Open Link in New Tab\tMMB"), this, SLOT(openLinkInNewPage()) );
		m->addAction( tr("Link properties..."), this, SLOT(showLinkProperties()) );
        m->addSeparator();
    }else{
        if ( textCursor().hasSelection() ){
			m->addAction( tr("Create link..."), this, SLOT(showLinkProperties()) );
            //m->addAction( tr("Add to signatures"), this, SLOT(addToSignatures()) );
            m->addSeparator();
        }
	}
	if ( textCursor().currentTable() ){
		m->addAction( tr("Table properties..."), this, SLOT(tableProperties()) );
		m->addMenu(menuTable);
		m->addMenu(menuRow);
		m->addMenu(menuColumn);
		m->addMenu(menuCell);
		m->addSeparator();
	}
    //m->addSeparator();
	mw->setupPopupMenu(m);
	m->exec(e->globalPos());
	delete m;
}

//-------------------------------------------------
void HelpWindow::showLinkProperties()
{
	QString s = "";
	QTextCursor cursor = raEdit::textCursor();
	QTextCharFormat cf = cursor.charFormat();
	selCur = cursor.position();
	if (cf.isAnchor()){		//select whole text of the link.
		while (cf.isAnchor()){	//move anchor to the start of hypertext link
			cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
			cf = cursor.charFormat();
		}
		cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
		cf = cursor.charFormat();
		while (cf.isAnchor()){ //move cursor to the end of hypertext link
			cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
			cf = cursor.charFormat();
		}
		cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
	}
	if (!cursor.hasSelection())//if there is no selection, select the word under cursor
		cursor.select(QTextCursor::WordUnderCursor);
	// Выделение текста остаётся только в этой процедуре, поэтому используем глоб переменные.
	// Cделать cursor глобальным нельзя, т.к. почему-то курсор всегда и в конце текста и на выделении. Баг qt? Использовать указатель на raEdit::textCursor() не помогло - таже ошибка. Подставил Edit::textCursor() вместо cursor - вообще прога зависать стала при просмотре свойств ссылки.
	selStart = cursor.selectionStart();
	selEnd = cursor.selectionEnd();
	s = cursor.selectedText();
	linkprop->setProperties(s, lastAnchor);
	linkprop->show();
	/*
	QTextCharFormat::setAnchor ( bool anchor )
	QTextCharFormat::setAnchorHref ( const QString & value )
	QTextCharFormat::setAnchorNames ( const QStringList & names )
	*/
}

//-------------------------------------------------
void HelpWindow::addToSignatures()
{
    mw->addSignature(raEdit::textCursor().selectedText());
}

//-------------------------------------------------
void HelpWindow::removeLink()
{
	QTextCursor cursor = raEdit::textCursor();
	cursor.setPosition(selStart, QTextCursor::MoveAnchor);
	cursor.setPosition(selEnd, QTextCursor::KeepAnchor);
	QString s =	cursor.selectedText();;
	cursor.removeSelectedText();
	cursor.insertText(s);
	//!+! убрать форматирование текста
}

//-------------------------------------------------
void HelpWindow::updateLink(QString lText, QString lLocation)
{
	QTextCursor cursor = raEdit::textCursor();
	QString s =	lText;
	if (lLocation.isEmpty()){
		removeLink();
	}else{
		s = "<a href=\"" + lLocation +"\">"+ lText +"</a>";
		qDebug() << "s = " << s;
		QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(s);
		cursor.setPosition(selStart, QTextCursor::MoveAnchor);
		cursor.setPosition(selEnd, QTextCursor::KeepAnchor);
		cursor.removeSelectedText();
		cursor.insertFragment(fragment);
		raEdit::textCursor().setPosition(selCur, QTextCursor::MoveAnchor);
   }
}

//-------------------------------------------------
void HelpWindow::mouseMoveEvent(QMouseEvent *e)
{
//	!+! Change cursor when mouse is over link and Ctrl is pressed.
	raEdit::mouseMoveEvent(e);
}

//-------------------------------------------------
void HelpWindow::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::XButton1) {
//        raEdit::backward();
		return;
	}

	if (e->button() == Qt::XButton2) {
  //      raEdit::forward();
		return;
	}

	if (e->button() == Qt::MidButton && hasAnchorAt(e->pos())){
		openLinkInNewPage();
		return;
	}
	if (e->button() == Qt::LeftButton && hasAnchorAt(e->pos()) && e->modifiers() && Qt::ControlModifier){
		setSource(lastAnchor);
		return;
	}
	if (e->button() == Qt::LeftButton && hasAnchorAt(e->pos()) && e->modifiers() && Qt::ShiftModifier){
		showLinkProperties();
		return;
	}
	raEdit::mouseReleaseEvent(e);
}

//-------------------------------------------------
void HelpWindow::mousePressEvent(QMouseEvent *e)
{
	shiftPressed = e->modifiers() & Qt::ShiftModifier;
	if (!(shiftPressed && hasAnchorAt(e->pos())))
		raEdit::mousePressEvent(e);
}

//-------------------------------------------------
void HelpWindow::keyPressEvent(QKeyEvent *e)
{
   //shiftPressed = e->modifiers() & Qt::ShiftModifier;
  // ctrlPressed = e->modifiers() & Qt::ControlModifier;
   raEdit::keyPressEvent(e);
}

//-------------------------------------------------
void HelpWindow::blockScrolling(bool b)
{
	blockScroll = b;
}

//-------------------------------------------------
void HelpWindow::ensureCursorVisible()
{
	if (!blockScroll)
		raEdit::ensureCursorVisible();
}

// ====================== from TextEdit =================================

void HelpWindow::closeEvent(QCloseEvent *e)
{
	if (maybeSave())
		e->accept();
	else
		e->ignore();
}

//-------------------------------------------------
void HelpWindow::setupFileActions()
{
	connect(mw->ui.actionNewFile, 		SIGNAL(triggered()), this, SLOT(fileNew()));
	connect(mw->ui.actionFileAdd, 		SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(mw->ui.actionSaveFile, 		SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(mw->ui.actionSaveFileAs, 	SIGNAL(triggered()), this, SLOT(fileSaveAs()));
	connect(mw->ui.actionExport_PDF, 	SIGNAL(triggered()), this, SLOT(filePrintPdf()));
	connect(mw->ui.actionFilePrint, 	SIGNAL(triggered()), this, SLOT(filePrint()));
	connect(mw->ui.actionPrint_Preview, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
	connect(mw->ui.actionItemProperties,SIGNAL(triggered()), mw->helpDialog(), SLOT(showItemProperties()));
	mw->ui.actionFileAdd->setShortcut(QKeySequence::Open);
	mw->ui.actionSaveFile->setShortcut(QKeySequence::Save);
	mw->ui.actionFilePrint->setShortcut(QKeySequence::Print);
	connect(mw, SIGNAL(saveOpenedLink()), this, SLOT(fileSave()));

	connect(raEdit::document(), SIGNAL(modificationChanged(bool)), mw->ui.actionSaveFile, SLOT(setEnabled(bool)));
	connect(raEdit::document(), SIGNAL(modificationChanged(bool)), this, SLOT(setWindowModified(bool)));

	setWindowModified(raEdit::document()->isModified());
	mw->ui.actionSaveFile->setEnabled(raEdit::document()->isModified());
}

//-------------------------------------------------
void HelpWindow::setupEditActions()
{
	connect(raEdit::document(), SIGNAL(undoAvailable(bool)), mw->ui.actionUndo, SLOT(setEnabled(bool)));
	connect(raEdit::document(), SIGNAL(redoAvailable(bool)), mw->ui.actionRedo, SLOT(setEnabled(bool)));
	connect(mw->ui.actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
	connect(mw->ui.actionRedo, SIGNAL(triggered()), this, SLOT(redo()));
	mw->ui.actionUndo->setEnabled(raEdit::document()->isUndoAvailable());
	mw->ui.actionRedo->setEnabled(raEdit::document()->isRedoAvailable());

	connect(mw->ui.actionCut,   SIGNAL(triggered()), this, SLOT(cut()));
	connect(mw->ui.actionCopy,  SIGNAL(triggered()), this, SLOT(copy()));
	connect(mw->ui.actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
	connect(this, SIGNAL(copyAvailable(bool)), mw->ui.actionCut, SLOT(setEnabled(bool)));
	connect(this, SIGNAL(copyAvailable(bool)), mw->ui.actionCopy, SLOT(setEnabled(bool)));
	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
	mw->ui.actionCut->setEnabled(false);
	mw->ui.actionCopy->setEnabled(false);
	mw->ui.actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

//-------------------------------------------------
void HelpWindow::setupTextActions()
{
	connect(mw->ui.actionTextBold, 		SIGNAL(triggered()), this, SLOT(textBold()));
	connect(mw->ui.actionTextItalic, 	SIGNAL(triggered()), this, SLOT(textItalic()));
	connect(mw->ui.actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
	connect(mw->ui.actionTextLeft, 		SIGNAL(triggered()), this, SLOT(textAlignLeft()));
	connect(mw->ui.actionTextCenter, 	SIGNAL(triggered()), this, SLOT(textAlignCenter()));
	connect(mw->ui.actionTextRight, 	SIGNAL(triggered()), this, SLOT(textAlignRight()));
	connect(mw->ui.actionTextJustify, 	SIGNAL(triggered()), this, SLOT(textAlignJustify()));
	connect(mw->ui.actionTextColor, 	SIGNAL(triggered()), this, SLOT(textColor()));

	comboStyle = new QComboBox(mw->ui.toolBarFormat);
	mw->ui.toolBarFormat->addWidget(comboStyle);
	comboStyle->addItem(tr("Standard","Text style (paragraph layout)"));
	comboStyle->addItem(tr("Bullet List (Disc)"));
	comboStyle->addItem(tr("Bullet List (Circle)"));
	comboStyle->addItem(tr("Bullet List (Square)"));
	comboStyle->addItem(tr("Ordered List (Decimal)"));
	comboStyle->addItem(tr("Ordered List (Alpha lower)"));
	comboStyle->addItem(tr("Ordered List (Alpha upper)"));
	connect(comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

	comboFont = new QFontComboBox(mw->ui.toolBarFormat);
	mw->ui.toolBarFormat->addWidget(comboFont);
	connect(comboFont, SIGNAL(activated(const QString &)), this, SLOT(textFamily(const QString &)));

	comboSize = new QComboBox(mw->ui.toolBarFormat);
	comboSize->setObjectName("comboSize");
	mw->ui.toolBarFormat->addWidget(comboSize);
	comboSize->setEditable(true);

	QFontDatabase db;
	foreach(int size, db.standardSizes())
		comboSize->addItem(QString::number(size));

	connect(comboSize, SIGNAL(activated(const QString &)), this, SLOT(textSize(const QString &)));
	comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));
}

//-------------------------------------------------
void HelpWindow::setupTableActions()
{
	connect(mw->ui.menuTable, SIGNAL(aboutToShow()), this, SLOT(setupTableMenu()));
	connect(mw->ui.actionInsertTable, SIGNAL(triggered()), this, SLOT(tableNew()));
	connect(mw->ui.actionTableProperty, SIGNAL(triggered()), this, SLOT(tableProperties()));
	connect(mw->ui.actionDeleteTable, SIGNAL(triggered()), this, SLOT(tableDelete()));
	connect(mw->ui.actionInsertRowAbove, SIGNAL(triggered()), this, SLOT(rowInsertAbove()));
	connect(mw->ui.actionInsertRowBelow, SIGNAL(triggered()), this, SLOT(rowInsertBelow()));
	connect(mw->ui.actionDeleteRow, SIGNAL(triggered()), this, SLOT(rowDelete()));
	connect(mw->ui.actionInsertColumnLeft, SIGNAL(triggered()), this, SLOT(columnInsertLeft()));
	connect(mw->ui.actionInsertColumnRight, SIGNAL(triggered()), this, SLOT(columnInsertRight()));
	connect(mw->ui.actionDeleteColumn, SIGNAL(triggered()), this, SLOT(columnDelete()));
}

//-------------------------------------------------
void HelpWindow::setupImageActions()
{
	connect(mw->ui.actionInsertImage, SIGNAL(triggered()), this, SLOT(imageNew()));
}

//-------------------------------------------------
bool HelpWindow::load(const QString &f)
{
	if (!QFile::exists(f))
		return false;
	QFile file(f);
	if (!file.open(QFile::ReadOnly))
		return false;

	raEdit::setSource(urlifyFileName(f));
	setCurrentFileName(f);
	return true;
}

//-------------------------------------------------
bool HelpWindow::maybeSave()
{
	if (!raEdit::document()->isModified())
		return true;
	if (Config::configuration()->CurFile().startsWith(QLatin1String(":/")))
		return true;
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(this, tr("Application"),
							   tr("The document has been modified.\n"
								  "Do you want to save your changes?"),
							   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	if (ret == QMessageBox::Save)
		return fileSave();
	else if (ret == QMessageBox::Cancel)
		return false;
	return true;
}

//-------------------------------------------------
void HelpWindow::setCurrentFileName(const QString fName)
{
	Config::configuration()->setCurFile(fName);
	raEdit::document()->setModified(false);
}

//-------------------------------------------------
void HelpWindow::loadNewItemFile()
{
	load(itemprop->filename());
}

//-------------------------------------------------
void HelpWindow::updateItem(QString title, QString fileName, QString iconFN)
{
		itemprop->setProperties(title,fileName,iconFN,false);
		itemprop->show();
}

//-------------------------------------------------
void HelpWindow::fileNew()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("New File..."),
							  Config::configuration()->CurPrjDir(), tr("HTML (*.htm *.html);;Text (*.txt);;All Files (*)"));
	if (!fn.isEmpty()){
		QFileInfo fi(fn);
		if (fi.suffix().isEmpty())
			fn += ".html";
		itemprop->setProperties("",fn,"",true);
		itemprop->show();
	}
}

//-------------------------------------------------
void HelpWindow::fileOpen()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
							  Config::configuration()->CurPrjDir(), tr("HTML (*.htm *.html);;Text (*.txt);;All Files (*)"));
	if (!fn.isEmpty()){
		QFileInfo fi(fn);
		if (fi.suffix().isEmpty())
			fn += ".html";
		itemprop->setProperties("",fn,"",true);
		itemprop->show();
	}
}

//-------------------------------------------------
bool HelpWindow::fileSave()
{
	if (Config::configuration()->CurFile().isEmpty())
		return fileSaveAs();

	QFile file(Config::configuration()->CurFile());
	if (!file.open(QFile::WriteOnly))
		return false;
	QTextStream ts(&file);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	//if (raEdit::ModeHtml()){
		ts << raEdit::document()->toHtml("UTF-8");
	//}else //!+! does not work yet. Need to edit rich text and plain in the same widget
		//ts << raEdit::document()->toPlainText();
	raEdit::document()->setModified(false);
	return true;
}

//-------------------------------------------------
bool HelpWindow::fileSaveAs()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("Save file as..."),
							   Config::configuration()->CurPrjDir(), tr("HTML (*.htm *.html);;All Files (*)"));
	if (fn.isEmpty())
		return false;
	if (QFileInfo(fn).suffix().isEmpty())
			fn.append(".html");

	setCurrentFileName(fn);
	return fileSave();
}

//-------------------------------------------------
void HelpWindow::filePrint()
{
#ifndef QT_NO_PRINTER
	QPrinter printer(QPrinter::HighResolution);
	printer.setFullPage(true);
	QPrintDialog *dlg = new QPrintDialog(&printer, this);
	if (raEdit::textCursor().hasSelection())
		dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
	dlg->setWindowTitle(tr("Print Document"));
	if (dlg->exec() == QDialog::Accepted) {
		raEdit::print(&printer);
	}
	delete dlg;
#endif
}

//-------------------------------------------------
void HelpWindow::filePrintPreview()
{
/*    PrintPreview *preview = new PrintPreview(raEdit::document(), this);
	preview->setWindowModality(Qt::WindowModal);
	preview->setAttribute(Qt::WA_DeleteOnClose);
	preview->show();*/
}

//-------------------------------------------------
void HelpWindow::filePrintPdf()
{
//#ifndef QT_NO_PRINTER
	QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", QString(), "*.pdf");
	if (!fileName.isEmpty()) {
		if (QFileInfo(fileName).suffix().isEmpty())
			fileName.append(".pdf");
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(fileName);
		raEdit::document()->print(&printer);
	}
//#endif
}

//-------------------------------------------------
void HelpWindow::textBold()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(mw->ui.actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
	mergeFormatOnWordOrSelection(fmt);
}

//-------------------------------------------------
void HelpWindow::textItalic()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(mw->ui.actionTextItalic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

//-------------------------------------------------
void HelpWindow::textUnderline()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(mw->ui.actionTextUnderline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

//-------------------------------------------------
void HelpWindow::textFamily(const QString &f)
{
	QTextCharFormat fmt;
	fmt.setFontFamily(f);
	mergeFormatOnWordOrSelection(fmt);
}

//-------------------------------------------------
void HelpWindow::textSize(const QString &p)
{
	QTextCharFormat fmt;
	mw->statusBar()->showMessage(tr("It does not work yet! Set font size to: %1.").arg(p), 5000);
	fmt.setFontPointSize(p.toFloat()); //!!! does not work yet for all cases!
	mergeFormatOnWordOrSelection(fmt);
}

//-------------------------------------------------
void HelpWindow::textStyle(int styleIndex)
{
	QTextCursor cursor = raEdit::textCursor();

	if (styleIndex != 0) {
		QTextListFormat::Style style = QTextListFormat::ListDisc;

		switch (styleIndex) {
			default:
            case 1:	style = QTextListFormat::ListDisc;          break;
            case 2:	style = QTextListFormat::ListCircle;    	break;
            case 3:	style = QTextListFormat::ListSquare;    	break;
            case 4:	style = QTextListFormat::ListDecimal;       break;
            case 5:	style = QTextListFormat::ListLowerAlpha;    break;
            case 6:	style = QTextListFormat::ListUpperAlpha;    break;
		}

		cursor.beginEditBlock();

		QTextBlockFormat blockFmt = cursor.blockFormat();

		QTextListFormat listFmt;

		if (cursor.currentList()) {
			listFmt = cursor.currentList()->format();
		} else {
			listFmt.setIndent(blockFmt.indent() + 1);
			blockFmt.setIndent(0);
			cursor.setBlockFormat(blockFmt);
		}

		listFmt.setStyle(style);

		cursor.createList(listFmt);

		cursor.endEditBlock();
	} else {
		// ####
		QTextBlockFormat bfmt;
		bfmt.setObjectIndex(-1);
		cursor.mergeBlockFormat(bfmt);
	}
}

//-------------------------------------------------
void HelpWindow::textColor()
{
	QColor col = QColorDialog::getColor(raEdit::textColor(), this);
	if (!col.isValid())
		return;
	QTextCharFormat fmt;
	fmt.setForeground(col);
	mergeFormatOnWordOrSelection(fmt);
	colorChanged(col);
}

//-------------------------------------------------
void HelpWindow::textAlignLeft()	{ raEdit::setAlignment(Qt::AlignLeft); }
void HelpWindow::textAlignCenter()	{ raEdit::setAlignment(Qt::AlignCenter); }
void HelpWindow::textAlignRight()	{ raEdit::setAlignment(Qt::AlignRight); }
void HelpWindow::textAlignJustify()	{ raEdit::setAlignment(Qt::AlignJustify); }

//-------------------------------------------------
void HelpWindow::currentCharFormatChanged(const QTextCharFormat &format)
{
	fontChanged(format.font());
	colorChanged(format.foreground().color());
}

//-------------------------------------------------
void HelpWindow::cursorPositionChanged()
{
	alignmentChanged(raEdit::alignment());
}

//-------------------------------------------------
void HelpWindow::clipboardDataChanged()
{
	mw->ui.actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

//-------------------------------------------------
void HelpWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
	QTextCursor cursor = raEdit::textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(format);
	raEdit::mergeCurrentCharFormat(format);
}

//-------------------------------------------------
void HelpWindow::fontChanged(const QFont &f)
{
	comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
	comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
	mw->ui.actionTextBold->setChecked(f.bold());
	mw->ui.actionTextItalic->setChecked(f.italic());
	mw->ui.actionTextUnderline->setChecked(f.underline());
}

//-------------------------------------------------
void HelpWindow::colorChanged(const QColor &c) //for text and background color indicators
{
	//!+! draw color indicator
 /*   QPixmap pix(16, 16);
	pix.fill(c);
	actionTextColor->setIcon(pix);*/
}

//-------------------------------------------------
void HelpWindow::alignmentChanged(Qt::Alignment a)
{
	if (a & Qt::AlignLeft) {
		mw->ui.actionTextLeft->setChecked(true);
	} else if (a & Qt::AlignHCenter) {
		mw->ui.actionTextCenter->setChecked(true);
	} else if (a & Qt::AlignRight) {
		mw->ui.actionTextRight->setChecked(true);
	} else if (a & Qt::AlignJustify) {
		mw->ui.actionTextJustify->setChecked(true);
	}
}

//-------------------------------------------------
void HelpWindow::setTagTitle(QString title)
{
	raEdit::setDocumentTitle(title);
	fileSave();
	mw->browsers()->updateTitle(title);
}

//-------------------------------------------------
QString HelpWindow::getTagTitle() { return raEdit::documentTitle(); }

//-------------------------------------------------
void HelpWindow::ViewSource()//!+! ability to view html source to edit tags
{
	fileSave();
	//bool viewHtml = !mw->ui.actionViewSource->isChecked();
	//raEdit::setModeHtml(viewHtml);
	setSource( raEdit::source().toString() );
}

//-------------------------------------------------
void HelpWindow::showDocProperties()
{
	docprop->setTitle(raEdit::documentTitle());
	docprop->setFileName(raEdit::source().toString());
	docprop->show();
}

//-------------------------------------------------
void HelpWindow::tableNew()
{
	tableprop->setProperties(2, 2, QTextTableFormat(), true);
	tableprop->show();
}

//-------------------------------------------------
void HelpWindow::tableInsert(int rows, int columns, QTextTableFormat format)
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.insertTable(rows, columns);
	table->setFormat(format);
}

//-------------------------------------------------
void HelpWindow::tableProperties()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	tableprop->setProperties(table->rows(), table->columns(), table->format(), false);
	tableprop->show();
}

//-------------------------------------------------
void HelpWindow::tableUpdate(int rows, int colums, QTextTableFormat tableFormat)
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	table->resize(rows, colums);
	table->setFormat(tableFormat);
}

//-------------------------------------------------
void HelpWindow::insertRichText(QString text)
{	//!+! reimplement procedures in reEdit to work with HTML comment, since QTextEdit does not understand it
	QTextCursor cursor = raEdit::textCursor();
	cursor.insertHtml(text);
}

//-------------------------------------------------
void HelpWindow::tableDelete()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	table->removeRows(0, table->rows());
}

//-------------------------------------------------
void HelpWindow::rowInsertAbove()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	QTextTableCell cell = table->cellAt(cursor);
	table->insertRows(cell.row(), 1);
}

//-------------------------------------------------
void HelpWindow::rowInsertBelow()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	QTextTableCell cell = table->cellAt(cursor);
	table->insertRows(cell.row()+1, 1);
}

//-------------------------------------------------
void HelpWindow::rowDelete()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	QTextTableCell cell = table->cellAt(cursor);
	table->removeRows(cell.row(), 1);
}

//-------------------------------------------------
void HelpWindow::columnInsertLeft()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	QTextTableCell cell = table->cellAt(cursor);
	table->insertColumns(cell.column(), 1);
}

//-------------------------------------------------
void HelpWindow::columnInsertRight()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	QTextTableCell cell = table->cellAt(cursor);
	table->insertColumns(cell.column()+1, 1);
}

//-------------------------------------------------
void HelpWindow::columnDelete()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	QTextTableCell cell = table->cellAt(cursor);
	table->removeColumns(cell.column(), 1);
}

//-------------------------------------------------
void HelpWindow::cellMerge()
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	table->mergeCells(cursor);
}

//-------------------------------------------------
void HelpWindow::cellSplit()
{
	cellsplit->show();
}

//-------------------------------------------------
void HelpWindow::cellSplit(int rows, int columns)
{
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	QTextTableCell cell = table->cellAt(cursor);
	Config::configuration()->toPrjLog(3,"Cell Split: cell.row()="+QString::number(cell.row())+", cell.column()="+QString::number(cell.column())+", rows="+QString::number(rows)+", columns="+QString::number(columns));
	table->splitCell(cell.row(), cell.column(), rows, columns);
}

//-------------------------------------------------
void HelpWindow::setupTableMenu()
{
	bool state = false;
	QTextCursor cursor = raEdit::textCursor();
	QTextTable *table = cursor.currentTable();
	if ( table != 0) state = true;

	mw->ui.actionTableProperty->setEnabled(state);
	mw->ui.actionInsertRowAbove->setEnabled(state);
	mw->ui.actionInsertRowBelow->setEnabled(state);
	mw->ui.actionInsertColumnLeft->setEnabled(state);
	mw->ui.actionInsertColumnRight->setEnabled(state);
	mw->ui.menuDelete->setEnabled(state);
}

//-------------------------------------------------
void HelpWindow::imageNew()
{
	imageprop->setProperties(0, 0, QImage());
	imageprop->show();
}

//-------------------------------------------------
void HelpWindow::imageInsert(QImage image)
{
	QString fn = uniqueFileName(Config::configuration()->CurPrjImgDir()+"/image.png");
	//Config::configuration()->toPrjLog(3,tr("Image fn: %1, CurPrjImgDir= %2","For log").arg(fn).arg(Config::configuration()->CurPrjImgDir()));
	if ( image.save(fn, "PNG") ){
		fn = relatifyFileName(fn, Config::configuration()->CurPrjDir());
		htmlInsert(QString("<img src=\"%1\" />").arg(fn));
	}else
		Config::configuration()->toPrjLog(3,tr("Could not save image from clipboard to file: %1","For log").arg(fn));
}

//-------------------------------------------------
void HelpWindow::imageUpdate(QString html)
{
	//QTextCursor cursor = raEdit::textCursor();
	Config::configuration()->toPrjLog(3,"Image update code: "+html);
}

//-------------------------------------------------
void HelpWindow::htmlInsert(QString html)
{
	QTextCursor cursor = raEdit::textCursor(); // this->
	//!+! change html (if external images) before insert, so we don't have to process the whole file after
	//QString text = html;
	//Config::configuration()->toPrjLog(3,"HTML to insert, before check of external image: "+text);
	//if (text.indexOf(QLatin1String("<img"), 0, Qt::CaseInsensitive) > 0){
	//	text = copyExternalImages(text);
	cursor.insertHtml(html);
	if (html.indexOf(QLatin1String("<img"), 0, Qt::CaseInsensitive) > -1){
		qDebug() << "- Insert html and replace img tags";
		fileSave();
		mw->helpDialog()->copyFileImages(source().toString());
	}
}


//-------------------------------------------------
// To test future new MIME types
void HelpWindow::insertClipboard()
{
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();
	//QString cbText = clipboard->text(); // grabs just text from data
	QString cbText(mimeData->data(QString(QMetaType::QByteArray)));	//does not work
	QTextCursor cursor = raEdit::textCursor();
	cursor.insertText(cbText);
}
