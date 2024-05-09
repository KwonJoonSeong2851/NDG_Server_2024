#pragma once
#include "../../stdafx.h"
#include "ContentsProcess.h"
class MainProcess : public ContentsProcess
{
public:
	MainProcess();

private:
	void RegistSubPacketFunc();
	static void InitRequestProcess(Session* session, Packet* rowPacket);
	static void OperationRequestProcess(Session* seesion, Packet* rowPacket);
	static void EventDataProcess(Session* session, byte eventCode, Object* eventContent);
	
	static void Authenticate(Session* session, Packet* rowPacket);

};