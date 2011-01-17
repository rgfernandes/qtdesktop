#ifdef QGlooxTag

#include "qglooxtag.h"

QGlooxTag::QGlooxTag(Tag *tag)
{
	m_tag = tag;
}

Tag *QGlooxTag::glooxTag()
{
	return m_tag;
}

QTag *QGlooxTag::newInstance(const QString &name, const QString &cdata)
{
	return new QGlooxTag(new Tag(utils::toStd(name), utils::toStd(cdata)));
}

QTag *QGlooxTag::newInstance(QTag *parent, const QString &name, const QString &cdata)
{
	return new QGlooxTag(new Tag(reinterpret_cast<QGlooxTag *>(parent)->glooxTag(), utils::toStd(name), utils::toStd(cdata)));
}

QTag *QGlooxTag::newInstance(const QString &name, const QString &attrib, const QString &value)
{
	return new QGlooxTag(new Tag(utils::toStd(name), utils::toStd(attrib), utils::toStd(value)));
}

QTag *QGlooxTag::newInstance(QTag *parent, const QString &name, const QString &attrib, const QString &value)
{
	return new QGlooxTag(new Tag(reinterpret_cast<QGlooxTag *>(parent)->glooxTag(), utils::toStd(name), utils::toStd(attrib), utils::toStd(value)));
}

const QString QGlooxTag::xml()
{
	return utils::fromStd(m_tag->xml());
}

bool QGlooxTag::setPrefix(const QString &xmlns)
{
	return m_tag->setPrefix(utils::toStd(xmlns));
}

const QString QGlooxTag::prefix() const
{
	return utils::fromStd(m_tag->prefix());
}

const QString QGlooxTag::prefix(const QString &xmlns)
{
	return utils::fromStd(m_tag->prefix(utils::toStd(xmlns)));
}

bool QGlooxTag::setXmlns (const QString &xmlns, const QString &prefix)
{
	return m_tag->setXmlns(utils::toStd(xmlns), utils::toStd(prefix));
}

const QString QGlooxTag::xmlns () const
{
	return utils::fromStd(m_tag->xmlns());
}

const QString QGlooxTag::xmlns (const QString &prefix) const
{
	return utils::fromStd(m_tag->xmlns(utils::toStd(prefix)));
}

bool QGlooxTag::addAttribute(const QString &name, const QString &value)
{
	return m_tag->addAttribute(utils::toStd(name), utils::toStd(value));
}

void QGlooxTag::addChild(QTag *child)
{
	m_tag->addChild(reinterpret_cast<QGlooxTag *>(child)->glooxTag());
	m_childs.append(child);
}

void QGlooxTag::addChildCopy(const QTag *child)
{
	Tag *tag = reinterpret_cast<QGlooxTag *>(child)->glooxTag()->clone();
	m_childs.append(new QTag(tag));
	m_tag->addChild(tag);
}

bool QGlooxTag::setCData (const QString &cdata)
{
	return m_tag->setCData(utils::toStd(cdata));
}

bool QGlooxTag::addCData (const QString &cdata)
{
	return m_tag->addCData(utils::toStd(cdata));
}

const QString QGlooxTag::name() const
{
	return utils::fromStd(m_tag->name());
}

const QString QGlooxTag::cdata() const
{
	return utils::fromStd(m_tag->cdata());
}

const QTagList QGlooxTag::children() const
{
	return QTagList;
}

const QString QGlooxTag::findAttribute (const QString &name) const
{
	return utils::fromStd(m_tag->findAttribute(utils::toStd(name)));
}

bool QGlooxTag::hasAttribute (const QString &name, const QString &value) const
{
	return m_tag->hasAttribute(utils::toStd(name), utils::toStd(value));
}

QTag *QGlooxTag::findChild (const QString &name) const
{
	return new QGlooxTag(m_tag->findChild(utils::toStd(name)));
}

QTag *QGlooxTag::findChild (const QString &name, const QString &attr, const QString &value) const
{
	return new QGlooxTag(m_tag->findChild(utils::toStd(name), utils::toStd(attr), utils::toStd(value)));
}

bool QGlooxTag::hasChild (const QString &name, const QString &attr, const QString &value) const
{
	return m_tag->hasChild(utils::toStd(name), utils::toStd(attr), utils::toStd(value));
}

QTag *QGlooxTag::findChildWithAttrib (const QString &attr, const QString &value) const
{
	return new QGlooxTag(m_tag->findChildWithAttrib(utils::toStd(attr), utils::toStd(value)));
}

bool QGlooxTag::hasChildWithAttrib (const QString &attr, const QString &value) const
{
	return m_tag->hasChildWithAttrib(utils::toStd(attr), utils::toStd(value));
}

QTagList QGlooxTag::findChildren (const QString &name, const QString &xmlns) const
{
	return QTagList;
}

void QGlooxTag::removeChild (const QString &name, const QString &xmlns)
{
}

void QGlooxTag::removeChild (QTag *tag)
{
}

void QGlooxTag::removeAttribute (const QString &attr, const QString &value, const QString &xmlns)
{
}

bool QGlooxTag::hasChildWithCData (const QString &name, const QString &cdata) const
{
}

QTag *QGlooxTag::parent() const
{
}

QTag *QGlooxTag::clone() const
{
}

const QString QGlooxTag::findCData (const QString &expression) const
{
}

const QTag *QGlooxTag::findTag (const QString &expression) const
{
}

QConstTagList QGlooxTag::findTagList (const QString &expression) const
{
}

QGlooxTag::operator bool() const
{
}

#endif
