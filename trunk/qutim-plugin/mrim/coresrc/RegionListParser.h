/*****************************************************************************
    RegionListParser

    Copyright (c) 2009 by Rusanov Peter <tazkrut@mail.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef REGIONLISTPARSER_H
#define REGIONLISTPARSER_H

#include <QtCore>

struct LiveRegion
{
	quint32 id;
	quint32 cityId;
	quint32 countryId;
	QString name;
};

class RegionListParser
{
public:
	RegionListParser(QString aRelPath);
	inline const QList<LiveRegion>* GetRegionsList() { return const_cast<const QList<LiveRegion>*>(m_regionsList); }
	~RegionListParser(void);
private:
	void AddRegion(QString aStr);

	QList<LiveRegion>* m_regionsList;
};

#endif //REGIONLISTPARSER_H
