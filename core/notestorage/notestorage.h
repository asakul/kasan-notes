#ifndef NOTESTORAGE_H
#define NOTESTORAGE_H

#include "core/backends/common/note.h"
#include "core/backends/common/notebook.h"

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

	void replaceRootNotebook(const Notebook::Ptr& notebook);
	Notebook::Ptr rootNotebook() const { return m_rootNotebook; }

signals:
	void notesChanged();

private:
	Notebook::Ptr m_rootNotebook;
};

#endif // NOTESTORAGE_H
