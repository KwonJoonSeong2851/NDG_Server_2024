#include "Peer.h"

Peer::Peer(Session* session) : m_session(session)
{
	m_nickName = L"Unkown";
	m_peerStatus = PeerStatusCode::PeerCreate;
	m_properties = new Hashtable();
	m_serverConnection = ServerConnection::MasterServer;
}

Peer::~Peer()
{
	if (m_currentRoom != nullptr)
	{

	}

	if (m_currentLobby != nullptr)
	{
		m_currentLobby->ClosePeer(this);
	}
}

void Peer::SendPacket(Packet* packet)
{
	if(m_session != nullptr)
	m_session->SendPacket(packet);
}

void Peer::InitPeer()
{
}

void Peer::InitCallback()
{
}


void Peer::OnStatusChanged(PeerStatusCode code)
{
}

void Peer::OnJoinedLobby(Lobby* lobby)
{
	m_currentLobby = lobby;
	m_peerStatus = PeerStatusCode::JoinedLobby;
}

void Peer::OnLeftLobby()
{
	m_currentLobby = nullptr;
	m_peerStatus = PeerStatusCode::Initialized;
}

void Peer::OnCreateRoom(Room* room)
{
	m_currentRoom = room;
	m_peerStatus = PeerStatusCode::JoinedRoom;
}

void Peer::OnJoinedRoom(Room* room)
{
	m_currentRoom = room;
	m_peerStatus = PeerStatusCode::JoinedRoom;
}

void Peer::OnLeftRoom()
{
}

void Peer::OnGetRoomList()
{
}

void Peer::OnOtherPlayerEnteredRoom(Peer* other)
{
}

void Peer::OnPropertiesChanged(Hashtable* props)
{
}

void Peer::CacheProperties(Hashtable* hash)
{
	/*delete m_properties;
	m_properties = (Hashtable*)hash->Copy();*/

	if (hash->Find((byte)255) != hash->End())
	{
		m_nickName = **(wstring_*)(*hash)[(byte)255];
		m_properties->Insert(make_pair(new generic_<byte>(255),(*hash)[(byte)255]->Copy()));
	}

	m_properties->MergeStringKey(hash);
}

Hashtable* Peer::GetProperties()
{
	return m_properties;
}

Session* Peer::GetSession()
{
	return m_session;
}

int Peer::GetActorNumber()
{
	return m_actorNumber;
}

void Peer::SetActorNumber(int num)
{
	m_actorNumber = num;
}

Room* Peer::GetCurrentRoom()
{
	return m_currentRoom;
}

ServerConnection Peer::GetServerConnection()
{
	return m_serverConnection;
}

void Peer::SetServerConnection(ServerConnection server)
{
	m_serverConnection = server;
}

