#include "Room.h"

Room::Room(wstring name, Hashtable* hash, Lobby* lobby,int roomNumber) : m_name(name), m_currentLobby(lobby), m_lock(wstring(L"Room_" + roomNumber).c_str())
{
	CacheProperties(hash);
}

Room::~Room()
{
}

bool Room::AddPeer(Peer* peer)
{
	SAFE_LOCK(m_lock);
	peer->SetActorNumber(m_actorNumberSeed++);
	for (auto p : m_playerList)
		p->OnOtherPlayerEnteredRoom(peer);
	m_playerList.push_back(peer);
	m_playersProperties.Insert(make_pair(new generic_<int>(peer->GetActorNumber()), peer->GetProperties()));
	peer->OnJoinedRoom(this);

	return true;
}

void Room::CacheProperties(Hashtable* roomProperties)
{
	SAFE_LOCK(m_lock);
	Hashtable& hash = *roomProperties;

	if(hash.Find((byte)RoomPropertyKey::IsOpen) != hash.End())
		m_isOpen = **(generic_<byte>*)hash[(byte)RoomPropertyKey::IsOpen];

	if(hash.Find((byte)RoomPropertyKey::IsVisible) != hash.End())
		m_isVisible = **(generic_<byte>*)hash[(byte)RoomPropertyKey::IsVisible];

	if (hash.Find((byte)RoomPropertyKey::MaxPlayers) != hash.End())
		m_maxPlayer = **(generic_<byte>*)hash[(byte)RoomPropertyKey::MaxPlayers];

	if(hash.Find((byte)RoomPropertyKey::PropsListedInLobby) != hash.End())
	{
		vector<wstring_>& vec = **(vector_<wstring_>*)hash[(byte)RoomPropertyKey::PropsListedInLobby];
		for (auto s : vec)
		{
			m_propertiesListedInLobby.push_back(*s);
		}
	}
	
	m_roomProperties.MergeStringKey(&hash);
}

void Room::OnRoomPropertiesChanged(Hashtable* changedHash)
{
	SAFE_LOCK(m_lock);

	PK_EventData* ev = new PK_EventData();
	ev->SetSender(0);
	ev->m_eventCode = EventCode::PropertiesChanged;
	ev->m_parameters->insert(make_pair((byte)ParameterCode::Properties, changedHash->Copy()));


	BroadcastPacket(nullptr, ev, BroadcastTarget::All);

	SAFE_DELETE(ev);
}

void Room::OnPlayerPropertiesChanged(int target, Hashtable* changedHash)
{
	SAFE_LOCK(m_lock);
	//현재 룸에 타겟 플레이어의 프로퍼티를 캐싱합니다.
	//int targetNum = target->GetActorNumber();
	auto iter = m_playersProperties.Find(target);
	if (iter != m_playersProperties.End())
	{
		((Hashtable*)(iter->second))->MergeStringKey(changedHash);
	}
	else
	{
		m_playersProperties.Insert(make_pair(new generic_<int>(target),changedHash->Copy()));
	}

	//업데이트된 해시 정보를 다른 플레이어들에게 브로드캐스팅합니다.
	PK_EventData* ev = new PK_EventData();
	//ev->SetSender();
	ev->m_eventCode = EventCode::PropertiesChanged;
	ev->m_parameters->insert(make_pair((byte)ParameterCode::TargetActorNr, new generic_<int>(target)));
	ev->m_parameters->insert(make_pair((byte)ParameterCode::Properties, changedHash->Copy()));

	BroadcastPacket(nullptr, ev, BroadcastTarget::All);

	SAFE_DELETE(ev);

}

void Room::OnInstantiationGO(Peer* sender, Object* eventContent)
{
	PK_EventData* ev = new PK_EventData();
	ev->SetSender(sender->GetActorNumber());
	ev->m_eventCode = GameEventCode::Instantiation;
	ev->SetCustomData(eventContent->Copy());

	BroadcastPacket(sender, ev, BroadcastTarget::Others);

	SAFE_DELETE(ev);
}

void Room::OnSerialize(Peer* sender, Object* eventContent)
{
	PK_EventData* ev = new PK_EventData();
	int senderActorNum = sender->GetActorNumber();
	ev->SetSender(senderActorNum);
	ev->m_eventCode = GameEventCode::SendSerialize;
	ev->m_parameters->insert(make_pair((byte)ParameterCode::Data, eventContent->Copy()));

	BroadcastPacket(sender, ev, BroadcastTarget::Others);

	SAFE_DELETE(ev);
}

void Room::OnRPC(Peer* sender, Object* eventContent)
{
	PK_EventData* ev = new PK_EventData();
	ev->SetSender(sender->GetActorNumber());
	ev->m_eventCode = GameEventCode::RPC;
	ev->SetCustomData(eventContent->Copy());

	BroadcastPacket(sender, ev, BroadcastTarget::Others);

	SAFE_DELETE(ev);
}

void Room::BroadcastPacket(Peer* sender, Packet* packet, const byte target)
{
	SAFE_LOCK(m_lock);
	switch (target)
	{
	case BroadcastTarget::All:
		for (auto p : m_playerList)
			p->SendPacket(packet);
		break;

	case BroadcastTarget::Others:
		for (auto p : m_playerList)
		{
			if (p->GetActorNumber() != sender->GetActorNumber())
				p->SendPacket(packet);
		}
		break;
	}
}

wstring Room::GetName()
{
	return m_name;
}

list<Peer*>& Room::GetPlayerList()
{
	return m_playerList;
}

Hashtable* Room::RoomInfoToHashtable(bool isLobby)
{
	SAFE_LOCK(m_lock);
	Hashtable* hash = new Hashtable();
	hash->Insert(make_pair(new generic_<byte>((byte)RoomPropertyKey::MaxPlayers), new generic_<byte>(m_maxPlayer)));
	hash->Insert(make_pair(new generic_<byte>((byte)RoomPropertyKey::IsOpen), new generic_<bool>(m_isOpen)));
	hash->Insert(make_pair(new generic_<byte>((byte)RoomPropertyKey::IsVisible), new generic_<bool>(m_isVisible)));
	hash->Insert(make_pair(new generic_<byte>((byte)RoomPropertyKey::PlayerCount), new generic_<byte>((byte)m_playerList.size())));
	hash->Insert(make_pair(new generic_<byte>((byte)RoomPropertyKey::MasterClientId), new generic_<int>(m_masterClientId)));

	vector_<wstring_>* copy = new vector_<wstring_>(m_propertiesListedInLobby.size());
	for (int i = 0; i = m_propertiesListedInLobby.size(); ++i)
	{
		(*copy)[i] = (wstring_)m_propertiesListedInLobby[i];
	}
	hash->Insert(make_pair(new generic_<byte>((byte)RoomPropertyKey::PropsListedInLobby), copy));


	if (isLobby == false)
	{
		hash->MergeStringKey(&m_roomProperties);
	}

	return hash;
}

Hashtable* Room::GetPlayersProperties()
{
	return (Hashtable*)m_playersProperties.Copy();
}
