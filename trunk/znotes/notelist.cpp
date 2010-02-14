#include "notelist.h"
#include "settings.h"

NoteList::NoteList(QWidget* parent)
	: QObject(), vec(), current_index(-1)
{
	tabs = new QTabWidget(parent);
	tabs->setDocumentMode(true);
	tabs->setTabPosition(QTabWidget::TabPosition(settings.getTabPosition()));
	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(CurrentTabChanged(int)));
	connect(&settings, SIGNAL(ShowExtensionsChanged(bool)), this, SLOT(ShowExtensionsChanged(bool)));
	connect(&settings, SIGNAL(TabPositionChanged()), this, SLOT(TabPositionChanged()));
}

void NoteList::add(const QFileInfo& fileinfo)
{
	Note* note = new Note(fileinfo);
	vec.append(note);
	tabs->addTab(note->widget(), note->title());
	notes_filenames.insert(fileinfo.fileName());
	tabs->setCurrentWidget(note->widget());
}

bool NoteList::load(const QFileInfo& fileinfo, const QString& old_title)
{
	Note* note = new Note(fileinfo);
	vec.append(note);
	tabs->addTab(note->widget(), note->title());
	notes_filenames.insert(fileinfo.fileName());
	return (note->title()==old_title);
}

void NoteList::remove(int i)
{
	Note* note = vec[i];
	QString filename = note->fileName();
	tabs->removeTab(i);
	delete note;
	vec.remove(i);
	notes_filenames.remove(filename);
}

void NoteList::move(const QString& path)
{
	for(int i=0; i<vec.size(); ++i)
	{
		vec[i]->move(path);
	}
}

void NoteList::search(const QString& text)
{
	//Searching in current note
	if(current()->find(text)) return;
	//Searching in all notes
	const int max = count()+currentIndex();
	for(int n=currentIndex()+1; n<=max; ++n)
	{
		int i = n%vec.size(); //secret formula of success search
		if(vec[i]->find(text, true))
		{
			tabs->setCurrentIndex(i);
			return;
		}
	}
}

void NoteList::rename(int index, const QString& title)
{
	Note* note = vec[index];
	note->save();
	notes_filenames.remove(note->fileName());
	note->rename(title);
	tabs->setTabText(index, note->title());
	notes_filenames.insert(note->fileName());
}

void NoteList::CurrentTabChanged(int index)
{
	if(current_index!=-1) current()->save();
	int old_index = current_index;
	current_index = index;
	emit currentNoteChanged(old_index, current_index);
}

void NoteList::ShowExtensionsChanged(bool show_extensions)
{
	for(int i=0; i<vec.size(); ++i)
	{
		vec[i]->setTitle(show_extensions);
		tabs->setTabText(i, vec[i]->title());
	}
}

void NoteList::TabPositionChanged()
{
	tabs->setTabPosition(QTabWidget::TabPosition(settings.getTabPosition()));
}

//Saving all notes
void NoteList::SaveAll()
{
	for(int i=0; i<vec.size(); ++i)
	{
		vec[i]->save(true); //Forced saving
	}
}
