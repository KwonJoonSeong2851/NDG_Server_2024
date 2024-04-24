#pragma once
#include "../stdafx.h"


class Packet;

/// <summary>
/// �ٸ� �͹̳ΰ��� ���� �� ������ �ۼ����� �����ϴ� Ŭ�����Դϴ�.
/// </summary>
class TerminalSession : public Session
{
public:
	bool ConnectTo(char* ip, int port);
	void OnSend(size_t transferSize);
	void SendPacket(Packet* packet);
	Package* OnRecv(size_t transferSize);

	void ReadPing(char* buf);
	void SendPing();
};