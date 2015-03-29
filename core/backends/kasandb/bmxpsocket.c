
#include "bmxpsocket.h"

BmxpSocket::BmxpSocket(QAbstractSocket* socket) : m_socket(socket),
	m_state(State::ReadingHeader),
	m_inPtr(0),
	m_expectedData(0)
{
	connect(socket, SIGNAL(readyRead()), this, SLOT(availableData()));
}

BmxpSocket::~BmxpSocket()
{
}

void BmxpSocket::sendPacket(const BmxpPacket::Ptr& packet)
{
	m_socket->write(packet->makeRawData());
}

void BmxpSocket::availableData()
{
	auto socketData = m_socket->read(65536);
	uint8_t* data = (uint8_t*)socketData.data();
	int dataleft = socketData.size();
	while(dataleft > 0)
	{
		switch(m_state)
		{
			case State::ReadingHeader:
				{
					int tocopy = std::min(dataleft, 8 - m_inPtr);
					memcpy(m_inHeader + m_inPtr, data, tocopy);
					dataleft -= tocopy;
					m_inPtr += tocopy;
					data += tocopy;
					if(dataleft <= 0)
						break;
					BmxpHeader* header = reinterpret_cast<BmxpHeader*>(m_inHeader);
					if(header->data_length > 0)
					{
						m_expectedData = dataleft;
						m_state = State::ReadingData;
						m_inPtr = 0;
						m_incomingData.clear();
					}
					else
					{
						auto packet = std::make_shared<BmxpPacket>(header->cmd, header->subcmd);
						emit incomingPacket(packet);
						m_inPtr = 0;
						break;
					}
				}
				// Fallthrough

			case State::ReadingData:
				{
					int tocopy = std::min(m_expectedData, dataleft);
					int currentLength = m_incomingData.size();
					m_incomingData.resize(currentLength + tocopy);
					memcpy(m_incomingData.data() + currentLength, data, tocopy);
					dataleft -= tocopy;
					data += tocopy;
					m_expectedData -= tocopy;
					if(m_expectedData <= 0)
					{
						BmxpHeader* header = reinterpret_cast<BmxpHeader*>(m_inHeader);
						auto packet = std::make_shared<BmxpPacket>(header->cmd, header->subcmd, m_incomingData);
						emit incomingPacket(packet);
						m_state = State::ReadingHeader;
					}
					break;
				}
		}
	}
}
