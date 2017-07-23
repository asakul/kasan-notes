#ifndef NOTESTORAGE_H
#define NOTESTORAGE_H

#include "core/backends/common/note.h"

#include <QObject>
#include <QList>

#include <memory>

class NoteStorage : public QObject
{
	Q_OBJECT
public:
	using Ptr = std::shared_ptr<NoteStorage>;

	NoteStorage();
	virtual ~NoteStorage();

	void addNote(const Note::Ptr& note);
	Note::Ptr getNote(const Note::id_t& id) const;
	void clear();

	QList<Note::Ptr> allNotes() const;

signals:
	void notesChanged();

private:
	QList<Note::Ptr> m_notes;
};

#endif // NOTESTORAGE_H
