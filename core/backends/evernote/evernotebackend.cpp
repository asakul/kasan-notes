#include "evernotebackend.h"

#include "log.h"

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "core/backends/common/note.h"
#include "core/backends/evernote/evernotenote.h"

#include <QList>

EvernoteBackend::EvernoteBackend() : Backend()
{

}

EvernoteBackend::Ptr EvernoteBackend::createFromDeveloperToken(const QString& token, const QUrl& url)
{
	auto backend = std::make_shared<EvernoteBackend>();
	backend->m_client = std::make_unique<qevercloud::NoteStore>(url.toString(), token);
	backend->m_token = token;
	return backend;
}


bool EvernoteBackend::isAuthenticated()
{
	return m_client != nullptr;
}

void EvernoteBackend::requestAllNotes()
{
	Note::id_t id = 1;
	QList<Note::Ptr> notesList;

	const int NotesInChunk = 50;

	LOG(DEBUG) << "EvernoteBackend: requesting all notebooks";
	auto notebooks = m_client->listNotebooks();
	for(const auto& notebook : notebooks)
	{
		LOG(DEBUG) << "Obtained notebook: " << notebook.name.value("[unset]") << " (" << notebook.guid.value("[unset]") << ")";
		qevercloud::NoteFilter filter;
		filter.notebookGuid = notebook.guid;
		qevercloud::NotesMetadataResultSpec resultSpec;
		resultSpec.includeTitle = true;
		resultSpec.includeUpdateSequenceNum = true;
		int startIndex = 0;
		auto notesMetadata = m_client->findNotesMetadata(filter, startIndex, NotesInChunk, resultSpec);
		do
		{
			for(const auto& metadata : notesMetadata.notes)
			{
				LOG(DEBUG) << "Obtained metadata: " << metadata.title.value("[unset]") <<
						" (" << metadata.guid << ";" <<
						metadata.updateSequenceNum.value(-1) << ")";

				auto note = std::make_shared<EvernoteNote>(shared_from_this());
				note->setId(id++);
				note->setGuid(metadata.guid);
				note->setTitle(metadata.title.value("[untitled]"));
				note->setPath("/" + notebook.name.value("[unknown notebook]") + "/");
				notesList.append(note);
			}
			startIndex += notesMetadata.notes.length();
		} while(notesMetadata.startIndex + notesMetadata.notes.length() < notesMetadata.totalNotes);
	}

	emit allNotes(notesList);
}
