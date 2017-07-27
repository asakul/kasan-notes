#include "evernotebackend.h"

#include "log.h"

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "core/backends/common/note.h"
#include "core/backends/evernote/evernotenotebook.h"
#include "core/backends/evernote/evernoteattachment.h"
#include "core/backends/evernote/evernotenote.h"

#include <QList>

static QString emptyNoteContent = R"(
<!DOCTYPE en-note SYSTEM 'http://xml.evernote.com/pub/enml2.dtd'>
<en-note>
</en-note>
)";

EvernoteBackend::EvernoteBackend() : Backend(),
	m_noteIdCounter(1),
	m_notebookIdCounter(1)
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
	QList<Note::Ptr> notesList;

	const int NotesInChunk = 50;

	LOG(DEBUG) << "EvernoteBackend: requesting all notebooks";
	auto notebooks = m_client->listNotebooks();
	auto root = std::make_shared<EvernoteNotebook>(0);
	for(const auto& notebook : notebooks)
	{
		LOG(DEBUG) << "Obtained notebook: " << notebook.name.value("[unset]") << " (" << notebook.guid.value("[unset]") << ")";
		qevercloud::NoteFilter filter;
		filter.notebookGuid = notebook.guid;
		qevercloud::NotesMetadataResultSpec resultSpec;
		resultSpec.includeTitle = true;
		resultSpec.includeUpdateSequenceNum = true;
		int startIndex = 0;

		auto evernoteNotebook = std::static_pointer_cast<EvernoteNotebook>(makeNotebook());
		evernoteNotebook->setGuid(notebook.guid);
		evernoteNotebook->setTitle(notebook.name.value("[unset]"));
		root->addNotebook(evernoteNotebook);
		auto notesMetadata = m_client->findNotesMetadata(filter, startIndex, NotesInChunk, resultSpec);
		do
		{
			for(const auto& metadata : notesMetadata.notes)
			{
				LOG(DEBUG) << "Obtained metadata: " << metadata.title.value("[unset]") <<
						" (" << metadata.guid << ";" <<
						metadata.updateSequenceNum.value(-1) << ")";

				auto note = std::static_pointer_cast<EvernoteNote>(makeNote());
				note->setGuid(metadata.guid);
				note->setTitle(metadata.title.value("[untitled]"));
				evernoteNotebook->addNote(note);
			}
			startIndex += notesMetadata.notes.length();
		} while(notesMetadata.startIndex + notesMetadata.notes.length() < notesMetadata.totalNotes);
	}

	emit allNotes(root);
}

void EvernoteBackend::requestNoteContent(const Note::Ptr& note)
{
	auto evernoteNote = std::dynamic_pointer_cast<EvernoteNote>(note);
	if(!evernoteNote)
	{
		LOG(WARNING) << "Invalid note requested from EvernoteBackend: id=" << note->id() << ", backendId=" << note->backendId();
		return;
	}
	LOG(DEBUG) << "EvernoteBackend: requesting note: " << note->id();
	auto rawNote = m_client->getNote(evernoteNote->guid(), true, true, false, false);
	if(rawNote.resources.isSet())
	{
		for(const auto& resource : rawNote.resources.ref())
		{
			auto attachment = std::make_shared<EvernoteAttachment>(resource.mime.value("unknown"), resource.data.ref().body.value(QByteArray()));
			attachment->setGuid(resource.guid.value(""));
			evernoteNote->addAttachment(attachment);
			LOG(DEBUG) << "Added attachment: mime: " << attachment->mimeType() << "; hash: " << QString::fromUtf8(attachment->hash().toHex());
		}
	}
	evernoteNote->setEnml(rawNote.content.value(""));
	emit noteUpdated(note);
}

void EvernoteBackend::updateNote(const Note::Ptr& note)
{
	LOG(DEBUG) << "EvernoteBackend: updateNote: " << note->id();
	auto evernoteNote = std::dynamic_pointer_cast<EvernoteNote>(note);
	if(!evernoteNote)
	{
		LOG(WARNING) << "Invalid note passed to updateNote to EvernoteBackend: id=" << note->id() << ", backendId=" << note->backendId();
		return;
	}
	LOG(DEBUG) << "Content: " << evernoteNote->enml().value();

	qevercloud::Note evNote = m_client->getNote(evernoteNote->guid(), false, false, false, false);
	QList<QString> oldResources;
	QList<Attachment::Ptr> newResources;

	for(int i = 0; i < evernoteNote->attachmentsCount(); i++)
	{
		LOG(DEBUG) << "Attachment: " << evernoteNote->attachmentByIndex(i)->hash().toHex();
		auto evernoteAttachment = std::dynamic_pointer_cast<EvernoteAttachment>(evernoteNote->attachmentByIndex(i));
		if(!evernoteAttachment) // Generic new Attachment added by editor
		{
			newResources.append(evernoteNote->attachmentByIndex(i));
		}
		else
		{
			oldResources.append(evernoteAttachment->guid());
		}
	}

	QList<qevercloud::Resource> resources;
	for(const auto& guid : oldResources)
	{
		qevercloud::Resource r;
		r.guid = guid;
		resources.append(r);
		LOG(DEBUG) << "Adding old resource: " << r.guid.value();
	}
	for(const auto& attach : newResources)
	{
		qevercloud::Resource r;
		r.mime = attach->mimeType();
		qevercloud::Data d;
		d.bodyHash = attach->hash();
		d.size = attach->data().size();
		d.body = attach->data();
		r.data = d;
		resources.append(r);
		LOG(DEBUG) << "Adding new resource: " << QString::fromUtf8(attach->hash().toHex());
	}

	evNote.resources = resources;

	evNote.title = note->title();
	auto content = evernoteNote->enml();
	if(content.isSet())
	{
		evNote.content = content.value("");
		m_client->updateNote(evNote);
		emit noteSaved(note);
	}
	else
	{
		LOG(WARNING) << "EvernoteBackend: trying to update note without content";
	}
	LOG(INFO) << "Note updated";
}

Note::Ptr EvernoteBackend::makeNote()
{
	return std::make_shared<EvernoteNote>(m_noteIdCounter.fetch_add(1), shared_from_this());
}

Notebook::Ptr EvernoteBackend::makeNotebook()
{
	return std::make_shared<EvernoteNotebook>(m_notebookIdCounter.fetch_add(1));
}

void EvernoteBackend::createNewNote(const Notebook::Ptr& parentNotebook)
{
	LOG(DEBUG) << "EvernoteBackend: create new note";
	auto evernoteNotebook = std::dynamic_pointer_cast<EvernoteNotebook>(parentNotebook);
	if(evernoteNotebook)
	{
		qevercloud::Note note;
		note.notebookGuid = evernoteNotebook->guid();
		note.title = QString("[Untitled]");
		note.content = emptyNoteContent;
		auto newNote = m_client->createNote(note);
		auto evernoteNote = std::static_pointer_cast<EvernoteNote>(makeNote());
		evernoteNote->setGuid(newNote.guid.value());
		evernoteNote->setTitle(newNote.title.value());
		evernoteNote->setContent(newNote.content.value());
		parentNotebook->addNote(evernoteNote);
		emit noteCreated(evernoteNote);
		LOG(INFO) << "EvernoteBackend: New note created: " << evernoteNote->guid();
	}
}
