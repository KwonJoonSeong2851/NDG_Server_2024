#pragma once
#include "Peer.h"
#include "../stdafx.h"

class Session;

class C_UnityPeer : public Peer
{
private:


public:
	C_UnityPeer(Session* session);
	virtual ~C_UnityPeer();

	virtual void InitPeer();

	virtual void InitCallback();

	virtual void OnOperationResponse(OperationResponse* operationResponse);

	virtual void OnStatusChanged(PeerStatusCode code);
};