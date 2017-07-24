#include "notestorage.h"

NoteStorage::NoteStorage() : m_rootNotebook(std::make_shared<Notebook>(0, ""))
{

}

NoteStorage::~NoteStorage()
{

}

void NoteStorage::replaceRootNotebook(const Notebook::Ptr& notebook)
{
	m_rootNotebook = notebook;
}
