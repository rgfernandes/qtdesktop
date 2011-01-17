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

#include "RegionListParser.h"

RegionListParser::RegionListParser(QString aRelPath)
{//usually will be - Resources/region.txt	
	QFile regionsFile(aRelPath);
	QString codepage = "UTF8";
	QTextCodec* codec = QTextCodec::codecForName(codepage.toLocal8Bit());
	m_regionsList = new QList<LiveRegion>;

	if (codec == NULL)
		return;

	if (!regionsFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	
	QTextStream fileStream(&regionsFile);
	fileStream.setCodec(codec);
	while (!fileStream.atEnd()) 
	{
		QString line = fileStream.readLine();
		AddRegion(line);
	}
}

RegionListParser::~RegionListParser(void)
{
	if (m_regionsList)
	{
		delete m_regionsList;
	}
}

void RegionListParser::AddRegion(QString aStr)
{
	QStringList strList = aStr.split(';');
	LiveRegion reg;
	if (strList.count() > 0)
	{
		reg.id = strList[0].toUInt();		
	}

	if (strList.count() > 1)
	{
		reg.cityId = strList[1].toUInt();		
	}

	if (strList.count() > 2)
	{
		reg.countryId = strList[2].toUInt();		
	}

	if (strList.count() > 3)
	{
		reg.name = strList[3];		
	}
	m_regionsList->append(reg);
}
