/**
 * @file stubnote.h
 */

#ifndef TEST_STUBS_STUBNOTE_H_
#define TEST_STUBS_STUBNOTE_H_

#include "core/backends/common/note.h"

class StubNote : public Note
{
public:
	StubNote(id_t id, const QString& backendId);
	virtual ~StubNote();

	virtual id_t id() const;
	virtual QString backendId() const;

	virtual void setPath(const QString& path);
	virtual QString path() const;

	virtual void setTitle(const QString& title);
	virtual QString title() const;

	virtual void setContent(const boost::optional<QString>& content);
	virtual boost::optional<QString> content() const;

private:
	id_t m_id;
	QString m_backendId;

	QString m_path;
	QString m_title;
	boost::optional<QString> m_content;
};

#endif /* TEST_STUBS_STUBNOTE_H_ */
