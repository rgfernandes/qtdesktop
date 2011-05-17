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
#ifndef DOCUPARSER_H
#define DOCUPARSER_H

#include <QtXml>
#include <QList>
//#include <QMap>

class Profile;

struct ContentItem {
    ContentItem() 
	: title( QString() ), reference( QString() ), iconFN( QString() ), depth( 0 ) {}
    ContentItem( const QString &t, const QString &r, const QString &i, int d )
	: title( t ), reference( r ), iconFN( i ), depth( d ) {}
    QString title;
    QString reference;
	QString iconFN;
    int depth;
    Q_DUMMY_COMPARISON_OPERATOR(ContentItem)
};

QDataStream &operator>>( QDataStream &s, ContentItem &ci );
QDataStream &operator<<( QDataStream &s, const ContentItem &ci );

struct IndexItem {
    IndexItem( const QString &k, const QString &r )
	: keyword( k ), reference( r ) {}
    QString keyword;
    QString reference;
};

//===================== class DocuParser =====================
class DocuParser : public QXmlDefaultHandler //To parse file with profile
{
public:
    static DocuParser *createParser( const QString &fileName );

    virtual bool parse( QFile *file );
    
    QList<ContentItem> getContentItems();
    QList<IndexItem*> getIndexItems();

    QString errorProtocol() const;
    
protected:
    //QString absolutify( const QString &input, bool makeUrl = true ) const;
    
    QString contentRef, indexRef, indexKey, errorProt;
    QString docTitle, title, iconFileName;
    QList<ContentItem> contentList;
    QList<IndexItem*> indexList;
    QString fname;
};

//===================== class DocuParserRAP =====================
class DocuParserRAP : public DocuParser //parser for Research Assistant Project
{
public:
    enum States { StateInit, StateProfile, StateProperty, StateContents, StateSection,
				  StateKeywords, StateKeyword };

    DocuParserRAP();    
    // parse xml file with SAX method. Procedures are called when pareser finds one of <element> charecters </element>
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& ); 
    bool endElement( const QString&, const QString&, const QString& );
    bool characters( const QString & );
    bool fatalError( const QXmlParseException& exception );

    //virtual void addTo( Profile *p );
    Profile *profile() const { return prof; }    

private:
    
    States state;
    int depth;
    QString propertyValue;
    QString propertyName;
    Profile *prof;
	
};

#endif // DOCUPARSER_H
