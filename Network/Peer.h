#pragma once
#include "Enum.h"

class Session;
class OperationResponse;
class Peer
{
protected:
	Session* m_session;
public:
	Peer(Session* session);
	virtual ~Peer();

	virtual void InitPeer();

	virtual void InitCallback();
	//virtual void DebugReturn(DeubugLevel level, string message);
	virtual void OnOperationResponse(OperationResponse* operationResponse);

	virtual void OnStatusChanged(PeerStatusCode code);

	Session* GetSession();



	//virtual void OnEvent(EventData eventData);
};