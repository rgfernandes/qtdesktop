#ifndef XTRAZUTILS_H
#define XTRAZUTILS_H

#include "xtrazpackage.h"

class QDomElement;

class XtrazUtils
{
public:
    static QList<XtrazPackage*> FindPackages();
protected:
    static QList<XtrazPackage*> ProccessPackages(const QString& aPaths);

    XtrazUtils();
};

#endif // XTRAZUTILS_H
