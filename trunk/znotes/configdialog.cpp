#include "configdialog.h"
#include "ui_configdialog.h"

#include "settings.h"
#include "toolbarmodel.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>

ItemModel m_items;
ItemToolbarModel mt_items;

configDialog::configDialog(QWidget *parent) :
	QDialog(parent), m_ui(new Ui::configDialog)
{
	m_ui->setupUi(this);
	m_ui->listActions->setModel(&m_items);
	m_ui->listToolbarActions->setModel(&mt_items);
	//
	m_ui->ed_NotesPath->setText(settings.getNotesPath());
	m_ui->cb_FileScan->setChecked(settings.getFileScanner());
	m_ui->sb_FileScanTimeout->setValue(settings.getFileScannerTimeout());
	m_ui->cmb_TabPosition->setCurrentIndex(settings.getTabPosition());
	m_ui->cb_ShowHidden->setChecked(settings.getShowHidden());
	m_ui->cb_ShowExtensions->setChecked(settings.getShowExtensions());
	m_ui->cb_HideStart->setChecked(settings.getHideStart());
	m_ui->cb_FrameHide->setChecked(settings.getHideFrame());
	m_ui->cb_StayTop->setChecked(settings.getStayTop());
	//
	m_ui->tabScripts->setModel(&settings.getScriptModel());
	m_ui->tabScripts->resizeColumnsToContents();
	m_ui->cb_ScriptShowOutput->setChecked(settings.getScriptShowOutput());
	m_ui->cb_ScriptCopyOutput->setChecked(settings.getScriptCopyOutput());
	//
	m_ui->lb_FontExample->setFont(settings.getNoteFont());
	m_ui->cb_NoteLinksHighlight->setChecked(settings.getNoteLinksHighlight());
	m_ui->cb_NoteLinksOpen->setChecked(settings.getNoteLinksOpen());
	//
	const QMap<QLocale::Language, QString> translations = settings.getTranslations();
	QMapIterator<QLocale::Language, QString> translation(translations);
	while(translation.hasNext())
	{
		const QLocale::Language language = translation.next().key();
		m_ui->cmb_Language->addItem(QLocale::languageToString(language), language);
	}
	int current_language_index = m_ui->cmb_Language->findData(settings.getLanguageCurrent());
	m_ui->cmb_Language->setCurrentIndex(current_language_index);
	//
	m_ui->cb_LanguageCustom->setChecked(settings.getLanguageCustom());
	m_ui->cmb_Language->setEnabled(settings.getLanguageCustom());
	//
	mt_items.setVector(settings.getToolbarItems());
	m_items.setVector(settings.getToolbarItems());
	//
	connect(m_ui->listActions->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentListActionChanged(QModelIndex,QModelIndex))); //TODO: selection changed
	connect(m_ui->listToolbarActions->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentToolbarActionChanged(QModelIndex,QModelIndex)));
}

configDialog::~configDialog()
{
	delete m_ui;
}

void configDialog::SaveSettings()
{
	settings.setHideStart(m_ui->cb_HideStart->checkState());
	settings.setNotesPath(m_ui->ed_NotesPath->text());
	settings.setFileScannerTimeout(m_ui->sb_FileScanTimeout->value());
	settings.setFileScanner(m_ui->cb_FileScan->isChecked());
	settings.setTabPosition(TabPosition(m_ui->cmb_TabPosition->currentIndex()));
	settings.setShowHidden(m_ui->cb_ShowHidden->checkState());
	settings.setShowExtensions(m_ui->cb_ShowExtensions->checkState());
	settings.setHideFrame(m_ui->cb_FrameHide->checkState());
	settings.setStayTop(m_ui->cb_StayTop->checkState());
	settings.setNoteFont(m_ui->lb_FontExample->font());
	settings.setNoteLinksHighlight(m_ui->cb_NoteLinksHighlight->checkState());
	settings.setNoteLinksOpen(m_ui->cb_NoteLinksOpen->checkState());
	settings.setScriptShowOutput(m_ui->cb_ScriptShowOutput->checkState());
	settings.setScriptCopyOutput(m_ui->cb_ScriptCopyOutput->checkState());
	settings.setToolbarItems(mt_items.getVector());
	settings.setLanguageCustom(m_ui->cb_LanguageCustom->isChecked());
	settings.setLanguageCurrent(QLocale::Language(m_ui->cmb_Language->itemData(m_ui->cmb_Language->currentIndex(), Qt::UserRole).toInt()));
}

void configDialog::on_buttonBox_clicked(QAbstractButton* button)
{
	const QAbstractButton* BTN_SAVE = m_ui->buttonBox->buttons().at(0);
	const QAbstractButton* BTN_CLOSE = m_ui->buttonBox->buttons().at(1);
	const QAbstractButton* BTN_APPLY = m_ui->buttonBox->buttons().at(2);
	//
	if(button==BTN_SAVE)
	{
		SaveSettings();
		close();
	}
	else if(button==BTN_CLOSE)
	{
		close();
	}
	else if(button==BTN_APPLY)
	{
		SaveSettings();
	}
}

void configDialog::on_btn_NotesPath_clicked()
{
	QString path = settings.getNotesPath();
	path = QFileDialog::getExistingDirectory(0,
			tr("Select notes directory"),settings.getNotesPath(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(!path.isEmpty()) m_ui->ed_NotesPath->setText(path);
}

void configDialog::on_btn_FontChange_clicked()
{
	QFontDialog dlg(m_ui->lb_FontExample->font());
	dlg.exec();
	m_ui->lb_FontExample->setFont(dlg.currentFont());
}

void configDialog::on_btn_ScriptRemove_clicked()
{
	if(!m_ui->tabScripts->selectionModel()->hasSelection()) return;
	int row = m_ui->tabScripts->selectionModel()->currentIndex().row();
	settings.getScriptModel().removeRow(row);
}

void configDialog::on_btn_ScriptAdd_clicked()
{
	settings.getScriptModel().append();
}

void configDialog::on_butActionAdd_clicked()
{
	int id = m_ui->listActions->currentIndex().row();
	int row = (m_ui->listToolbarActions->selectionModel()->hasSelection())?
		m_ui->listToolbarActions->currentIndex().row():-1;
	mt_items.insert(id, row);
	m_items.remove(id);
}

void configDialog::on_butActionRemove_clicked()
{
	m_ui->butActionRemove->setDisabled(true);
	QModelIndex index = m_ui->listToolbarActions->currentIndex();
	int id = mt_items.getId(index);
	mt_items.remove(index);
	m_items.insert(id);
}

void configDialog::on_butActionTop_clicked()
{
	QModelIndex index(m_ui->listToolbarActions->currentIndex());
	QModelIndex new_index = mt_items.up(index);
	m_ui->listToolbarActions->setCurrentIndex(new_index);
}

void configDialog::on_butActionBottom_clicked()
{
	QModelIndex index(m_ui->listToolbarActions->currentIndex());
	QModelIndex new_index = mt_items.down(index);
	m_ui->listToolbarActions->setCurrentIndex(new_index);
}

//On changing selection in toolar actions list
void configDialog::currentToolbarActionChanged(QModelIndex index, QModelIndex)
{
	m_ui->butActionRemove->setEnabled(index.isValid());
	int row = index.row();
	m_ui->butActionTop->setEnabled(row>0);
	m_ui->butActionBottom->setEnabled(row<(mt_items.rowCount()-1));
}

//On changing selection in current toolar actions list
void configDialog::currentListActionChanged(QModelIndex index, QModelIndex)
{
//	QModelIndexList list = m_ui->listActions->selectionModel()->selectedRows(0);
//	qDebug() << "> " << list.size();
//	for(int i=0; i<list.size(); ++i)
//	{if(m_items.isUsed(list[i].row()))
//	{
//		m_ui->butActionAdd->setEnabled(false);
//		return;
//	} qDebug() << i; }
//	m_ui->butActionAdd->setEnabled(true);
	m_ui->butActionAdd->setEnabled(index.isValid() && !m_items.isUsed(index.row()));
}

//Retranslating ui on language change
void configDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		//retranslateUi() function changes current index of cmb_TabPosition
		m_ui->cmb_TabPosition->setCurrentIndex(settings.getTabPosition());
		break;
	default:
		break;
	}
}

