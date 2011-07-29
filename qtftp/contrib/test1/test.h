//#include <QtCore/QAbstractFileEngineHandler> 
//#include <QtCore/QAbstractFileEngine>
//#include <QtCore/QDateTime> 
//#include <QtCore/QDebug>
#include <QtCore>

class FtpEngineIterator : public QAbstractFileEngineIterator { 
public: 
	FtpEngineIterator(const QString &, QDir::Filters, const QStringList &);
	QString next();
	bool hasNext() const;
	QString currentFileName() const;
private: 
	QStringList entries; 
	int index; 
}; 

class FtpEngine : public QAbstractFileEngine {
public:
	FtpEngine(const QString &);
	~FtpEngine() { }
	Iterator* beginEntryList (QDir::Filters filters, const QStringList & filterNames );
	virtual FileFlags fileFlags (FileFlags type = FileInfoAll ) const { 
		return ExistsFlag | ReadOwnerPerm | ReadUserPerm | ReadGroupPerm | ReadOtherPerm | DirectoryType;
	}
	virtual QString fileName (FileName file = DefaultName ) const;
	virtual void setFileName(const QString & file);
private:
	QString path;
}; 

class FtpEngineHandler : public QAbstractFileEngineHandler { 
public:
	FtpEngineHandler()  : QAbstractFileEngineHandler() { }
	QAbstractFileEngine* create(const QString& path) const;
};
