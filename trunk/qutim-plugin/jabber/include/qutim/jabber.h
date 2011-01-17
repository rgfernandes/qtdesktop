/*!
 * Jabber Plugin SDK
 *
 * @author Nigmatullin Ruslan
 * Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
 * encoding: UTF-8 
 *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
 */

#ifndef QUTIM_JABBER_SDK_H
#define QUTIM_JABBER_SDK_H

#include <QMultiMap>
#include <QString>
#include <QList>
#include <QStringList>

namespace qutim_sdk_jabber {

enum EventToJabber
{
	SetStatus = 0,
	RestoreStatus,
	SetMood,
	SetActivity,
	SetTune
};

enum Status
{
	Simple = -1, /*!< do not change */
	Online = 0,
	FreeForChat = 1,
	Away = 2,
	NA = 3,
	DND = 5,
	Invisible = 6,
	Offline = 12,
	Connecting = 13
};

class QTag;

typedef QList<QTag *> QTagList;
typedef QList<const QTag *> QConstTagList;

class QTag
{
public:
    virtual QTag *newInstance(const QString &name, const QString &cdata = "") = 0;
    virtual QTag *newInstance(QTag *parent, const QString &name, const QString &cdata = "") = 0;
    virtual QTag *newInstance(const QString &name, const QString &attrib, const QString &value) = 0;
    virtual QTag *newInstance(QTag *parent, const QString &name, const QString &attrib, const QString &value) = 0;
//    void *operator new(size_t, const QString &name, const QString &cdata = "")
//    {
//        return(qTagInstance?qTagInstance->newInstance(name, cdata):0);
//    }
//    void *operator new(size_t, QTag *parent, const QString &name, const QString &cdata = "")
//    {
//        return(qTagInstance?qTagInstance->newInstance(parent, name, cdata):0);
//    }
//    void *operator new(size_t, const QString &name, const QString &attrib, const QString &value)
//    {
//        return(qTagInstance?qTagInstance->newInstance(name, attrib, value):0);
//    }
//    void *operator new(size_t, QTag *parent, const QString &name, const QString &attrib, const QString &value)
//    {
//        return(qTagInstance?qTagInstance->newInstance(parent, name, attrib, value):0);
//    }
    virtual const QString xml() = 0;
    virtual bool setPrefix(const QString &xmlns) = 0;
    virtual const QString prefix() const = 0;
    virtual const QString prefix(const QString &xmlns) = 0;
    virtual bool setXmlns (const QString &xmlns, const QString &prefix = "") = 0;
    virtual const QString xmlns () const = 0;
    virtual const QString xmlns (const QString &prefix) const = 0;
    virtual bool addAttribute(const QString &name, const QString &value) = 0;
    virtual void addChild(QTag *child) = 0;
    virtual void addChildCopy(const QTag *child) = 0;
    virtual bool setCData (const QString &cdata) = 0;
    virtual bool addCData (const QString &cdata) = 0;
    virtual const QString name() const = 0;
    virtual const QString cdata() const = 0;
    virtual const QTagList children() const = 0;
    virtual const QString findAttribute (const QString &name) const = 0;
    virtual bool hasAttribute (const QString &name, const QString &value= "") const = 0;
    virtual QTag *findChild (const QString &name) const = 0;
    virtual QTag *findChild (const QString &name, const QString &attr, const QString &value= "") const = 0;
    virtual bool hasChild (const QString &name, const QString &attr= "", const QString &value= "") const = 0;
    virtual QTag *findChildWithAttrib (const QString &attr, const QString &value= "") const = 0;
    virtual bool hasChildWithAttrib (const QString &attr, const QString &value= "") const = 0;
    virtual QTagList findChildren (const QString &name, const QString &xmlns= "") const = 0;
    virtual void removeChild (const QString &name, const QString &xmlns= "") = 0;
    virtual void removeChild (QTag *tag) = 0;
    virtual void removeAttribute (const QString &attr, const QString &value= "", const QString &xmlns= "") = 0;
    virtual bool hasChildWithCData (const QString &name, const QString &cdata) const = 0;
    virtual QTag *parent() const = 0;
    virtual QTag *clone() const = 0;
    virtual const QString findCData (const QString &expression) const = 0;
    virtual const QTag *findTag (const QString &expression) const = 0;
    virtual QConstTagList findTagList (const QString &expression) const = 0;
    virtual operator bool() const = 0;
private:
    static QTag *qTagInstance;
};

struct QDataFormField
{
    enum Type
    {
        Boolean, Fixed, Hidden, JidMulti,
        JidSingle, ListMulti, ListSingle, TextMulti,
        TextPrivate, TextSingle, None, Invalid
    };
    QMultiMap<QString, QString> options;
    QString name;
    bool required;
    QString label;
    QString description;
    QString value;
    QStringList values;
    quint8 type;
};

struct QDataFormContainer
{
    QList<QDataFormField> fields;
};

struct QDataForm : QDataFormContainer
{
    enum Type
    {
        Form, Submit, Cancel,
        Result, Invalid
    };
    QString title;
    QStringList instructions;
    QList<QDataFormContainer> items;
    quint8 type;
};

} //end namespace qutim_sdk_jabber

#endif //QUTIM_JABBER_SDK_H
