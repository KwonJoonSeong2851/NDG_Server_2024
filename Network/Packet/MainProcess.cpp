
#include "MainProcess.h"
#include "../C_UnityPeer.h"

MainProcess::MainProcess()
{
	this->RegistSubPacketFunc();
}

void MainProcess::RegistSubPacketFunc()
{
#define INSERT_PACKET_PROCESS(type) m_runFuncTable.insert(make_pair(E_##type, &MainProcess::##type##Process))

	INSERT_PACKET_PROCESS(InitRequest);
	INSERT_PACKET_PROCESS(OperationRequest);
	//INSERT_PACKET_PROCESS(EventData);

}

void MainProcess::InitRequestProcess(Session* session, Packet* rowPacket)
{
	PK_InitRequest* pk = (PK_InitRequest*)rowPacket;

	byte* numArray = pk->data;

	if (numArray[0] == (unsigned char)251)
	{
		Peer* peer = new C_UnityPeer(session);
		session->SetPeer(peer);
	}

	PK_InitResponse* sendPacket = new PK_InitResponse();
	session->SendPacket(sendPacket);
	
	SAFE_DELETE(sendPacket);
	return;
}

void MainProcess::OperationRequestProcess(Session* session, Packet* rowPacket)
{
	PK_OperationRequest& op = *(PK_OperationRequest*)rowPacket;
	SLOG(L"OperationRequestProcess : OperationCode = %d, ParamSize = %d", (int)op.m_operationCode, op.m_parameters->size());

	switch ((OperationCode)op.m_operationCode)
	{
	case OperationCode::JoinLobby:
	{
		Lobby* lobby = LobbyManager::GetInstance().GetLobby(**(wstring_*)op[ParameterCode::LobbyName], **(generic_<byte>*)op[ParameterCode::LobbyType]);
		if (lobby->AddPeer(session->GetPeer()))
		{
			PK_OperationResponse* sendPacket = new PK_OperationResponse();
			sendPacket->m_operationCode = (byte)OperationCode::JoinLobby;
			session->SendPacket(sendPacket);
		}
		goto GetRoomList;
	}
	break;

	case OperationCode::CreateRoom:
	{
		wstring roomName = **(wstring_*)op[ParameterCode::RoomName];
		wstring lobbyName = **(wstring_*)op[ParameterCode::LobbyName];
		session->GetPeer()->CacheProperties((Hashtable*)op[ParameterCode::PlayerProperties]);
		Room* room = LobbyManager::GetInstance().GetLobby(lobbyName)->CreateRoom(roomName, (Hashtable*)op[ParameterCode::RoomProperties]);
		room->AddPeer(session->GetPeer());
	}
		break;

	case OperationCode::JoinRoom:
	{
		wstring roomName = **(wstring_*)op[ParameterCode::RoomName];
		wstring lobbyName = **(wstring_*)op[ParameterCode::LobbyName];
		session->GetPeer()->CacheProperties((Hashtable*)op[ParameterCode::PlayerProperties]);
		Room* room = LobbyManager::GetInstance().GetLobby(lobbyName)->GetRoom(roomName);
		if (room != nullptr)
		{
			room->AddPeer(session->GetPeer());
		}
	}
	break;

	case OperationCode::GetRoomList:
		GetRoomList:
		session->GetPeer()->OnGetRoomList();
		return;

	case OperationCode::SetProperties:
	{
		SLOG(L"Operation :: SetProperties ");
		Hashtable* properties = (Hashtable*)op[ParameterCode::Properties];
		Peer* peer = session->GetPeer();

		//TargetActorNumber가 포함되어있을 경우 Player Properties;
		//포함되어 있지 않을경우 Room Properties로 간주합니다.
		if (op.m_parameters->find((byte)ParameterCode::ActorNr) != op.m_parameters->end())
		{
			SLOG(L"Player properties changed");
			int targetActor = **(generic_<int>*)op[ParameterCode::ActorNr];
			peer->CacheProperties(properties);
			peer->GetCurrentRoom()->OnPlayerPropertiesChanged(targetActor, properties);
		}
		else
		{
			SLOG(L"Room properties changed");
			peer->GetCurrentRoom()->CacheProperties(properties);
			peer->GetCurrentRoom()->OnRoomPropertiesChanged(properties);
		}
	}
		break;

	case OperationCode::RaiseEvent:
		byte eventCode = **(generic_<byte>*)op[ParameterCode::Code];
		SLOG(L"Operation :: RaiseEvent : Code = %d", eventCode);
		Object* eventContent = nullptr;
		if (op.m_parameters->find((byte)ParameterCode::Data) != op.m_parameters->end())
		{
			eventContent = op[ParameterCode::Data];
		}
		EventDataProcess(session, eventCode, eventContent);
		break;

	}

	return;
}

void MainProcess::EventDataProcess(Session* session, byte eventCode, Object* eventContent)
{
	if (eventContent == nullptr)
		return;

	switch (eventCode)
	{
	case GameEventCode::Instantiation:
		SLOG(L"GAME EVENT : Instatiation : sender[%d]", session->GetPeer()->GetActorNumber());
		session->GetPeer()->GetCurrentRoom()->OnInstantiationGO(session->GetPeer(), eventContent);
		break;

	case GameEventCode::SendSerialize:
	case GameEventCode::SendSerializeReliable:
		session->GetPeer()->GetCurrentRoom()->OnSerialize(session->GetPeer(),eventContent);
		break;

	}
}

//void MainProcess::EventDataProcess(Session* session, PK_EventData* rowPacket)
//{
//	PK_EventData* ev = (PK_EventData*)rowPacket;
//	switch (ev->m_eventCode)
//	{
//	case GameEventCode::Instantiation:
//
//		break;
//	}
//}

void MainProcess::Authenticate(Session* session, Packet* rowPacket)
{
}





//
//void MainProcess::C_REQ_SIGNIN(Session* session, Packet* rowPacket)
//{
//	PK_C_REQ_SIGNIN* packet = (PK_C_REQ_SIGNIN*)rowPacket;
//
//	QI_DB_REQ_SIGNIN* query = new QI_DB_REQ_SIGNIN();
//	query->clientId = session->GetId();
//
//
//	array<WCHAR, SIZE_256> nickName;
//
//
//
//
//	int len = MultiByteToWideChar(CP_UTF8, 0, packet->userNickName.c_str(), -1, NULL, 0);
//	MultiByteToWideChar(CP_UTF8, 0, packet->userNickName.c_str(), -1, nickName.data(), len);
//
//
//	QueryStatement* statement = query->GetStatement();
//	statement->AddParam((char*)packet->userId.c_str());
//	statement->AddParam((char*)packet->userPwd.c_str());
//	statement->AddParam(nickName.data());
//
//	
//	statement->EndParam();
//	DBManager::GetInstance().PushQuery(query);
//
//
//}
//
//void MainProcess::C_REQ_USERINFO(Session* session, Packet* rowPacket)
//{
//	PK_C_REQ_USERINFO* packet = (PK_C_REQ_USERINFO*)rowPacket;
//
//	QI_DB_REQ_USERINFO* query = new QI_DB_REQ_USERINFO();
//	query->clientId = session->GetId(); 
//
//	QueryStatement* statement = query->GetStatement();
//	statement->AddParam((UINT64*)packet->account);
//	statement->EndParam();
//	DBManager::GetInstance().PushQuery(query);
//}
//
//void MainProcess::C_REQ_GAMERESULT(Session* session, Packet* rowPacket)
//{
//	PK_C_REQ_GAMERESULT* packet = (PK_C_REQ_GAMERESULT*)rowPacket;
//	QI_DB_REQ_GAMERESULT* query = new QI_DB_REQ_GAMERESULT();
//	query->clientId = session->GetId();
//
//	QueryStatement* statement = query->GetStatement();
//	statement->AddParam((UINT64*)packet->account);
//	statement->AddParam((UINT32*)packet->gameResult);
//	statement->AddParam((UINT64*)packet->experience);
//	statement->EndParam();
//	DBManager::GetInstance().PushQuery(query);
//}
//
//
//
//
//void MainProcess::C_REQ_CHATTING(Session* session, Packet* rowPacket)
//{
//	PK_C_REQ_CHATTING* packet = (PK_C_REQ_CHATTING*)rowPacket;
//	PK_S_ANS_CHATTING sendPacket;
//
//	array<WCHAR, SIZE_256> buff;
//
//	int len = MultiByteToWideChar(CP_UTF8, 0, packet->m_id.c_str(), -1, NULL, 0);
//	MultiByteToWideChar(CP_UTF8, 0, packet->m_id.c_str(), -1, buff.data(), len);
//	sendPacket.m_id = buff.data(); 
//	buff.fill(NULL);
//
//	len = MultiByteToWideChar(CP_UTF8, 0, packet->m_text.c_str(), -1, NULL, 0);
//	MultiByteToWideChar(CP_UTF8, 0, packet->m_text.c_str(), -1, buff.data(), len);
//	sendPacket.m_text = buff.data();
//
//	
//	sendPacket.characterType = packet->characterType;
//	sendPacket.team = packet->team;
//	sendPacket.session = packet->session;
//	sendPacket.roomNumber = packet->roomNumber;
//
//	SAFE_LOCK(SessionManager::GetInstance().GetLock());
//	auto playerList = SessionManager::GetInstance().GetSessionList();
//
//	SLOG(L"Player list length: %d", playerList.size());
//
//	for (auto player : playerList)
//	{
//		player->SendPacket(&sendPacket);
//	}
//
//	return;
//}
//void MainProcess::C_REQ_GET_TUTORIAL(Session* session, Packet* rowPacket)
//{
//	PK_C_REQ_GET_TUTORIAL* packet = (PK_C_REQ_GET_TUTORIAL*)rowPacket;
//
//	QI_DB_REQ_GET_TUTORIAL* query = new QI_DB_REQ_GET_TUTORIAL();
//	query->clientId = session->GetId();
//
//
//	QueryStatement* statement = query->GetStatement();
//	statement->AddParam((UINT64*)packet->accountId);
//	statement->EndParam();
//	DBManager::GetInstance().PushQuery(query);
//
//
//	SLOG(L"TUTORIAL Player Account: %u", packet->accountId);
//	
//	cout << packet->accountId << endl;
//}
//
//
//
//void MainProcess::C_REQ_SET_TUTORIAL(Session* session, Packet* rowPacket)
//{
//	PK_C_REQ_SET_TUTORIAL* packet = (PK_C_REQ_SET_TUTORIAL*)rowPacket;
//
//	QI_DB_REQ_SET_TUTORIAL* query = new QI_DB_REQ_SET_TUTORIAL();
//	query->clientId = session->GetId();
//
//	QueryStatement* statement = query->GetStatement();
//	statement->AddParam((UInt64*)packet->accountId);
//	statement->EndParam();
//	DBManager::GetInstance().PushQuery(query);
//
//}
//

