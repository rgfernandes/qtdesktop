#include "settings.h"

#include <QObject>
#include <QtDebug>

Settings::Settings() : config("pDev", "zNotes")
{
	NotesPath = config.value("NotesPath").toString();
	LastNote = config.value("LastNote").toString();
	HideStart = config.value("HideStart").toBool();
	DialogGeometry = config.value("DialogGeometry").toByteArray();
	//
	HideToolbar = config.value("HideToolbar").toBool();
	HideFrame = config.value("HideFrame").toBool();
	StayTop = config.value("StayTop").toBool();
	//
	NoteFont.fromString(config.value("NoteFont").toString());
	//
	int ScriptCount = config.value("ComandCount").toInt();
	for(int i=0; i<ScriptCount; ++i)
	{
		smodel.append(
			config.value(QString("ComandName%1").arg(i)).toString(),
			config.value(QString("ComandFile%1").arg(i)).toString(),
			config.value(QString("ComandIcon%1").arg(i)).toString());
	}
	ScriptShowOutput = config.value("ScriptShowOutput").toBool();
	ScriptCopyOutput = config.value("ScriptCopyOutput").toBool();
}

void Settings::setNotesPath(const QString& path)
{
	if(NotesPath != path)
	{
		NotesPath = path;
		config.setValue("NotesPath", NotesPath);
		emit NotesPathChanged();
	}
}

void Settings::setLastNote(const QString& note_name)
{
	if(LastNote != note_name)
	{
		LastNote = note_name;
		config.setValue("LastNote", LastNote);
	}
}

void Settings::setHideStart(bool hide)
{
	if(HideStart != hide)
	{
		HideStart = hide;
		config.setValue("HideStart", HideStart);
	}
}

void Settings::setHideToolbar(bool Hide)
{
	if(HideToolbar != Hide)
	{
		HideToolbar = Hide;
		config.setValue("HideToolbar", HideToolbar);
		emit ToolbarVisChanged();
	}
}

void Settings::setHideFrame(bool Hide)
{
	if(HideFrame != Hide)
	{
		HideFrame = Hide;
		config.setValue("HideFrame", HideToolbar);
		emit WindowStateChanged();
	}
}

void Settings::setStayTop(bool top)
{
	if(StayTop != top)
	{
		StayTop = top;
		config.setValue("StayTop", StayTop);
		emit WindowStateChanged();
	}
}

void Settings::setDialogGeometry(const QByteArray& g)
{
	DialogGeometry = g;
	config.setValue("DialogGeometry", DialogGeometry);
}

void Settings::setNoteFont(const QFont& f)
{
	if(NoteFont != f)
	{
		NoteFont = f;
		config.setValue("NoteFont", NoteFont.toString());
		emit NoteFontChanged();
	}
}

void Settings::setScriptShowOutput(bool sso)
{
	if(ScriptShowOutput != sso)
	{
		ScriptShowOutput = sso;
		config.setValue("ScriptShowOutput", ScriptShowOutput);
	}
}

void Settings::setScriptCopyOutput(bool sco)
{
	if(ScriptCopyOutput != sco)
	{
		ScriptCopyOutput = sco;
		config.setValue("ScriptCopyOutput", ScriptCopyOutput);
	}
}

void Settings::setScripts()
{
	config.setValue("ComandCount", smodel.rowCount());
	for(int i=0; i<smodel.rowCount(); ++i)
	{
		config.setValue(QString("ComandName%1").arg(i), smodel.getName(i));
		config.setValue(QString("ComandFile%1").arg(i), smodel.getFile(i));
		config.setValue(QString("ComandIcon%1").arg(i), smodel.getIcon(i));
	}
}