#include "qmpqfileenginehandler.h"

#include "qmpqfileengine.h"
#include "qmpqarchivecache.h"

#include <QtCore/QDebug>

QStringList suffixes;

QStringList QMPQFileEngineHandler::suffixes()
{
    return ::suffixes;
}

void QMPQFileEngineHandler::setSufixes(QStringList suffixes)
{
    ::suffixes.clear();
    foreach (QString suffix, suffixes) {
        ::suffixes.append('.' + suffix);
    }
}

QMPQFileEngineHandler::QMPQFileEngineHandler() : QAbstractFileEngineHandler()
{
    QMPQArchiveCache::init();
    ::suffixes << "mpq" << "w3x" << "w3m" << "s2ma" << "SC2Data" << "SC2Archive" << "SC2Assets"
                    << "SC2Replay" << "scx" << "w3n" << "snp" << "sv" << "hsv";
}

QAbstractFileEngine *QMPQFileEngineHandler::create(const QString &fileName) const
{
    if (fileName.startsWith("mpq:")) {
        QMPQFileEngine * engine = new QMPQFileEngine;
        engine->setFileName(fileName);
        if (!engine->isCreated()) {
            delete engine;
            return 0;
        }

        return engine;
    }
    return 0;
}

