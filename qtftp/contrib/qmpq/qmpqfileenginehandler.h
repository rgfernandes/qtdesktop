#ifndef QMPQFILEENGINEHANDLER_H
#define QMPQFILEENGINEHANDLER_H

#include "QMPQFileEngine_global.h"

#include <QAbstractFileEngineHandler>
#include <QString>

class QMPQFILEENGINESHARED_EXPORT QMPQFileEngineHandler : public QAbstractFileEngineHandler
{
public:
    QMPQFileEngineHandler();
    QAbstractFileEngine *create(const QString &fileName) const;
    static QStringList suffixes();
    static void setSufixes(QStringList suffixes);
};

#endif // QMPQFILEENGINEHANDLER_H
