#pragma once
#include "../../stdafx.h"
//#include "../StreamBuffer.h"

class StreamBuffer;
/// <summary>
///  rowPacket을 받아와 해당 packet클래스를 찾고 더 읽을 데이터가있으면
/// packet에 데이터를 삽입합니다.
/// </summary>
/// 
class PacketAnalyzer : public Singleton<PacketAnalyzer>
{
public:
	Packet* Analyzer(const char* rowPacket, size_t size);
};
