#include "evernotebackend.h"

#include "log.h"

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "core/backends/common/note.h"
#include "core/backends/common/notebook.h"
#include "core/backends/evernote/evernoteattachment.h"
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
	Note::id_t noteId = 1;
	Notebook::id_t notebookId = 1;
	QList<Note::Ptr> notesList;

	const int NotesInChunk = 50;

	LOG(DEBUG) << "EvernoteBackend: requesting all notebooks";
	auto notebooks = m_client->listNotebooks();
	auto root = std::make_shared<Notebook>(0, "evernote");
	for(const auto& notebook : notebooks)
	{
		LOG(DEBUG) << "Obtained notebook: " << notebook.name.value("[unset]") << " (" << notebook.guid.value("[unset]") << ")";
		qevercloud::NoteFilter filter;
		filter.notebookGuid = notebook.guid;
		qevercloud::NotesMetadataResultSpec resultSpec;
		resultSpec.includeTitle = true;
		resultSpec.includeUpdateSequenceNum = true;
		int startIndex = 0;

		auto evernoteNotebook = std::make_shared<Notebook>(notebookId, "evernote");
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

				auto note = std::make_shared<EvernoteNote>(noteId, shared_from_this());
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
}
