#ifndef QMPQFILEENGINE_P_H
#define QMPQFILEENGINE_P_H

#include <QtCore/QString>
#include "qmpqarchiveex.h"

class QMPQFileEnginePrivate
{
    Q_DECLARE_PUBLIC(QMPQFileEngine)

    QString archiveFilePath; // Path to archive itself
    QString innerPath; // Path within the archive
    QString localeName;

    QString fileName; // The same filename that was passed to the QAbstractFileEngine.
//    QString filePath; // The path to the file excluding the base name.
    QString filePath; // The path to the file including the base name.
    QString baseName; // The name of the file excluding the path.
    QMPQArchiveEx * archive;
//    QBuffer buffer;
    QIODevice::OpenMode openMode;
    uint mappedCnt;
    qint64 offset;
    QByteArray fileData;
    bool isCreated;

    QString getArchiveFilePath(const QString & path);

public:
    QMPQFileEnginePrivate(QMPQFileEngine * q)
        : archive(0), openMode(QIODevice::NotOpen), offset(0), isCreated(false)
    {
        q_ptr = q;
    }
protected:
    QMPQFileEngine *q_ptr;
};

#endif // QMPQFILEENGINE_P_H
