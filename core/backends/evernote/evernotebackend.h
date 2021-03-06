#ifndef EVERNOTEBACKEND_H
#define EVERNOTEBACKEND_H

#include <QtCore/QObject>
#include <QUrl>
#include "core/backends/common/backend.h"

#include "qt5qevercloud/QEverCloud.h"

#include <atomic>
#include <memory>

class EvernoteBackend : public Backend,	public std::enable_shared_from_this<EvernoteBackend>
{
	Q_OBJECT
public:
	using Ptr = std::shared_ptr<EvernoteBackend>;

	explicit EvernoteBackend();

	static Ptr createFromDeveloperToken(const QString& token, const QUrl& url);

	virtual bool isAuthenticated() override;

	virtual Note::Ptr makeNote() override;
	virtual Notebook::Ptr makeNotebook() override;

signals:

public slots:
	virtual void requestAllNotes() override;
	virtual void requestNoteContent(const Note::Ptr& id) override;
	virtual void updateNote(const Note::Ptr& note) override;
	virtual void createNewNote(const Notebook::Ptr& parentNotebook) override;

private:
	std::unique_ptr<qevercloud::NoteStore> m_client;
	QString m_token;
	std::atomic<Note::id_t> m_noteIdCounter;
	std::atomic<Notebook::id_t> m_notebookIdCounter;

};

#endif // EVERNOTEBACKEND_H
