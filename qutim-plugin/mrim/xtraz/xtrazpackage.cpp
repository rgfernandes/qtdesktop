#include "xtrazpackage.h"
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QTextDocument>
#include <qdebug.h>

XtrazPackage::XtrazPackage(QString aPackageName, QString aFullPath)
        : m_packageName(aPackageName), m_fullPath(aFullPath)
{
    qDebug()<<"Initializing package "<<m_packageName<< " at path: "<<m_fullPath;
}

XtrazPackage* XtrazPackage::TryParse(QString aPackageFilePath)
{    
    QFile docFile(aPackageFilePath);
    if (!docFile.open(QIODevice::ReadOnly)) return 0;
    qDebug()<<"Trying to parse xtraz package in: "<<aPackageFilePath;
    QDomDocument packageDoc;
    if (!packageDoc.setContent(&docFile))
    {
        qDebug()<<"Package is corrupted!";
        docFile.close();
        return 0;
    }
    if (packageDoc.isNull()) return 0;
    QDomElement rootEl = packageDoc.firstChildElement("xtrazpack");
    if (rootEl.isNull()) return 0;
    QString packageName = rootEl.attribute("name");
    qDebug()<<"Found package: "<<packageName;
    QDir packageDir(aPackageFilePath);
    QString fullPath = packageDir.absolutePath();
    XtrazPackage* package = new XtrazPackage(packageName,fullPath);
    package->ParseSmiles(rootEl.firstChildElement("smiles"));
    docFile.close();
    return package;
}

void XtrazPackage::ParseSmiles(const QDomElement& aSmilesElement)
{
    QList<XtrazSmile*> smiles;
    if (aSmilesElement.isNull()) return;
    for(QDomNode smileNode = aSmilesElement.firstChild(); !smileNode.isNull(); smileNode = smileNode.nextSibling())
    {
        if (smileNode.nodeName() != "smile") continue;
        XtrazSmile* smile = new XtrazSmile;
        smile->Type = (SmileType)smileNode.firstChildElement("type").text().toUInt();
        smile->FileName = m_fullPath+"/"+smileNode.firstChildElement("file").text();

        for(QDomElement nameEl = smileNode.firstChildElement("name"); !nameEl.isNull(); nameEl = nameEl.nextSiblingElement("name"))
        {
            qDebug()<<QString("Name for smile found (lang '%1'): %2").arg(nameEl.attribute("lang")).arg(nameEl.text());
            smile->Names.insert(nameEl.attribute("lang"),nameEl.text());
        }

        for(QDomElement expEl = smileNode.firstChildElement("exp"); !expEl.isNull(); expEl = expEl.nextSiblingElement("exp"))
        {
            qDebug()<<"Expression for smile found: "<<Qt::escape(expEl.text());
            smile->Expressions.append(QRegExp(Qt::escape(expEl.text())));
        }
        smile->Replacement = smileNode.firstChildElement("replace").text();
        smiles.append(smile);
    }
    qDebug()<<"Smiles found in package! Count: "<<smiles.count();
    AddSmiles(smiles);
}

void XtrazPackage::AddSmiles(QList<XtrazSmile*> aSmilesList)
{
    m_smilesList.append(aSmilesList);
}

void XtrazPackage::FindSmilesAndReplace(QString& aText)
{
    qDebug()<<"Finding smile tags in text: "<<aText;
    foreach (XtrazSmile* smile, m_smilesList)
    {
        foreach (QRegExp rx, smile->Expressions)
        {
            if (rx.indexIn(aText) >= 0)
            {
                qDebug()<<QString("Replacing '%1' with '%2'").arg(rx.cap(0)).arg(smile->Replacement);
                aText.replace(rx.cap(0),smile->Replacement.arg(m_fullPath.section('/',0,-2)+'/'));
            }
        }
    }
    qDebug()<<"Text after processing: "<<aText;
}
