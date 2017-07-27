/**
 * @file evernotenotebook.h
 */

#ifndef CORE_BACKENDS_EVERNOTE_EVERNOTENOTEBOOK_H_
#define CORE_BACKENDS_EVERNOTE_EVERNOTENOTEBOOK_H_

#include "core/backends/common/notebook.h"

class EvernoteNotebook : public Notebook
{
public:
	EvernoteNotebook(const Notebook::id_t& id);
	virtual ~EvernoteNotebook();

	virtual void setGuid(const QString& guid);
	virtual QString guid() const;

private:
	QString m_guid;
};

#endif /* CORE_BACKENDS_EVERNOTE_EVERNOTENOTEBOOK_H_ */
