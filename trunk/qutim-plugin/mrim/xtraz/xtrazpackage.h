#ifndef XTRAZPACKAGE_H
#define XTRAZPACKAGE_H

#include <QObject>
#include "xtrazdefs.h"

class QDomElement;

class XtrazPackage : public QObject
{    
public:
    static XtrazPackage* TryParse(QString aPackageFilePath);

    XtrazPackage(QString aPackageName, QString aFullPath);
    void AddSmiles(QList<XtrazSmile*> aSmilesList);
    void ParseSmiles(const QDomElement& aSmilesElement);
    void FindSmilesAndReplace(QString& aText);
    inline QString Name() { return m_packageName; }
    inline QString Path() { return m_fullPath; }
private:
    QList<XtrazSmile*> m_smilesList;
    QString m_packageName;
    QString m_fullPath;
};

#endif // XTRAZPACKAGE_H
