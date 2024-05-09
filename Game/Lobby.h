#pragma once
#include "../stdafx.h"

class Room;
class Lobby
{
private:
	wstring m_lobbyName;
	unsigned char m_lobbyType;

	list<Peer*> m_peerList;
	list<Room*> m_roomList;
	Lock m_lock;

	int m_roomNumberSeed = 1;
public:
	Lobby(wstring name, unsigned char type = (unsigned char)0);
	~Lobby();

	bool AddPeer(Peer* peer);
	bool ClosePeer(Peer* peer);
	Room* CreateRoom(wstring name, Hashtable* hash);
	void SendUpdateRoomList();

	wstring& GetName();
	Room* GetRoom(wstring name);
	int GetRoomCount();
	int GetPlayerCount();
	Hashtable* GetRoomListToHashtable();

};