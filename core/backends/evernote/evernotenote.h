#ifndef EVERNOTENOTE_H
#define EVERNOTENOTE_H

#include "core/backends/common/note.h"

#include "qt5qevercloud/QEverCloud.h"

class EvernoteBackend;
class EvernoteNote : public Note
{
public:
	EvernoteNote(const id_t& id, const std::weak_ptr<EvernoteBackend>& backend);

	virtual ~EvernoteNote();

	virtual void setEnml(const QString& enml);
	virtual qevercloud::Optional<QString> enml() const;

	virtual void setContent(const boost::optional<QString>& content) override;
	virtual boost::optional<QString> content() const override;

	bool attachmentsChanged() const;

	void setGuid(const QString& g) { m_guid = g; }
	QString guid() const { return m_guid; }

private:
	QString convertToEnml(const QString& content);

private:
	std::weak_ptr<EvernoteBackend> m_backend;
	QString m_guid;
	qevercloud::Note m_note;
};

#endif // EVERNOTENOTE_H
