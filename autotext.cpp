/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#include "autotext.h"

//====================== class AutoText ============================
AutoText::AutoText(QWidget *parent)
     : QDialog(parent)
{
     ui.setupUi(this);

     connect(ui.BOk, SIGNAL(clicked()), this, SLOT(Apply()) );
     connect(ui.BCancel, SIGNAL(clicked()), this, SLOT(closeDialog()) );
     connect(ui.BDelete, SIGNAL(clicked()), this, SLOT(delSignature()) );
}

//-------------------------------------------------
void AutoText::closeDialog()
{

    QWidget::hide();  //close dialog
}

//-------------------------------------------------
void AutoText::Apply()
{

    QWidget::hide();  //close dialog
}

//-------------------------------------------------
void AutoText::addSignature(QString str)
{
    ui.LWSignatures->addItem(str);
}

//-------------------------------------------------
void AutoText::delSignature()
{
    ui.LWSignatures->takeItem(ui.LWSignatures->currentRow());
}
