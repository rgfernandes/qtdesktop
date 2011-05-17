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
#include "fontsettingsdialog.h"
#include "fontpanel.h"

#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QStackedWidget>
#include <QtGui/QDialogButtonBox>

FontSettingsDialog::FontSettingsDialog(QWidget *parent)
    : QDialog(parent)
    , m_windowFontPanel(new FontPanel(this))
    , m_browserFontPanel(new FontPanel(this))
    , m_dialogButtonBox(new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel))
{
    setModal(true);
    setWindowTitle(tr("Font Settings"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    mainVLayout->addLayout(hboxLayout);

    QLabel *label = new QLabel(tr("Font settings for:"), this);
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    hboxLayout->addWidget(label);
    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItem(tr("Documents"));
    comboBox->addItem(tr("Application"));
    hboxLayout->addWidget(comboBox);

    m_windowFontPanel->setCheckable(true);
    m_browserFontPanel->setCheckable(true);

    const QString customSettings(tr("Use custom settings"));
    m_windowFontPanel->setTitle(customSettings);
    m_browserFontPanel->setTitle(customSettings);

    QStackedWidget *stackWidget = new QStackedWidget(this);
    stackWidget->addWidget(m_browserFontPanel);
    stackWidget->addWidget(m_windowFontPanel);

    mainVLayout->addWidget(stackWidget);
    mainVLayout->addWidget(m_dialogButtonBox);

    connect(m_dialogButtonBox , SIGNAL(rejected()), this, SLOT(reject()));
    connect(m_dialogButtonBox , SIGNAL(accepted()), this, SLOT(accept()));
    connect(comboBox, SIGNAL(activated(int)), stackWidget, SLOT(setCurrentIndex(int)));
}

//-------------------------------------------------
FontSettingsDialog::~FontSettingsDialog()
{
    // nothing todo
}

//-------------------------------------------------
bool FontSettingsDialog::showDialog(FontSettings *settings)
{
    setupFontSettingsDialog(settings);

    if (exec() != Accepted)
        return false;

    updateFontSettings(settings);
    return true;
}

//-------------------------------------------------
void FontSettingsDialog::updateFontSettings(FontSettings *settings)
{
    settings->useWindowFont = m_windowFontPanel->isChecked();
    settings->useBrowserFont = m_browserFontPanel->isChecked();

    settings->windowFont = settings->useWindowFont ? m_windowFontPanel->selectedFont() : qApp->font();
    settings->browserFont = settings->useBrowserFont ? m_browserFontPanel->selectedFont() : qApp->font();

    settings->windowWritingSystem = settings->useWindowFont ? m_windowFontPanel->writingSystem() : QFontDatabase::Latin;
    settings->browserWritingSystem = settings->useBrowserFont ? m_browserFontPanel->writingSystem() : QFontDatabase::Latin;
}

//-------------------------------------------------
void FontSettingsDialog::setupFontSettingsDialog(const FontSettings *settings)
{
    m_windowFontPanel->setSelectedFont(settings->windowFont);
    m_browserFontPanel->setSelectedFont(settings->browserFont);

    m_windowFontPanel->setWritingSystem(settings->windowWritingSystem);
    m_browserFontPanel->setWritingSystem(settings->browserWritingSystem);

    m_windowFontPanel->setChecked(settings->useWindowFont);
    m_browserFontPanel->setChecked(settings->useBrowserFont);
}
