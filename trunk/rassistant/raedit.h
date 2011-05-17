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
#ifndef QRAEDITOR_H
#define QRAEDITOR_H

#include <QtGui/qtextedit.h>
#include <QtCore/qurl.h>

QT_MODULE(Gui)
//====================== class raEdit ============================
class raEdit : public QTextEdit  //this is a remake of QTextBrowser
{
	//The Q_OBJECT macro must appear in the private section of a class definition that declares its own signals and slots or that uses other services provided by Qt's meta-object system.
    Q_OBJECT

	//This macro is used for declaring properties in classes that inherit QObject. Properties behave like class data members, but they have additional features accessible through the Meta-Object System.
	//qthelp://com.trolltech.qt.442/qdoc/properties.html
    Q_PROPERTY(QUrl source READ source WRITE setSource)
	
public:
    explicit raEdit(QWidget* parent = 0);
    virtual ~raEdit();

    QUrl source() const;
    void setSearchPaths(const QStringList &paths);
    virtual QVariant loadResource(int type, const QUrl &name);
    void setOpenExternalLinks(bool open){ openExternalLinks = open; }
    void setOpenLinks(bool open)		{ openLinks = open; }

//-pm- following brought from class raEditPrivate : public QTextEditPrivate
//-pm- to avoid the use of additional .h files such as qtextedit_p.h
    QUrl currentURL;
    QStringList searchPaths;

    /*flag necessary to give the linkClicked() signal some meaningful
      semantics when somebody connected to it calls setText() or setSource() */
    bool textOrSourceChanged;
    bool forceLoadOnSourceChange;
    bool openExternalLinks;
    bool openLinks;

//#ifndef QT_NO_CURSOR
    //QCursor oldCursor;
//#endif
    QString findFile(const QUrl &name) const;

    void _q_activateAnchor(const QString &href);
    void _q_highlightLink(const QString &href);

    void setSource(const QUrl &url);
    QUrl resolveUrl(const QUrl &url) const;
	//-pm- end of raEditPrivate section

	//bool ModeHtml() { return modeHtml; }
	//void setModeHtml(bool b) { modeHtml = b; }

	bool canInsertFromMimeData( const QMimeData *source ) const;

signals:
    void sourceChanged(const QUrl &);
    void highlighted(const QUrl &);
    void highlighted(const QString &);
    void anchorClicked(const QUrl &);
	void insertImageFromClipboard(QImage image);
	void insertHtmlFromClipboard(QString html);

public slots:
   // virtual void setSource(const QUrl &name);
    virtual void reload();
    inline void _q_documentModified()
    {
        textOrSourceChanged = true;
        forceLoadOnSourceChange = !currentURL.path().isEmpty();
    }
	void insertFromMimeData( const QMimeData *source );

protected:
    bool event(QEvent *e);
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void focusOutEvent(QFocusEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    //virtual bool focusNextPrevChild(bool next);
	// virtual void paintEvent(QPaintEvent *e);
private:
	//bool modeHtml;
	QTextDocument *doc;

};

#endif // QRAEDITOR_H
