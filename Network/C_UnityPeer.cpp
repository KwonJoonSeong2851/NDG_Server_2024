#include "C_UnityPeer.h"

C_UnityPeer::C_UnityPeer(Session* session) : Peer(session)
{

}

C_UnityPeer::~C_UnityPeer()
{
}

void C_UnityPeer::InitPeer()
{
	SLOG(L"C_UnityPeer : The new peer has been initialized. !!!");
	m_peerStatus = PeerStatusCode::Initialized;
	OnStatusChanged(PeerStatusCode::Initialized);
}

void C_UnityPeer::InitCallback()
{
}

void C_UnityPeer::OnStatusChanged(PeerStatusCode code)
{
}

void C_UnityPeer::OnJoinedLobby(Lobby* lobby)
{
	Peer::OnJoinedLobby(lobby);
}

void C_UnityPeer::OnLeftLobby()
{
	Peer::OnLeftLobby();
}

void C_UnityPeer::OnCreateRoom(Room* room)
{
	Peer::OnCreateRoom(room);
}

void C_UnityPeer::OnJoinedRoom(Room* room)
{
	Peer::OnJoinedRoom(room);

	PK_OperationResponse* op = new PK_OperationResponse();
	op->m_operationCode = (byte)OperationCode::JoinRoom;
	op->m_parameters->insert(make_pair((byte)ParameterCode::ActorNr, new generic_<int>(m_actorNumber)));

	auto list = room->GetPlayerList();
	if (list.size() > 1)
	{
		vector_<int>* playerList = new vector_<int>(list.size());
		int i = 0;

		for (auto p : list)
		{
			(**playerList)[i] = p->GetActorNumber();
			i++;
		}

		op->m_parameters->insert(make_pair((byte)ParameterCode::ActorList, playerList));
	}

	op->m_parameters->insert(make_pair((byte)ParameterCode::RoomProperties, room->RoomInfoToHashtable(false)));
	op->m_parameters->insert(make_pair((byte)ParameterCode::PlayerProperties, room->GetPlayersProperties()));

	this->SendPacket(op);

	SAFE_DELETE(op);
}

void C_UnityPeer::OnLeftRoom()
{
	Peer::OnLeftRoom();
}

void C_UnityPeer::OnGetRoomList()
{
	Peer::OnGetRoomList();
	PK_OperationResponse* op = new PK_OperationResponse();
	op->m_operationCode = (byte)OperationCode::GetRoomList;
	op->m_parameters->insert(make_pair((byte)ParameterCode::GameList, m_currentLobby->GetRoomListToHashtable()));

	this->SendPacket(op);

	SAFE_DELETE(op);
}

void C_UnityPeer::OnOtherPlayerEnteredRoom(Peer* other)
{
	Peer::OnOtherPlayerEnteredRoom(other);
	PK_EventData* ev = new PK_EventData();
	ev->SetSender(other->GetActorNumber());
	ev->m_eventCode = EventCode::Join;
	ev->m_parameters->insert(make_pair((byte)ParameterCode::PlayerProperties,other->GetProperties()->Copy()));
	this->SendPacket(ev);

	SAFE_DELETE(ev);

}




