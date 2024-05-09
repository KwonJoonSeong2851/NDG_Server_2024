#pragma once
#include "../stdafx.h"

class Lobby;
class Room
{
private:
	int m_actorNumberSeed = 1;
	int m_masterClientId =  1;
	list<Peer*> m_playerList;
	Lobby* m_currentLobby;

	wstring m_name;
	int m_roomNumber = -1;
	vector<wstring> m_propertiesListedInLobby;
	Hashtable m_roomProperties;
	Hashtable m_playersProperties;

	unsigned char m_maxPlayer = 0;
	int m_emptyRoomTtl = 0;
	int m_playerTtl = 0;
	bool m_isOpen = true;
	bool m_isVisible = true;
	bool m_autoCleanUp = true;

	Lock m_lock;

public:
	Room(wstring name, Hashtable* hash, Lobby* looby, int roomNumber);
	~Room();

	bool AddPeer(Peer* peer);
	void CacheProperties(Hashtable* roomProperties);
	void OnRoomPropertiesChanged(Hashtable* changedHash);
	void OnPlayerPropertiesChanged(int target, Hashtable* changedHash);
	void OnInstantiationGO(Peer* sender, Object* eventContent);
	void OnSerialize(Peer* sender, Object* eventContent);

	wstring GetName();
	list<Peer*>& GetPlayerList();
	/// <summary>
	/// Response에 필요한 방의 정보를 Hashtable에 입력후 반환합니다
	/// </summary>
	/// <param name="lobby">true일시 lobby상태에서 필요한 정보만 반환합니다 </param>
	/// <returns></returns>
	Hashtable* RoomInfoToHashtable(bool lobby);
	Hashtable* GetPlayersProperties();
};