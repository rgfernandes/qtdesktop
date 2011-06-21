#include <QApplication>
#include <QFSFileEngine>
#include <QAbstractFileEngine>
#include <QAbstractFileEngineHandler>
#include <QFileDialog>

class ZipEngine : public QFSFileEngine
{
    public:
        ZipEngine( const QString & fn ) : QFSFileEngine( fn ){
            setFileName( "ZIP FILE" );
        }
};

class ZipEngineHandler : public QAbstractFileEngineHandler
{
    public:
        QAbstractFileEngine *create(const QString &fileName) const;
};

QAbstractFileEngine *ZipEngineHandler::create(const QString &fileName) const
{
    // ZipEngineHandler returns a ZipEngine for all .zip files
    return fileName.toLower().endsWith(".zip") ? new ZipEngine(fileName) : 0;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ZipEngineHandler engine;

    QFileDialog fd;
    fd.show();

    return app.exec();
}