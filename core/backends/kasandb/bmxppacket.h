
#ifndef BMXPPACKET_H_MHVDZ2PE
#define BMXPPACKET_H_MHVDZ2PE

#include <memory>
#include <QByteArray>

struct BmxpHeader
{
	uint8_t signature;
	uint8_t endpoint;
	uint8_t cmd;
	uint8_t subcmd;
	uint16_t flags;
	uint16_t data_length;
} __attribute__((__packed__));

class BmxpPacket
{
public:
	typedef std::shared_ptr<BmxpPacket> Ptr;

	BmxpPacket(int cmd, int subcmd, const QByteArray& data = QByteArray());
	~BmxpPacket();

	int cmd() const { return m_cmd; }
	int subcmd() const { return m_subcmd; }
	int endpoint() const { return m_endpoint; }
	QByteArray& data() { return m_data; }

	void setEndpoint(int ep) { m_endpoint = ep; }

	QByteArray makeRawData();
	static BmxpPacket::Ptr fromRawData(const QByteArray& data);

private:
	int m_cmd;
	int m_subcmd;
	int m_endpoint;
	QByteArray m_data;
};

#endif /* end of include guard: BMXPPACKET_H_MHVDZ2PE */
