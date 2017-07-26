#ifndef CORE_BACKENDS_COMMON_BACKEND_H_
#define CORE_BACKENDS_COMMON_BACKEND_H_

#include "note.h"
#include "notebook.h"

#include <memory>
#include <QObject>

class Backend : public QObject
{
	Q_OBJECT
public:
	typedef std::shared_ptr<Backend> Ptr;

	virtual ~Backend();

	virtual bool isAuthenticated() = 0;

signals:
	void allNotes(const Notebook::Ptr& root);
	void noteUpdated(const Note::Ptr& note);
	void noteSaved(const Note::Ptr& note);

public slots:
	virtual void requestAllNotes() = 0;
	virtual void requestNoteContent(const Note::Ptr& id) = 0;
	virtual void updateNote(const Note::Ptr& note) = 0;
};


#endif /* CORE_BACKENDS_COMMON_BACKEND_H_ */
