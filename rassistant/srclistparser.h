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

/* This parser parse files with structure like:
<raproject-sources version="1.0">
<source title="Book Name" author="Book Author" comment="First Edition">
    <link comment="local">../../sources/book.html</link>
    <link comment="remote">http://ya.ru</link>
</source>
<source title="Another Book" author="Another Author" comment="Only Edition">
    <link comment="local source">/var/share/docs/sources/book2.html</link>
    <link comment="remote location">http://pm4u.opennet.ru</link>
    <link comment="second local ">folder</link>
</source>
</raproject-sources>
*/
#ifndef __SrcListParser_H__
#define __SrcListParser_H__

#include <QtXml>
#include <QList>
#include <QMap>

struct LnkItem { //structure of link in source
	//--- constructors ---
    LnkItem() : id(0), src_id(0), comment(QString()), link(QString()) {}
	LnkItem(int a, int n, const QString c, const QString l) : id(a), src_id(n), comment(c), link(l) {} //set variables: src_id = n, comment = c, etc.
	
	//--- operators ---
	bool operator==(LnkItem &other) const {  
		return ((src_id == other.src_id) && (comment == other.comment) && (link == other.link));
	}
	
	//--- variables ---
	int id; //autocounter
	int src_id; //to link with srcItem
	QString comment;
    QString link;
};

struct SrcItem { //structure of source
    SrcItem() 
	: id(0), title(QString()), author(QString()), comment(QString()) {}		//set variables to default values (initialize). Not using yet
    SrcItem( int n, const QString &t, const QString &a, const QString &c )	
	: id(n), title(t), author(a), comment(c) {}								//set variables to passed values
    int id;		//to link with lnkList
	QString title;
    QString author;
	QString comment;

    //Q_DUMMY_COMPARISON_OPERATOR(SrcItem) ?+? what this is for?
};

//===================== class SrcListParser =====================
class SrcListParser : public QXmlDefaultHandler //parser for Research Assistant Projects' source list
{
public:
    SrcListParser();    
	static SrcListParser *createParser( const QString &fileName );

    bool parse();
    QString errorProtocol() const { return errorString; }
	void setFileName(QString fn)  { fName = fn; }
    
    QList<SrcItem> getSourceItems()	{ return srcList; }
    QList<LnkItem> getLinkItems()	{ return lnkList; }

    // parse xml file with SAX method. Procedures are called when parser finds one of <element> charecters </element>
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& ); 
    bool endElement( const QString&, const QString&, const QString& );
    bool characters( const QString & );
    bool fatalError( const QXmlParseException& exception );

protected:
    QString errorString;
	QString srcTitle, srcAuthor, srcComment;
    QString lnkComment, lnkRef;
	int srcID, lnkID;
    QList<SrcItem> srcList;
    QList<LnkItem> lnkList;


private:
    QString fName, curTag;	
};

#endif // __SrcListParser_H__
