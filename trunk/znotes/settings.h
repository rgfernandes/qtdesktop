#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QObject>
#include <QSettings>
#include <QRect>
#include <QPoint>
#include <QByteArray>
#include <QFont>
#include <QVector>
#include <QTranslator>
#include <QLocale>

//#include <QtDebug>

#include "scriptmodel.h"

class Settings : public QObject
{
	Q_OBJECT
public:
	Settings();
	void load();
	//
	inline const QString& getNotesPath()			{ return NotesPath; }
	inline const QString& getLastNote()				{ return LastNote; }
	inline bool getHideStart()						{ return HideStart; }
	inline const QByteArray& getDialogGeometry()	{ return DialogGeometry; }
	inline const QByteArray& getDialogState()		{ return DialogState; }
	inline bool getHideFrame()						{ return HideFrame; }
	inline bool getStayTop()						{ return StayTop; }
	inline const QFont& getNoteFont()				{ return NoteFont; }
	inline ScriptModel& getScriptModel()			{ return script_model; }
	inline bool getScriptShowOutput()				{ return ScriptShowOutput; }
	inline bool getScriptCopyOutput()				{ return ScriptCopyOutput; }
	inline const QVector<int>& getToolbarItems()	{ return tb_items; }
	inline const QMap<QLocale::Language, QString>& getTranslations()	{ return translations; }
	inline QLocale::Language getLanguageCurrent()	{ return LanguageCurrent; }
	inline bool getLanguageCustom()					{ return LanguageCustom; }
	inline bool getNoteLinksHighlight()				{ return NoteLinksHighlight; }
	inline bool getNoteLinksOpen()					{ return NoteLinksOpen; }
	//
	void setNotesPath(const QString& path);
	void setLastNote(const QString& name);
	void setHideStart(bool hide);
	void setDialogGeometry(const QByteArray& g);
	void setDialogState(const QByteArray& g);
	void setHideFrame(bool s);
	void setStayTop(bool s);
	void setNoteFont(const QFont& f);
	void setScriptShowOutput(bool b);
	void setScriptCopyOutput(bool b);
	void setScripts();
	void setToolbarItems(const QVector<int>& v);
	void setLanguage(QLocale::Language);
	void setLanguageCurrent(QLocale::Language);
	void setLanguageCustom(bool);
	void setNoteLinksHighlight(bool);
	void setNoteLinksOpen(bool);
	//
	void loadLanguages();
	//
	inline void save() { config.sync(); }
private:
	QSettings config;
	QTranslator translator;
	QTranslator qtranslator;
	//
	QMap<QLocale::Language, QString> translations;
	QLocale::Language LanguageCurrent;
	bool LanguageCustom;
	//
	QString NotesPath;
	QString LastNote;
	bool HideStart;
	//
	QByteArray DialogGeometry;
	QByteArray DialogState;
	//
	bool HideFrame;
	bool StayTop;
	//
	QFont NoteFont;
	bool NoteLinksHighlight;
	bool NoteLinksOpen;
	//
	ScriptModel script_model;
	bool ScriptShowOutput;
	bool ScriptCopyOutput;
	//
	QVector<int> tb_items;
	//
	QLocale::Language system_language;
signals:
	void NotesPathChanged();
	void WindowStateChanged();
	void NoteFontChanged();
	void NoteHighlightChanged();
	void NoteLinkOpenChanged();
	void tbHidingChanged();
	void ToolbarItemsChanged();
};

extern Settings settings;

#endif // SETTINGS_H
