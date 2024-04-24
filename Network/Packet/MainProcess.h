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
	static void C_REQ_ID_PW(Session* session, Packet* rowPacket);
	static void C_OperationRequest(Session* seesion, Packet* rowPacket);
	//static void C_REQ_SIGNIN(Session* session, Packet* rowPacket);
	//static void C_REQ_USERINFO(Session* session, Packet* rowPacket);

	//static void C_REQ_GAMERESULT(Session* session, Packet* rowPacket);
	//static void C_REQ_CHATTING(Session* session, Packet* rowPacket);

	//static void C_REQ_GET_TUTORIAL(Session* session, Packet* rowPacket);
	//static void C_REQ_SET_TUTORIAL(Session* session, Packet* rowPacket);

};