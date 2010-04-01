/*
    joinChannel

    Copyright (c) 2008 by Alexander Kazarin <boiler@co.ru>s

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#ifndef JOINCHANNEL_H
#define JOINCHANNEL_H
//
#include <QDialog>
#include "ui_joinchannel.h"
//
class joinChannelDialog : public QDialog, public Ui::joinChannelClass
{
Q_OBJECT
public:
	joinChannelDialog( QWidget * parent = 0, Qt::WFlags f = 0 );
	QString getChannel() { return editChannel->text(); }
private slots:
};
#endif





