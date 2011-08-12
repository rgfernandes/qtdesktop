#ifndef MPQFILEENGINEITERATOR_H
#define MPQFILEENGINEITERATOR_H

#include <QAbstractFileEngineIterator>

class TreeItem;

class MPQFileEngineIterator : public QAbstractFileEngineIterator
{
public:
    MPQFileEngineIterator(QDir::Filters filters, const QStringList & nameFilters, TreeItem * item = 0);
    MPQFileEngineIterator(QDir::Filters filters, const QStringList & nameFilters, const QStringList &files);
    ~MPQFileEngineIterator();
//    QFileInfo currentFileInfo () const;
    QString currentFileName () const;
    bool hasNext () const;
    QString next();
private:
    TreeItem * m_item;
    QStringList entries;
    int index;
};

#endif // MPQFILEENGINEITERATOR_H
