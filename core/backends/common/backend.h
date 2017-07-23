#ifndef CORE_BACKENDS_COMMON_BACKEND_H_
#define CORE_BACKENDS_COMMON_BACKEND_H_

#include "note.h"

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
	void allNotes(const QList<Note::Ptr>& notes);

public slots:
	virtual void requestAllNotes() = 0;
};


#endif /* CORE_BACKENDS_COMMON_BACKEND_H_ */
