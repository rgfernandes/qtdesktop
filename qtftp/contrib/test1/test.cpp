#include "test.h"

FtpEngineIterator::FtpEngineIterator(const QString& parent, QDir::Filters filters, const QStringList & filterNames) 
        : QAbstractFileEngineIterator(filters, filterNames),
	index(-1) { 
	entries << ".." << "."; 
	if (parent == "/bup") { 
	    entries << "A" << "B" << "C"; 
	} 
} 

QString FtpEngineIterator::next() { 
        index++; 
        return currentFileName(); 
} 

bool FtpEngineIterator::hasNext() const { 
        return index + 1 < entries.size(); 
} 

QString FtpEngineIterator::currentFileName() const { 
        return (index < entries.size()) ? entries[index] : QString(); 
} 

FtpEngine::FtpEngine(const QString& path) :QAbstractFileEngine() { 
        this->path = path; 
}

QAbstractFileEngineIterator* FtpEngine::beginEntryList (QDir::Filters filters, const QStringList & filterNames ) { 
        qDebug() << "beginEntryList"; 
        return new FtpEngineIterator(path, filters, filterNames); 
} 

QString FtpEngine::fileName (FileName file ) const { 
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

void FtpEngine::setFileName(const QString & file) { 
	path = file; 
} 

QAbstractFileEngine* FtpEngineHandler::create(const QString& path) const {
	return path.startsWith("/bup") ? new FtpEngine(path) : NULL; 
} 

int main() { 
	FtpEngineHandler FtpEnginehandler; 
	QDir d("/bup"); 
	foreach(const QString& name, d.entryList()) 
		qDebug() << name; 
	qDebug() << "entryList: " + QString::number(d.entryList().size()); 
	return 0; 
} 