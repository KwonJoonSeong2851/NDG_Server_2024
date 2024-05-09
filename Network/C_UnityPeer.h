#pragma once

#include "Peer.h"


class Session;
class Lobby;

class C_UnityPeer : public Peer
{
private:
	int aaa = 0;

public:
	C_UnityPeer(Session* session);
	virtual ~C_UnityPeer();

	virtual void InitPeer();

	virtual void InitCallback();

	virtual void OnStatusChanged(PeerStatusCode code);

	//Matchmaking
	virtual void OnJoinedLobby(Lobby* lobby);

	virtual void OnLeftLobby();

	virtual void OnCreateRoom(Room* room);

	virtual void OnJoinedRoom(Room* room);

	virtual void OnLeftRoom();

	virtual void OnGetRoomList();

	virtual void OnOtherPlayerEnteredRoom(Peer* other);

	//InRoom
	virtual void OnPropertiesChanged(Hashtable* props);

	
};
