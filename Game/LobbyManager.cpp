#include "LobbyManager.h"

LobbyManager::LobbyManager() : m_lock(L"LobbyManager")
{
	m_lobbyCount = 0;
	m_maxLobbyCount = LOBBY_CAPACITY;
}

LobbyManager::~LobbyManager()
{
	auto deleteMap = unordered_map<wstring,Lobby*>(m_lobbyMap);
	for (auto lobby : deleteMap)
	{
		SAFE_DELETE(lobby.second);
	}
	m_lobbyMap.clear();
}

bool LobbyManager::AddLobby(Lobby* lobby)
{
	SAFE_LOCK(m_lock);
	auto findLobby = m_lobbyMap.find(lobby->GetName());
	if (findLobby != m_lobbyMap.end())
	{
		SLOG(L"LobbyManager : the lobby aready exists");
		return false;
	}

	if (m_lobbyCount >= m_maxLobbyCount)
	{
		SLOG(L"LobbyManager : lobby is full. count[%d] ", m_lobbyCount);
		return false;
	}

	m_lobbyMap.insert(make_pair(lobby->GetName(), lobby));
	++m_lobbyCount;
	return true;
}

bool LobbyManager::CloseLobby(Lobby* lobby)
{
	SAFE_LOCK(m_lock);
	if (lobby == nullptr)
	{
		SLOG(L"LobbyManager : lobby trying to close has a null state")
		return false;
	}

	if (m_lobbyMap[lobby->GetName()] != nullptr)
	{
		m_lobbyMap.erase(lobby->GetName());
		SAFE_DELETE(lobby);
		--m_lobbyCount;
		return true;
	}

	SLOG(L"LobbyManager : failed to closed the lobby ");
	return false;
}

Lobby* LobbyManager::CreateLobby(wstring name, unsigned char type)
{
	SLOG(L"LobbyManager::Created lobby !!! name[%s], type[%d]", name.c_str(), type);
	Lobby* lobby = new Lobby(name, type);
	AddLobby(lobby);
	return lobby;
}

unordered_map<wstring, Lobby*>& LobbyManager::GetLobbyMap()
{
	return m_lobbyMap;
}

Lobby* LobbyManager::GetLobby(wstring lobbyName,unsigned char type)
{
	SAFE_LOCK(m_lock);
	Lobby* lobby;
	auto lobbyFind = m_lobbyMap.find(lobbyName);

	if (lobbyFind == m_lobbyMap.end())
	{
		lobby = CreateLobby(lobbyName, type);
	} 
	else
	{
		lobby = lobbyFind->second;
	}
	return lobby;
}

Lock& LobbyManager::GetLock()
{
	return m_lock;
}
