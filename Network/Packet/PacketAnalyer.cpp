#include "../../stdafx.h"
#include "PacketAnalyer.h"
PacketAnalyzer::PacketAnalyzer()
{
	m_protocol = new Protocol18();
}

PacketAnalyzer::~PacketAnalyzer()
{
}

Packet* PacketAnalyzer::Analyzer(const char* rowPacket, size_t size)
{

	//StreamBuffer stream((byte*)rowPacket, size);
	cout << "packetData:" << endl;
	for (int i = 0; i < size; ++i)
	{
		cout << (unsigned char)(rowPacket[i]) << " ";
	}
	cout << endl << endl;

	unsigned char messageType = (const unsigned char)rowPacket[8];

	//Packet* packet = nullptr;

	switch (messageType)
	{
		case (unsigned char)0:
		{
			PK_InitRequest* pk = new PK_InitRequest();
			byte* data = new byte[size];
			memcpy_s(data, size, rowPacket, size);
			pk->size = size;
			pk->data = data;
			return (Packet*)pk;
		}

		case (unsigned char)2:
		{
			StreamBuffer stream((unsigned char*)rowPacket + 9, size - 9);
			PK_OperationRequest* op = new PK_OperationRequest();
			op->m_operationCode = stream.ReadByte();
			op->m_parameters = this->m_protocol->ReadParameterTable(stream);
			return (Packet*)op;
		}

	


	default:
		break;
	}

	//Packet* packet = protocol->DeserializePacket(stream);

	return nullptr;
}

Protocol* PacketAnalyzer::GetProtocol()
{
	return m_protocol;
}
