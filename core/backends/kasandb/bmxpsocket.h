
#ifndef BMXPSOCKET_H_GBWL3GNQ
#define BMXPSOCKET_H_GBWL3GNQ

#include <QObject>
#include <QAbstractSocket>
#include "bmxppacket.h"

class BmxpSocket : public QObject
{
	Q_OBJECT
public:
	BmxpSocket(QAbstractSocket* socket);
	virtual ~BmxpSocket();

	void sendPacket(const BmxpPacket::Ptr& packet);

signals:
	void incomingPacket(const BmxpPacket::Ptr& packet);

private slots:
	void availableData();

private:
	QAbstractSocket* m_socket;

	enum class State
	{
		ReadingHeader,
		ReadingData
	};
	State m_state;
	int m_inPtr;
	uint8_t m_inHeader[8];
	int m_expectedData;
	QByteArray m_incomingData;
};

#endif /* end of include guard: BMXPSOCKET_H_GBWL3GNQ */
