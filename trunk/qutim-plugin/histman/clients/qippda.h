/*
 * qippda.h
 * 
 * Copyright (C) 2008  Nigmatullin Ruslan
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
*/

#ifndef QIPPDA_H_
#define QIPPDA_H_
#include "include/qutim/historymanager.h"

namespace HistoryManager {

class qippda : public HistoryImporter
{
public:
	qippda();
	virtual ~qippda();
	virtual void loadMessages(const QString &path);
	virtual bool validate(const QString &path);
	virtual QString name();
	virtual QIcon icon();
};

}

#endif /*QIPPDA_H_*/
