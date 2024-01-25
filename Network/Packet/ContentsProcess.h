#pragma once
#include "../../stdafx.h"

#define MAX_PACKET_THREAD_ SIZE_64

class Thread;

/// <summary>
/// 수신받은 Package를 큐에 쌓고 해당 runFuncTable_에 등록된 함수 중 package에 맞는
///  함수를 호출해주는 클래스입니다.
/// 해당 클래스를 상속받은 자식클래스가 RegistSubPacketFunc() 함수를 오버라이딩 하여
/// 다른 프로세스를 등록할수 있습니다.
/// </summary>
class ContentsProcess
{
private:
	std::array<Thread*, MAX_PACKET_THREAD_> m_threadPool;
	ThreadJobQueue<Package*>* m_packageQueue;

protected:
	typedef void(*RunFunc)(Session* session, Packet* rowPacket);
	std::unordered_map<PacketType, RunFunc> m_runFuncTable;

private:
	void Initialize(xml_t* config);
	void RegistDefaultPacketFunc();
	void Run(Package* package);
	void Execute();

	void Process();
public:
	ContentsProcess();
	~ContentsProcess();

	void PutPackage(Package* package);
	virtual void RegistSubPacketFunc() = 0;

	static void Packet_HeartBeat(Session* session, Packet* rowPacket) {};
	static void Packet_Notify_Terminal(Session* session, Packet* rowPacket) {};
	static void Packet_Exit(Session* session, Packet* rowPacket) {};
};