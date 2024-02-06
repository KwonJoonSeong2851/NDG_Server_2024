#pragma once
#include "../../stdafx.h"

/// <summary>
///  rowPacket�� �޾ƿ� �ش� packetŬ������ ã�� �� ���� �����Ͱ�������
/// packet�� �����͸� �����մϴ�.
/// </summary>
/// 
class PacketAnalyzer : public Singleton<PacketAnalyzer>
{
private:
	Protocol* protocol;
public:
	PacketAnalyzer();
	~PacketAnalyzer();
	Packet* Analyzer(const char* rowPacket, size_t size);

	Protocol* GetProtocol();
};
