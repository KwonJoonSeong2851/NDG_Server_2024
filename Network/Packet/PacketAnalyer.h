#pragma once
#include "../../stdafx.h"
#include "PacketClass.h"
#include "PacketFactory.h"

class StreamBuffer;
/// <summary>
///  rowPacket을 받아와 해당 packet클래스를 찾고 더 읽을 데이터가있으면
/// packet에 데이터를 삽입합니다.
/// </summary>
class PacketAnalyzer : public Singleton<PacketAnalyzer>
{
public:
	inline Packet* Analyzer(const char* rowPacket, size_t size)
	{
		size_t offset = 9;
		//PacketType type[1] = { (PacketType)0, };

		//memcpy_s((void*)type, sizeof(type), (void*)rowPacket, sizeof(type));
		//offset += sizeof(tcpMessage);
		StreamBuffer stream((byte*)rowPacket + offset, size - offset);


		Packet* packet = IOCPServer::GetInstance().GetProtocol()->DeserializePacket(stream);
		//Packet* packet = PacketFactory::GetInstance().GetPacket(type[0]);
	
		return packet;
	}
};
