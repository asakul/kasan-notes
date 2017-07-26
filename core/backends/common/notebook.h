/**
 * @file notebook.h
 */

#ifndef CORE_BACKENDS_COMMON_NOTEBOOK_H_
#define CORE_BACKENDS_COMMON_NOTEBOOK_H_

#include "note.h"

#include <memory>
#include <cstdint>

#include <QString>
#include <QList>


class Notebook : public std::enable_shared_from_this<Notebook>
{
public:
	using Ptr = std::shared_ptr<Notebook>;
	using WeakPtr = std::weak_ptr<Notebook>;
	using id_t = uint64_t;

	Notebook(const id_t& id, const QString& backendId);
	virtual ~Notebook();

	virtual id_t id() const;
	virtual QString backendId() const;

	virtual void setTitle(const QString& title);
	virtual QString title() const;

	virtual void addNote(const Note::Ptr& note);
	virtual size_t notesCount() const;
	virtual Note::Ptr noteByIndex(size_t index) const;
	virtual Note::Ptr noteById(const Note::id_t& id) const;
	virtual void removeNote(const Note::Ptr& note);

	virtual void addNotebook(const Notebook::Ptr& notebook);
	virtual size_t notebooksCount() const;
	virtual Notebook::Ptr notebookByIndex(size_t index) const;

	virtual void setParent(const std::shared_ptr<Notebook>& parent);
	virtual std::weak_ptr<Notebook> parent() const;

private:
	id_t m_id;
	QString m_backendId;

	QString m_title;
	QList<Notebook::Ptr> m_notebooks;
	QList<Note::Ptr> m_notes;
	Notebook::WeakPtr m_parent;
};

#endif /* CORE_BACKENDS_COMMON_NOTEBOOK_H_ */
