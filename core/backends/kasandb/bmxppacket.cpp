
#include "bmxppacket.h"
#include "bmxpcommands.h"
#include "exceptions.h"

BmxpPacket::BmxpPacket(int cmd, int subcmd, const QByteArray& data) :
	m_cmd(cmd), m_subcmd(subcmd), m_endpoint(0), m_data(data)
{
	if(data.size() >= 65536)
	{
		BOOST_THROW_EXCEPTION(ProtocolError() << error_message("Too long packet"));
	}
}

BmxpPacket::~BmxpPacket()
{
}

QByteArray BmxpPacket::makeRawData()
{
	QByteArray data;
	data.append(Bmxp::Signature);
	data.append(m_endpoint);
	data.append(m_cmd);
	data.append(m_subcmd);
	data.append((char)0);
	data.append((char)0);
	data.append(m_data.size() & 0xff);
	data.append((m_data.size() >> 8) & 0xff);
	data.append(m_data);
	return data;
}

BmxpPacket::Ptr BmxpPacket::fromRawData(const QByteArray& data)
{
	if((size_t)data.size() < sizeof(BmxpHeader))
	{
		BOOST_THROW_EXCEPTION(ProtocolError() << error_message("Truncated packet"));
	}

	const BmxpHeader* header = reinterpret_cast<const BmxpHeader*>(data.data());
	auto packet = std::make_shared<BmxpPacket>(header->cmd, header->subcmd, data.mid(8));
	packet->setEndpoint(header->endpoint);
	return packet;
}

