#ifndef QMPQFILEENGINE_H
#define QMPQFILEENGINE_H

#include "QMPQFileEngine_global.h"

#include <QtCore/QAbstractFileEngine>
#include <QDebug>

class QMPQArchiveEx;
class QMPQFileEnginePrivate;
class QMPQFILEENGINESHARED_EXPORT QMPQFileEngine : public QAbstractFileEngine
{
    Q_DECLARE_PRIVATE(QMPQFileEngine);
public:
    QMPQFileEngine();
    QMPQFileEngine(const QString & file);
    ~QMPQFileEngine();

    QMPQArchiveEx * archive();
    Iterator * beginEntryList(QDir::Filters filters, const QStringList & filterNames);
    bool caseSensitive () const;
    bool close();
    bool copy(const QString & newName);
    QStringList	entryList(QDir::Filters filters, const QStringList & filterNames) const;
    FileFlags fileFlags(FileFlags type = FileInfoAll) const ;
    QString fileName(FileName file = DefaultName) const;
    QDateTime fileTime(FileTime time) const;
    bool flush();
    bool isRelativePath() const;
    bool isCreated();
//    bool isSequential() const { return true; } // Why works with it???
    bool mkdir(const QString & dirName, bool createParentDirectories) const;
    bool open(QIODevice::OpenMode mode);
    QString owner(FileOwner owner) const;
    uint ownerId(FileOwner owner) const;
    qint64 pos() const;
    qint64 read(char* data, qint64 maxlen);
    bool seek(qint64 offset);
    qint64 size() const;
    bool remove();
    bool rename(const QString & newName);
    bool rmdir(const QString & dirName, bool recurseParentDirectories) const;
    void setFileName(const QString & file);
    bool setPermissions(uint perms);
    bool setSize(qint64 size);
    qint64 write(const char * data, qint64 len);

    static QString localeNameFromName(const QString name);
    static QLocale localeFromName(const QString name);
    static QString fileNameFromName(const QString name);

private:
    void initArchive();

protected:
    QMPQFileEnginePrivate * d_ptr;
};

#endif // QMPQFILEENGINE_H
