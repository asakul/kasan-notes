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


class Notebook
{
public:
	using Ptr = std::shared_ptr<Notebook>;
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

private:
	id_t m_id;
	QString m_backendId;

	QString m_title;
	QList<Note::Ptr> m_notes;
};

#endif /* CORE_BACKENDS_COMMON_NOTEBOOK_H_ */
