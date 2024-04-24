#pragma once
#include "../stdafx.h"


class Packet;

/// <summary>
/// 다른 터미널과의 연결 및 데이터 송수신을 지원하는 클래스입니다.
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