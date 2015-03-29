#ifndef CORE_BACKENDS_KASANDB_KASANDBAUTHENTICATOR_H_
#define CORE_BACKENDS_KASANDB_KASANDBAUTHENTICATOR_H_

#include <QObject>
#include "kasandbbackend.h"

class KasanDbAuthenticator : public QObject
{
	Q_OBJECT
public:
	KasanDbAuthenticator(const KasanDbBackend::Ptr& backend);
	virtual ~KasanDbAuthenticator();

signals:
	void authenticationCompleted();
	void authenticationError(const QString& message);

public slots:
	void login(const QString& name, const QString& password);

private:
	KasanDbBackend::Ptr m_backend;
};

#endif /* CORE_BACKENDS_KASANDB_KASANDBAUTHENTICATOR_H_ */
