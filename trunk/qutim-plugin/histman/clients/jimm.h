/*
 * jimm.h
 * 
 * Copyright (C) 2008  Nigmatullin Ruslan
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
*/

#ifndef JIMM_H_
#define JIMM_H_
#include <QString>
#include <QProgressBar>
#include "interfaces.h"
#include "dbh.h"

class jimm : public ClientInterface
{
public:
	jimm();
	virtual ~jimm();
	void load(QString path,DataBase *data,QByteArray defCharset,QString nickName,QProgressBar *bar);
};

#endif /*JIMM_H_*/
