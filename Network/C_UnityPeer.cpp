#include "C_UnityPeer.h"

C_UnityPeer::C_UnityPeer(Session* session) : Peer(session)
{
	SLOG(L"C_UnityPeer : The new peer has been initialized. !!!")
}

C_UnityPeer::~C_UnityPeer()
{
}

void C_UnityPeer::InitPeer()
{
}

void C_UnityPeer::InitCallback()
{
}

void C_UnityPeer::OnOperationResponse(OperationResponse* operationResponse)
{
}

void C_UnityPeer::OnStatusChanged(PeerStatusCode code)
{
}
