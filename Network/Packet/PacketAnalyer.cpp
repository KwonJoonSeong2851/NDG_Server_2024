#include "../../stdafx.h"
#include "PacketAnalyer.h"
PacketAnalyzer::PacketAnalyzer()
{
	protocol = new Protocol18();
}

PacketAnalyzer::~PacketAnalyzer()
{
}

Packet* PacketAnalyzer::Analyzer(const char* rowPacket, size_t size)
{
	//StreamBuffer stream((byte*)rowPacket, size);

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

	default:
		break;
	}

	//Packet* packet = protocol->DeserializePacket(stream);

	return nullptr;
}

Protocol* PacketAnalyzer::GetProtocol()
{
	return protocol;
}
