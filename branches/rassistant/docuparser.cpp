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
#include "docuparser.h"
//#include "profile.h"
#include "config.h"
#include "pcommon.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QString>
#include <QtXml>
#include <QDataStream>
#include <QtDebug> //to use qWarning and qDebug messages


//-------------------------------------------------
QDataStream &operator>>(QDataStream &s, ContentItem &ci)
{
    s >> ci.title;
    s >> ci.reference;
    s >> ci.iconFN;
    s >> ci.depth;
    return s;
}

//-------------------------------------------------
QDataStream &operator<<(QDataStream &s, const ContentItem &ci)
{
    s << ci.title;
    s << ci.reference;
    s << ci.iconFN;
    s << ci.depth;
    return s;
}

//-------------------------------------------------
DocuParser *DocuParser::createParser(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)) {
        return 0;
    }
	
    return new DocuParserRAP;
}

//===================== class DocuParser =====================
bool DocuParser::parse(QFile *file)
{
    QXmlInputSource source(file);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    fname = QFileInfo(*file).absoluteFilePath();
    return reader.parse(source);
}

//-------------------------------------------------
QString DocuParser::errorProtocol() const {    return errorProt; }

QList<ContentItem> DocuParser::getContentItems(){  return contentList; }
QList<IndexItem*> DocuParser::getIndexItems(){    return indexList; }

DocuParserRAP::DocuParserRAP()
    : prof(new Profile)
{
}

//===================== class DocuParserRAP =====================
bool DocuParserRAP::startDocument()
{
    state = StateInit;
    errorProt = QLatin1String("");
    contentRef = QLatin1String("");
    indexRef = QLatin1String("");
	indexKey = QLatin1String("");
    depth = 0;
    contentList.clear();
    indexList.clear();

    return true;
}

//-------------------------------------------------
bool DocuParserRAP::startElement(const QString &, const QString &, 
								 const QString &qname, const QXmlAttributes &attr)
{
    QString lower = qname.toLower();

    switch(state) {

    case StateInit:
        if(lower == QLatin1String("raproject"))
			state = StateProfile;
        break;

	case StateProfile:
        if(lower == QLatin1String("property")) {
            state = StateProperty;
            propertyName = attr.value(QLatin1String("name"));
        }
        break;

	case StateProperty: //values are set at the endElement tag </property>
        break;

	case StateContents: 
		if(lower == QLatin1String("contents")) 
			state = StateSection;
        break;

    case StateSection:
        if(lower == QLatin1String("section")) {
            docTitle = attr.value(QLatin1String("title"));
            contentRef = urlifyFileName(absolutifyFileName(attr.value(QLatin1String("ref")), Config::configuration()->CurPrjDir()));
            iconFileName = absolutifyFileName(attr.value(QLatin1String("icon")),Config::configuration()->CurPrjDir());
			contentList.append(ContentItem(docTitle, contentRef, iconFileName, depth));
			depth++; 
        }
        break;

	case StateKeywords: 
        if (lower == QLatin1String("keyword")) {
            indexRef = absolutifyFileName(attr.value(QLatin1String("ref")), Config::configuration()->CurPrjDir());
			indexKey = attr.value(QLatin1String("key"));
			state = StateKeyword;
        } 
        break;

	case StateKeyword:
		break;

    default:
        break;
    }

    return true;
}

//-------------------------------------------------
bool DocuParserRAP::endElement(const QString &nameSpace, const QString &localName, const QString &qName)
{
    Q_UNUSED(nameSpace);
    Q_UNUSED(localName);
    Q_UNUSED(qName);

    switch(state) {
    case StateInit:
        break;
    case StateProfile:
		state = StateContents;
        break;
    case StateProperty:
        state = StateProfile;
        if(propertyName.isEmpty() || propertyValue.isEmpty()){
            return false;
        }else{
            static const QStringList lst = QStringList()  // lst is a list of properties with file names
				<< QLatin1String("startpage");
            if (lst.contains(propertyName)){ //see if propertyValue is a file name then convert string to full path name
                propertyValue = absolutifyFileName(propertyValue, Config::configuration()->CurPrjDir());
			}
        }
        prof->addProperty(propertyName, propertyValue);
        break;
	case StateSection:
        if(contentRef.isEmpty())  return false;
		depth--;
		break;
	case StateContents:
		state = StateKeywords; 
        break;
    case StateKeyword:
		state = StateKeywords;
		if(indexRef.isEmpty())  return false;
        break;
    case StateKeywords:
        state = StateInit;
        break;
    }
    return true;
}

//-------------------------------------------------
bool DocuParserRAP::characters(const QString& ch)
{
    QString str = ch.simplified();
    if (str.isEmpty())
        return true;

    switch (state) {
    case StateInit:
    case StateContents:
    case StateSection:
    case StateKeywords:
        return false; //no value for these tags
        break;
    case StateKeyword:
        indexList.append(new IndexItem(str, indexRef)); // !+! need to add indexKey for future use
        break;
    case StateProperty:
        propertyValue = ch;
        break;
    default:
        return false;
    }
    return true;
}

//-------------------------------------------------
bool DocuParserRAP::fatalError(const QXmlParseException& exception)
{
    errorProt += QString::fromLatin1("fatal parsing error: %1 in line %2, column %3\n")
        .arg(exception.message())
        .arg(exception.lineNumber())
        .arg(exception.columnNumber());

    return QXmlDefaultHandler::fatalError(exception);
}
