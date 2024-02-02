#include "../../stdafx.h"
#include "PacketAnalyer.h"

Packet* PacketAnalyzer::Analyzer(const char* rowPacket, size_t size)
{
	StreamBuffer* stream = new StreamBuffer((byte*)rowPacket, size);


	//Packet* packet = IOCPServer::GetInstance().GetProtocol()->DeserializePacket(stream);

	return nullptr;
}
