/*
Copyright (c) 2008-2009 by Denis Daschenko <daschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JDISKOITEM_H
#define JDISKOITEM_H

//#include "jDiscoIdentity.h"
#include <QStringList>

class jDiscoItem
{
	public:
		enum jDiscoAction {EXEC, REGISTER, JOIN, SEARCH, ADD, VCARD, PROXY};
		struct jDiscoIdentity
		{
			QString name;
			QString category;
			QString type;
		};
		jDiscoItem() {isEmpty = true;}
		void setName(const QString &name) {m_name = name;}
		void setJID(const QString &jid) {m_jid = jid;}
		void setNode(const QString &node) {m_node = node;}
		void setKey(const QString &key) {m_key = key;}
		void setError(const QString &error) {m_error = error;}
		void setExpand(bool expand) {m_expand = expand;}
		void addIdentity(const jDiscoIdentity &identity) {m_identities << identity; isEmpty = false;}
		bool hasIdentity(const QString &category, const QString &type = "")
		{
			foreach (jDiscoIdentity identity, m_identities)
				if (identity.category == category && (type.isEmpty() || identity.type == type))
					return true;
		}
		void addFeature(const QString &feature) {m_features << feature; isEmpty = false;}
		bool hasFeature(const QString &feature)
		{
			return m_features.contains(feature);
		}
		void addAction(jDiscoAction action) {m_actions << action; isEmpty = false;}
		/*bool hasAction(jDiscoAction action)
		{
			return m_actions.contains(action);
		}*/
		QString name() const {return m_name;}
		QString jid() const {return m_jid;}
		QString node() const {return m_node;}
		QString key() const {return m_key;}
		QString error() const {return m_error;}
		bool expand() const {return m_expand;}
		QList<jDiscoIdentity> identities() {return m_identities;}
		QList<jDiscoAction> actions() {return m_actions;}
		QStringList features() {return m_features;}
		bool empty() const {return isEmpty;}

	private:
		QString m_name, m_jid, m_node, m_key, m_error;
		QList<jDiscoIdentity> m_identities;
		QStringList m_features;
		QList<jDiscoAction> m_actions;
		bool m_expand, isEmpty;

};

#endif //JDISKOITEM_H
