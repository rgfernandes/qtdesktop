/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef AUTOTEXT_H
#define AUTOTEXT_H

#include "ui_autotext.h"

//==================== class AutoText ====================
class AutoText : public QDialog
{
     Q_OBJECT

public:
    AutoText(QWidget *parent = 0);
    void addSignature(QString str);

//signals:

public slots:
    void Apply(); 		//apply changes and close dialog
    void closeDialog(); //close dialog without changes

private slots:
    void delSignature();

private:
    Ui::AutoText ui;

}; // class AutoText

#endif // AUTOTEXT_H
