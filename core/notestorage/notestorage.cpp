#include "notestorage.h"

NoteStorage::NoteStorage()
{

}

NoteStorage::~NoteStorage()
{

}

void NoteStorage::addNote(const Note::Ptr& note)
{
	m_notes.append(note);
}

Note::Ptr NoteStorage::getNote(const Note::id_t& id) const
{
	for(const auto& x : m_notes)
	{
		if(x->id() == id)
			return x;
	}
	return nullptr;
}

QList<Note::Ptr> NoteStorage::allNotes() const
{
	return m_notes;
}

void NoteStorage::clear()
{
	m_notes.clear();
}
