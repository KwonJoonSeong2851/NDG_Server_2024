#pragma once

#include "../stdafx.h"
class Session;
class PK_OperationResponse;
class Room;
class Lobby;



class Peer
{
protected:
	wstring m_nickName;
	int m_actorNumber = -1;

	ServerConnection m_serverConnection;
	PeerStatusCode m_peerStatus;

	Hashtable* m_properties;
	Session* m_session;
	Lobby* m_currentLobby;
	Room* m_currentRoom;

public:
	Peer(Session* session);
	virtual ~Peer();

	void SendPacket(Packet* packet);

	virtual void InitPeer();

	virtual void InitCallback();

	virtual void OnStatusChanged(PeerStatusCode code);

	// Matchmaking
	virtual void OnJoinedLobby(Lobby* lobby);

	virtual void OnLeftLobby();

	virtual void OnCreateRoom(Room* room);

	virtual void OnJoinedRoom(Room* room);

	virtual void OnLeftRoom();

	virtual void OnGetRoomList();

	virtual void OnOtherPlayerEnteredRoom(Peer* other);

	//In Room
	virtual void OnPropertiesChanged(Hashtable* props);


	void CacheProperties(Hashtable* hash);

	Hashtable* GetProperties();

	Session* GetSession();

	int GetActorNumber();
	void SetActorNumber(int num);

	Room* GetCurrentRoom();

	ServerConnection GetServerConnection();
    void SetServerConnection(ServerConnection server);



	//virtual void OnEvent(EventData eventData);

	//Peer() = default;
};

