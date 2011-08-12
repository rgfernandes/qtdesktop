#include "mpqfileengineiterator.h"

//#include "treeitem.h"

#include <QtCore/QDebug>

//MPQFileEngineIterator::MPQFileEngineIterator(QDir::Filters filters, const QStringList & nameFilters, TreeItem * item)
//    : QAbstractFileEngineIterator(filters, nameFilters), index(-1)
//{
////    qDebug() << "MPQFileEngineIterator" << item;
//    m_item = item;
////    entries << "entry1" << "entry2" << "entry3";
//}

MPQFileEngineIterator::MPQFileEngineIterator(QDir::Filters filters, const QStringList & nameFilters, const QStringList &files)
    : QAbstractFileEngineIterator(filters, nameFilters), index(-1)
{
    entries = files;
}

MPQFileEngineIterator::~MPQFileEngineIterator()
{
}

//QFileInfo MPQFileEngineIterator::currentFileInfo() const
//{
//    qDebug() << "MPQFileEngineIterator::currentFileInfo" << currentFilePath();
//
////    ((QObject*)0)->setObjectName("lol");
////    qDebug() << QAbstractFileEngineIterator::currentFileInfo().dir();
//    return QAbstractFileEngineIterator::currentFileInfo();
//}

bool MPQFileEngineIterator::hasNext() const
{
    return index + 1 < entries.count();
//    qDebug() << "MPQFileEngineIterator::hasNext";
//    return m_item && index < m_item->childCount() - 1;
}

QString MPQFileEngineIterator::next()
{
//    qDebug() << "MPQFileEngineIterator::next";
    if (!hasNext())
       return QString();
   ++index;
//   qDebug() << "MPQFileEngineIterator::next" << currentFilePath();
   return currentFileName();
}

QString MPQFileEngineIterator::currentFileName() const
{
    return entries.at(index);
//    qDebug() << "MPQFileEngineIterator::currentFileName" << m_item->childItems.at(index)->data(3).toString();
//    return m_item->childItems.at(index)->data(0).toString();
}
