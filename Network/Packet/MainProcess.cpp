
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

	case GameEventCode::RPC:
		session->GetPeer()->GetCurrentRoom()->OnRPC(session->GetPeer(), eventContent);
		break;

	}
}


void MainProcess::Authenticate(Session* session, Packet* rowPacket)
{
}



