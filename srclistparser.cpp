/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#include "srclistparser.h"
#include "pcommon.h"

#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QtXml>
#include <QtDebug> //to use qWarning and qDebug messages

//===================== class SrcListParser =====================
SrcListParser::SrcListParser()
{
}

//-------------------------------------------------
SrcListParser *SrcListParser::createParser(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)) {
        return 0;
    }
	SrcListParser *parser = new SrcListParser;
   	parser->setFileName(fileName);
    return parser; 
}

//-------------------------------------------------
bool SrcListParser::parse()
{	
	QFile file(fName);
	QXmlInputSource source(&file);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
	bool p = reader.parse(source);
    return p;
}

//-------------------------------------------------
bool SrcListParser::startDocument()
{
    errorString = QLatin1String("");
    srcID = 0;	srcTitle = QLatin1String("");	srcAuthor = QLatin1String("");	srcComment = QLatin1String("");
    lnkID = 0;	lnkComment = QLatin1String("");	lnkRef = QLatin1String("");
    srcList.clear();
    lnkList.clear();

    return true;
}

//-------------------------------------------------
bool SrcListParser::startElement(const QString &, const QString &, const QString &qname, const QXmlAttributes &attr)
{
    QString lower = qname.toLower();
	curTag = lower;
	if(lower == QLatin1String("source")) {
		srcID++;
        srcTitle = attr.value(QLatin1String("title"));
		srcAuthor = attr.value(QLatin1String("author"));
		srcComment = attr.value(QLatin1String("comment"));
//		qDebug() << "title = " << srcTitle << ", author= " << srcAuthor << ", comment= " << srcComment << ", ID= " << srcID;
	}else if(lower == QLatin1String("link")) {
		lnkID++;
		lnkComment = attr.value(QLatin1String("comment"));
	}
	
    return true;
}

//-------------------------------------------------
bool SrcListParser::endElement(const QString &nameSpace, const QString &localName, const QString &qName)
{
    Q_UNUSED(nameSpace);// These variables required for the function, but we don't use them. 
    Q_UNUSED(localName);// So tell compiler to not warn about this.

    QString lower = qName.toLower();
	if(lower == QLatin1String("source")) {
		srcList.append(SrcItem(srcID, srcTitle, srcAuthor, srcComment));
	}
    return true;
}

//-------------------------------------------------
bool SrcListParser::characters(const QString& ch)
{
    QString str = ch.simplified();
    if (str.isEmpty())  //ignore link if it is empty
        return true;

	if(curTag == QLatin1String("link")){
			lnkRef = str;
			lnkList.append(LnkItem(lnkID, srcID, lnkComment, lnkRef));			
//			qDebug() << "link comment = " << lnkComment << ", ref= " << lnkRef << ", srcID= " << srcID;
	}else return false;

    return true;
}

//-------------------------------------------------
bool SrcListParser::fatalError(const QXmlParseException& exception)
{
    errorString += QString::fromLatin1("Fatal error encountered during parsing sources list: %1 in line %2, column %3")
        .arg(exception.message())
        .arg(exception.lineNumber())
        .arg(exception.columnNumber());

    return QXmlDefaultHandler::fatalError(exception);
}
