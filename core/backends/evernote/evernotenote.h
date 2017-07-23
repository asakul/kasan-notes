#ifndef EVERNOTENOTE_H
#define EVERNOTENOTE_H

#include "core/backends/common/note.h"

#include "qt5qevercloud/QEverCloud.h"

class EvernoteBackend;
class EvernoteNote : public Note
{
public:
	EvernoteNote(const std::weak_ptr<EvernoteBackend>& backend);

	virtual ~EvernoteNote();

	void setId(const id_t& id);
	virtual id_t id() const override;
	virtual QString backendId() const override;

	virtual void setPath(const QString& path) override;
	virtual QString path() const override;

	virtual void setTitle(const QString& title) override;
	virtual QString title() const override;

	virtual void setContent(const boost::optional<QString>& content) override;
	virtual boost::optional<QString> content() const override;

	void setGuid(const QString& g) { m_guid = g; }
	QString guid() const { return m_guid; }

private:
	id_t m_id;
	std::weak_ptr<EvernoteBackend> m_backend;
	QString m_guid;
	qevercloud::Note m_note;
	QString m_path;
};

#endif // EVERNOTENOTE_H
