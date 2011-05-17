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

#include "printpreview.h"

#include <QAbstractScrollArea>
#include <QPrintDialog>
#include <QPrinter>
#include <QToolBar>
#include <QAction>
#include <QTextFormat>
#include <QMouseEvent>
#include <QTextFrame>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <QPageSetupDialog>
#include <QStatusBar>

#ifdef Q_WS_MAC
const QString rsrcPath = ":/images/mac";
#else
const QString rsrcPath = ":/images/win";
#endif

static inline int inchesToPixels(float inches, QPaintDevice *device)
{
    return qRound(inches * device->logicalDpiY());
}

static inline qreal mmToInches(double mm)
{
    return mm*0.039370147;
}

class PreviewView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    PreviewView(QTextDocument *document, PrintPreview *printPrev);

    inline void updateLayout() { resizeEvent(0); viewport()->update(); }

public slots:
    void zoomIn();
    void zoomOut();

protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void resizeEvent(QResizeEvent *);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    void paintPage(QPainter *painter, int page);
    QTextDocument *doc;
    qreal scale;
    int interPageSpacing;
    QPoint mousePressPos;
    QPoint scrollBarValuesOnMousePress;
    PrintPreview *printPreview;
};

PreviewView::PreviewView(QTextDocument *document, PrintPreview *printPrev)
    : printPreview(printPrev)
{
    verticalScrollBar()->setSingleStep(20);
    horizontalScrollBar()->setSingleStep(20);

    viewport()->setBackgroundRole(QPalette::Dark);

    doc = document;
    scale = 1.0;
    interPageSpacing = 30;
}

void PreviewView::zoomIn()
{
    scale += 0.2;
    resizeEvent(0);
    viewport()->update();
}

void PreviewView::zoomOut()
{
    scale -= 0.2;
    resizeEvent(0);
    viewport()->update();
}

void PreviewView::paintEvent(QPaintEvent *)
{
    QPainter p(viewport());

    p.translate(-horizontalScrollBar()->value(), -verticalScrollBar()->value());
    p.translate(interPageSpacing, interPageSpacing);

    const int pages = doc->pageCount();
    for (int i = 0; i < pages; ++i) {
        p.save();
        p.scale(scale, scale);

        paintPage(&p, i);

        p.restore();
        p.translate(0, interPageSpacing + printPreview->paperSize.height() * scale);
    }
}

void PreviewView::paintPage(QPainter *painter, int page)
{
    const QSizeF pgSize = doc->pageSize();

    QColor col(Qt::black);

    painter->setPen(col);
    painter->setBrush(Qt::white);
    painter->drawRect(QRectF(QPointF(0, 0), printPreview->paperSize));
    painter->setBrush(Qt::NoBrush);

    col = col.light();
    painter->drawLine(QLineF(printPreview->paperSize.width(), 1,
                             printPreview->paperSize.width(), printPreview->paperSize.height() - 1));

    col = col.light();
    painter->drawLine(QLineF(printPreview->paperSize.width(), 2,
                             printPreview->paperSize.width(), printPreview->paperSize.height() - 2));

    QRectF docRect(QPointF(0, page * pgSize.height()), pgSize);
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.clip = docRect;

    // don't use the system palette text as default text color, on HP/UX
    // for example that's white, and white text on white paper doesn't
    // look that nice
    ctx.palette.setColor(QPalette::Text, Qt::black);

    painter->translate(0, - page * pgSize.height());

    painter->translate(printPreview->pageTopLeft);
    painter->setClipRect(docRect);//.translated(printPreview->pageTopLeft));
    doc->documentLayout()->draw(painter, ctx);
}

void PreviewView::resizeEvent(QResizeEvent *)
{
    const QSize viewportSize = viewport()->size();

    QSize docSize;
    docSize.setWidth(qRound(printPreview->paperSize.width() * scale + 2 * interPageSpacing));
    const int pageCount = doc->pageCount();
    docSize.setHeight(qRound(pageCount * printPreview->paperSize.height() * scale + (pageCount + 1) * interPageSpacing));

    horizontalScrollBar()->setRange(0, docSize.width() - viewportSize.width());
    horizontalScrollBar()->setPageStep(viewportSize.width());

    verticalScrollBar()->setRange(0, docSize.height() - viewportSize.height());
    verticalScrollBar()->setPageStep(viewportSize.height());
}

void PreviewView::mousePressEvent(QMouseEvent *e)
{
    mousePressPos = e->pos();
    scrollBarValuesOnMousePress.rx() = horizontalScrollBar()->value();
    scrollBarValuesOnMousePress.ry() = verticalScrollBar()->value();
    e->accept();
}

void PreviewView::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressPos.isNull()) {
        e->ignore();
        return;
    }

    horizontalScrollBar()->setValue(scrollBarValuesOnMousePress.x() - e->pos().x() + mousePressPos.x());
    verticalScrollBar()->setValue(scrollBarValuesOnMousePress.y() - e->pos().y() + mousePressPos.y());
    horizontalScrollBar()->update();
    verticalScrollBar()->update();
    e->accept();
}

void PreviewView::mouseReleaseEvent(QMouseEvent *e)
{
    mousePressPos = QPoint();
    e->accept();
}

PrintPreview::PrintPreview(const QTextDocument *document, QWidget *parent)
    : QMainWindow(parent), printer(QPrinter::HighResolution)
{
    setWindowTitle(tr("TextEdit Demo - Print Preview"));

    printer.setFullPage(true);
    doc = document->clone();

    view = new PreviewView(doc, this);
    setCentralWidget(view);
    resize(800, 600);

    doc->setUseDesignMetrics(true);
    doc->documentLayout()->setPaintDevice(view->viewport());

    // add a nice 2 cm margin
    const qreal margin = inchesToPixels(mmToInches(20), this);
    QTextFrameFormat fmt = doc->rootFrame()->frameFormat();
    fmt.setMargin(margin);
    doc->rootFrame()->setFrameFormat(fmt);

    setup();

    QFont f(doc->defaultFont());
    f.setPointSize(10);
    doc->setDefaultFont(f);

    QToolBar *tb = addToolBar(tr("Print"));
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QAction *a;
    a = new QAction(/*QIcon(rsrcPath + "/fileprint.png"),*/ tr("&Print..."), this);
    a->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(a, SIGNAL(triggered()), this, SLOT(print()));
    tb->addAction(a);

    a = new QAction(this);
    a->setText(tr("Page Setup..."));
    connect(a, SIGNAL(triggered()), this, SLOT(pageSetup()));
    tb->addAction(a);

    tb->addSeparator();

    a = new QAction(QIcon(rsrcPath + "/zoomin.png"), tr("Zoom In"), this);
    connect(a, SIGNAL(triggered()), view, SLOT(zoomIn()));
    tb->addAction(a);

    a = new QAction(QIcon(rsrcPath + "/zoomout.png"), tr("Zoom Out"), this);
    connect(a, SIGNAL(triggered()), view, SLOT(zoomOut()));
    tb->addAction(a);

    tb->addSeparator();

    a = new QAction(this);
    a->setText(tr("&Close"));
    connect(a, SIGNAL(triggered()), this, SLOT(close()));
    tb->addAction(a);

    statusBar()->setSizeGripEnabled(true);
}

void PrintPreview::setup()
{
    QSizeF page = printer.pageRect().size();
    paperSize = printer.paperRect().size();
    paperSize.rwidth() *= qreal(view->logicalDpiX()) / printer.logicalDpiX();
    paperSize.rheight() *= qreal(view->logicalDpiY()) / printer.logicalDpiY();

    pageTopLeft = printer.pageRect().topLeft();
    pageTopLeft.rx() *= qreal(view->logicalDpiX()) / printer.logicalDpiX();
    pageTopLeft.ry() *= qreal(view->logicalDpiY()) / printer.logicalDpiY();

    page.setWidth(page.width() * view->logicalDpiX() / printer.logicalDpiX());
    page.setHeight(page.height() * view->logicalDpiY() / printer.logicalDpiY());

    // add a nice 2 cm margin
    const qreal margin = inchesToPixels(mmToInches(20), this);
    QTextFrameFormat fmt = doc->rootFrame()->frameFormat();
    fmt.setMargin(margin);
    doc->rootFrame()->setFrameFormat(fmt);

    doc->setPageSize(page);
}

PrintPreview::~PrintPreview()
{
    delete doc;
}

void PrintPreview::print()
{
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() == QDialog::Accepted) {
        doc->print(&printer);
    }
    delete dlg;
}

void PrintPreview::pageSetup()
{
    QPageSetupDialog dlg(&printer, this);
    if (dlg.exec() == QDialog::Accepted) {
        setup();
        view->updateLayout();
    }
}

#include "printpreview.moc"
