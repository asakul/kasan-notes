#ifndef EVERNOTENOTE_H
#define EVERNOTENOTE_H

#include "core/backends/common/note.h"

#include "3rdparty/evernote-sdk-cpp/src/Types_types.h"

class EvernoteBackend;
class EvernoteNote : public Note
{
public:
	EvernoteNote(const std::weak_ptr<EvernoteBackend>& backend);

	virtual ~EvernoteNote();

	virtual id_t id() const override;
	virtual QString backendId() const override;

	virtual void setPath(const QString& path) override;
	virtual QString path() const override;

	virtual void setTitle(const QString& title) override;
	virtual QString title() const override;

	virtual void setContent(const QString& content) override;
	virtual QString content() const override;

private:
	std::weak_ptr<EvernoteBackend> m_backend;
	std::string m_guid;
	std::shared_ptr<evernote::edam::Note> m_note;
};

#endif // EVERNOTENOTE_H
