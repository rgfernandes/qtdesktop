#include <QTextStream>
#include <QDir>

#include "note.h"

Note::Note(const QString& fn, const QDir& dir, const QFont& f)
	: QPlainTextEdit(), name(fn), file(dir.absoluteFilePath(fn))
{
	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		setPlainText(in.readAll());
		file.close();
	}
	else if(file.open(QIODevice::WriteOnly | QIODevice::Text)) file.close();
	setFont(f);
}

