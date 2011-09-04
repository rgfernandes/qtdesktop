/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on demo application TextEdit by Trolltech, http://trolltech.com
****************************************************************************/
#ifndef _FONT_SETTINGS_DIALOG_H_
#define _FONT_SETTINGS_DIALOG_H_

#include <QtCore/QObject>
#include <QtGui/QDialog>
#include "config.h"

class FontPanel;
//struct FontSettings;
class QDialogButtonBox;

class FontSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    FontSettingsDialog(QWidget *parent = 0);
    ~FontSettingsDialog();

    bool showDialog(FontSettings *settings);

private:
    void updateFontSettings(FontSettings *settings);
    void setupFontSettingsDialog(const FontSettings *settings);
    
private:
    FontPanel *m_windowFontPanel;
    FontPanel *m_browserFontPanel;
    QDialogButtonBox *m_dialogButtonBox;
};

#endif  // _FONT_SETTINGS_DIALOG_H_
