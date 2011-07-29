#include <QtCore>

class FtpEngineIterator : public QAbstractFileEngineIterator { 
public: 
    FtpEngineIterator(const QString& parent, QDir::Filters filters, const QStringList & filterNames) 
        : QAbstractFileEngineIterator(filters, filterNames),
	index(-1) { 
        entries << ".." << "."; 
        if (parent == "/bup") { 
            entries << "A" << "B" << "C"; 
        } 
    } 
    QString next() { 
        index++; 
        return currentFileName(); 
    } 
    bool hasNext() const { 
        return index + 1 < entries.size(); 
    } 
    QString currentFileName() const { 
        return (index < entries.size()) ? entries[index] : QString(); 
    } 
private: 
    QStringList entries; 
    int index; 
}; 

class FtpEngine : public QAbstractFileEngine { 
public: 
    FtpEngine(const QString& path) :QAbstractFileEngine() { 
        this->path = path; 
    } 
    ~FtpEngine() { 
    } 
    Iterator* beginEntryList (QDir::Filters filters, const QStringList & filterNames ) { 
        qDebug() << "beginEntryList"; 
        return new FtpEngineIterator(path, filters, filterNames); 
    } 
    virtual FileFlags fileFlags (FileFlags type = FileInfoAll ) const { 
        FileFlags flags = ExistsFlag | ReadOwnerPerm | ReadUserPerm | 
                ReadGroupPerm | ReadOtherPerm | DirectoryType; 
        return flags; 
    } 
    virtual QString fileName (FileName file = DefaultName ) const { 
        QString name; 
        switch (file) { 
        case DefaultName: 
            name = path; 
            break; 
        case BaseName: 
            name = path.section('/', -1); 
            break; 
        } 
        return name; 
    } 
    virtual void setFileName(const QString & file) { 
        path = file; 
    } 
private:
    QString path;
}; 

class FtpEngineHandler : public QAbstractFileEngineHandler { 
public: 
    FtpEngineHandler() :QAbstractFileEngineHandler() { 
    } 
    QAbstractFileEngine* create(const QString& path) const { 
        return path.startsWith("/bup") ?new FtpEngine(path) :NULL; 
    } 
}; 
int 
main() { 
    FtpEngineHandler FtpEnginehandler; 
    QDir d("/bup"); 
    foreach(const QString& name, d.entryList()) { 
        qDebug() << name; 
    } 
    qDebug() << "entryList: " + QString::number(d.entryList().size()); 
    return 0; 
} 