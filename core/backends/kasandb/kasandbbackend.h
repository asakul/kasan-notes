#ifndef CORE_BACKENDS_KASANDB_KASANDBBACKEND_H_
#define CORE_BACKENDS_KASANDB_KASANDBBACKEND_H_

#include "core/backends/common/backend.h"
#include <QString>
#include <memory>
#include <QSslSocket>
#include "bmxpsocket.h"

class KasanDbBackend : public QObject, public Backend
{
	Q_OBJECT
public:
	typedef std::shared_ptr<KasanDbBackend> Ptr;
	KasanDbBackend(const QString& hostname, int port);
	virtual ~KasanDbBackend();

signals:
	void authenticationCompleted();

public slots:
	void authenticate(const QString& username, const QString& password);

private slots:
	void incomingPacket(const BmxpPacket::Ptr& packet);

private:
	QString m_username;
	QString m_password;
	QByteArray m_challenge;
	QString m_hostname;
	int m_port;
	QSslSocket m_sslSocket;
	BmxpSocket m_socket;
	QByteArray m_array;

	enum class State
	{
		AuthStart,
		AuthChallenged,
		Authenticated
	};
	State m_state;
};

#endif /* CORE_BACKENDS_KASANDB_KASANDBBACKEND_H_ */
