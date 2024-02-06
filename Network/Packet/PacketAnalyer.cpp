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
	StreamBuffer stream((byte*)rowPacket, size);


	Packet* packet = protocol->DeserializePacket(stream);

	return packet;
}

Protocol* PacketAnalyzer::GetProtocol()
{
	return protocol;
}
