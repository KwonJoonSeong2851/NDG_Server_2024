#pragma once
#include "../stdafx.h"
#define LOBBY_CAPACITY (50)

class Lobby;
class LobbyManager : public Singleton<LobbyManager>
{
private:
	unordered_map<wstring, Lobby*> m_lobbyMap;
	int m_lobbyCount;
	int m_maxLobbyCount;
	Lock m_lock;

public:
	LobbyManager();
	~LobbyManager();

	bool AddLobby(Lobby* lobby);
	bool CloseLobby(Lobby* lobby);
	Lobby* CreateLobby(wstring name, unsigned char type = 0);
	unordered_map<wstring, Lobby*>& GetLobbyMap();
	Lobby* GetLobby(wstring lobbyName, unsigned char type = 0);

	Lock& GetLock();

};