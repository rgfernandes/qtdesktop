#ifdef QGLOOXTAG_H
#define QGLOOXTAG_H

#include "../include/qutim/jabber.h"
#include "utils.h"
#include <gloox/tag.h>

using namespace qutim_sdk_jabber;
using namespace gloox;

class QGlooxTag : public QTag
{
public:
	QGlooxTag(Tag *tag);
//    void *operator new(size_t, Tag *tag)
//    {
//        return(::new QGlooxTag(tag));
//    }
	Tag *glooxTag();
	virtual QTag *newInstance(const QString &name, const QString &cdata = "");
	virtual QTag *newInstance(QTag *parent, const QString &name, const QString &cdata = "");
	virtual QTag *newInstance(const QString &name, const QString &attrib, const QString &value);
	virtual QTag *newInstance(QTag *parent, const QString &name, const QString &attrib, const QString &value);
	virtual const QString xml();
	virtual bool setPrefix(const QString &xmlns);
	virtual const QString prefix() const;
	virtual const QString prefix(const QString &xmlns);
	virtual bool setXmlns (const QString &xmlns, const QString &prefix = "");
	virtual const QString xmlns () const;
	virtual const QString xmlns (const QString &prefix) const;
	virtual bool addAttribute(const QString &name, const QString &value);
	virtual void addChild(QTag *child);
	virtual void addChildCopy(const QTag *child);
	virtual bool setCData (const QString &cdata);
	virtual bool addCData (const QString &cdata);
	virtual const QString name() const;
	virtual const QString cdata() const;
	virtual const QTagList children() const;
	virtual const QString findAttribute (const QString &name) const;
	virtual bool hasAttribute (const QString &name, const QString &value= "") const;
	virtual QTag *findChild (const QString &name) const;
	virtual QTag *findChild (const QString &name, const QString &attr, const QString &value= "") const;
	virtual bool hasChild (const QString &name, const QString &attr= "", const QString &value= "") const;
	virtual QTag *findChildWithAttrib (const QString &attr, const QString &value= "") const;
	virtual bool hasChildWithAttrib (const QString &attr, const QString &value= "") const;
	virtual QTagList findChildren (const QString &name, const QString &xmlns= "") const;
	virtual void removeChild (const QString &name, const QString &xmlns= "");
	virtual void removeChild (QTag *tag);
	virtual void removeAttribute (const QString &attr, const QString &value= "", const QString &xmlns= "");
	virtual bool hasChildWithCData (const QString &name, const QString &cdata) const;
	virtual QTag *parent() const;
	virtual QTag *clone() const;
	virtual const QString findCData (const QString &expression) const;
	virtual const QTag *findTag (const QString &expression) const;
	virtual QConstTagList findTagList (const QString &expression) const;
	virtual operator bool() const;
private:
	Tag *m_tag;
	QList<QTag *> m_childs;
};

#endif // QGLOOXTAG_H
