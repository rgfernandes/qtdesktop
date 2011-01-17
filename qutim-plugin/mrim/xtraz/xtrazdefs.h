#ifndef XTRAZDEFS_H
#define XTRAZDEFS_H

#include <QRegExp>
#include <QHash>

enum SmileType
{
    StaticSmile = 0,
    FlashSmile,
    GifSmile
};

struct XtrazSmile
{
    QHash<QString,QString> Names;
    SmileType Type;
    QString FileName;
    QList<QRegExp> Expressions;
    QString Replacement;
};

#endif // XTRAZDEFS_H
