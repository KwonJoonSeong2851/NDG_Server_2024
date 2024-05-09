#include "Lobby.h"


Lobby::Lobby(wstring name, unsigned char type) : m_lobbyName(name), m_lobbyType(type), m_lock(wstring(L"Lobby:" + name).c_str())
{
	SLOG(L"Lobby Create : %s", m_lock.Name());
}

Lobby::~Lobby()
{
	vector<Peer*> removeVec;
	removeVec.resize(m_peerList.size());
	std::copy(m_peerList.begin(), m_peerList.end(), removeVec.begin());

	for (auto curPeer : removeVec)
	{
		curPeer->OnLeftLobby();
	}
	m_peerList.clear();
}

bool Lobby::AddPeer(Peer* peer)
{
	SAFE_LOCK(m_lock);
	if (peer != nullptr)
	{
		m_peerList.push_back(peer);
		peer->OnJoinedLobby(this);
		return true;
	}
	return false;
}

bool Lobby::ClosePeer(Peer* peer)
{
	SAFE_LOCK(m_lock);
	auto closePeer = find(m_peerList.begin(), m_peerList.end(), peer);
	if (closePeer != m_peerList.end())
	{
		(*closePeer)->OnLeftLobby();
		m_peerList.erase(closePeer);
		return true;
	}
	return false;
}


Room* Lobby::CreateRoom(wstring name, Hashtable* hash)
{
	SAFE_LOCK(m_lock);
	Room* room = new Room(name, hash, this, m_roomNumberSeed);
	++m_roomNumberSeed;
	m_roomList.push_back(room);
	SendUpdateRoomList();
	SLOG(L"Lobby [%s]: Created Room !!! Name[%s], RoomNumber[%d]", m_lobbyName.c_str(), room->GetName().c_str());
	return room;
}

void Lobby::SendUpdateRoomList()
{
	SAFE_LOCK(m_lock);
	for (auto p : m_peerList)
	{
		p->OnGetRoomList();
	}
}

wstring& Lobby::GetName()
{
	return m_lobbyName;
}

Room* Lobby::GetRoom(wstring name)
{
	auto iter = find_if(m_roomList.begin(), m_roomList.end(), [name](Room* room) {return room->GetName() == name; });
	if (iter != m_roomList.end())
		return *iter;

	return nullptr;
}

int Lobby::GetRoomCount()
{
	return m_roomList.size();
}

int Lobby::GetPlayerCount()
{
	return m_peerList.size();
}

Hashtable* Lobby::GetRoomListToHashtable()
{
	Hashtable* hash = new Hashtable();
	for (auto room : m_roomList)
	{
		hash->Insert(make_pair(new wstring_(room->GetName()), room->RoomInfoToHashtable(true)));
	}

	return hash;
}
