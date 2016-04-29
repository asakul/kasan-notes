#ifndef EVERNOTEBACKEND_H
#define EVERNOTEBACKEND_H

#include <QObject>
#include "core/backends/common/backend.h"

#include "3rdparty/evernote-sdk-cpp/src/NoteStore.h"

#include <memory>

class EvernoteBackend : public Backend
{
	Q_OBJECT
public:
	using Ptr = std::shared_ptr<EvernoteBackend>;

	explicit EvernoteBackend();

	static Ptr createFromDeveloperToken(const QString& token, const QString& url);

	virtual bool isAuthenticated() override;

signals:

public slots:

private:
	std::unique_ptr<evernote::edam::NoteStoreClient> m_client;
};

#endif // EVERNOTEBACKEND_H
