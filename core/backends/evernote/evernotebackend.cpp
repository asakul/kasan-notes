#include "evernotebackend.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/THttpClient.h>
#include <boost/shared_ptr.hpp>

using namespace apache::thrift;

EvernoteBackend::EvernoteBackend() : Backend()
{

}

EvernoteBackend::Ptr EvernoteBackend::createFromDeveloperToken(const QString& token, const QString& url)
{
	auto transport = boost::shared_ptr<transport::THttpClient>(new transport::THttpClient(url.toStdString(), 80));
	auto proto = boost::shared_ptr<protocol::TBinaryProtocol>(new protocol::TBinaryProtocol(transport));
	auto backend = std::make_shared<EvernoteBackend>();
	backend->m_client = std::make_unique<evernote::edam::NoteStoreClient>(proto);
	return backend;
}


bool EvernoteBackend::isAuthenticated()
{
	return m_client != nullptr;
}
