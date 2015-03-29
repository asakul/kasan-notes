#include "kasandbauthenticator.h"
#include "log.h"

KasanDbAuthenticator::KasanDbAuthenticator(const KasanDbBackend::Ptr& backend) :
	m_backend(backend)
{
}

KasanDbAuthenticator::~KasanDbAuthenticator()
{
}

void KasanDbAuthenticator::login(const QString& name, const QString& password)
{
	m_backend->authenticate(name, password);
	emit authenticationCompleted();
}
