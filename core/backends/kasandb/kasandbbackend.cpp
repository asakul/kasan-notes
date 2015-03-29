#include "kasandbbackend.h"
#include <QFile>
#include <QThread>
#include <QCryptographicHash>
#include "log.h"
#include "exceptions.h"
#include "bmxppacket.h"
#include "bmxpcommands.h"

static QByteArray hmac(const QByteArray& password, const QByteArray& message)
{
	QByteArray ipadded = password;
	QByteArray opadded = password;
	for(auto& x : ipadded)
	{
		x ^= 0x36;
	}
	for(auto& x : opadded)
	{
		x ^= 0x5c;
	}

	auto step1 = QCryptographicHash::hash(ipadded + message, QCryptographicHash::Sha256);
	auto step2 = QCryptographicHash::hash(opadded + step1, QCryptographicHash::Sha256);
	return step2;
}

KasanDbBackend::KasanDbBackend(const QString& hostname, int port) :
	m_hostname(hostname),
	m_port(port),
	m_sslSocket(this),
	m_socket(&m_sslSocket)
{
	connect(&m_socket, SIGNAL(incomingPacket(BmxpPacket::Ptr)), this, SLOT(incomingPacket(BmxpPacket::Ptr)));
}

KasanDbBackend::~KasanDbBackend()
{
}

void KasanDbBackend::authenticate(const QString& username, const QString& password)
{
	m_username = username;
	m_password = password;
	LOG(DEBUG) << "kasan-db backend: authenticate";
	m_sslSocket.setPeerVerifyMode(QSslSocket::QueryPeer);
	m_sslSocket.connectToHostEncrypted(m_hostname, m_port);
	if(!m_sslSocket.waitForEncrypted())
	{
		BOOST_THROW_EXCEPTION(ConnectionError() << error_message("Unable to establish connection to host"));
	}
	LOG(DEBUG) << "Connection established";
	m_state = State::AuthStart;

	auto packet = std::make_shared<BmxpPacket>(Bmxp::CmdAuth, Bmxp::AuthPrompt);
	m_socket.sendPacket(packet);
}

void KasanDbBackend::incomingPacket(const BmxpPacket::Ptr& packet)
{
	if(m_state == State::AuthStart)
	{
		m_challenge = packet->data();

		LOG(DEBUG) << "Received challenge: " << m_challenge.toStdString();

		auto username = m_username.toUtf8();
		QByteArray resp;
		resp.append(username.size());
		resp.append(username);
		resp.append(hmac(m_password.toUtf8(), m_challenge));
		auto packet = std::make_shared<BmxpPacket>(Bmxp::CmdAuth, Bmxp::AuthResponse, resp);
		m_socket.sendPacket(packet);

		m_state = State::AuthChallenged;
	}
	else if(m_state == State::AuthChallenged)
	{
		if(packet->data().size() < 1)
		{
			LOG(WARNING) << "Invalid response";
		}
		else
		{
			int returnCode = packet->data()[0];
			if(returnCode != 0)
				emit authenticationCompleted();
		}
	}
}
