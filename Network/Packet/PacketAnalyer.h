#pragma once
#include "../../stdafx.h"
//#include "../StreamBuffer.h"

class StreamBuffer;
/// <summary>
///  rowPacket�� �޾ƿ� �ش� packetŬ������ ã�� �� ���� �����Ͱ�������
/// packet�� �����͸� �����մϴ�.
/// </summary>
/// 
class PacketAnalyzer : public Singleton<PacketAnalyzer>
{
public:
	Packet* Analyzer(const char* rowPacket, size_t size);
};
