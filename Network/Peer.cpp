#include "Peer.h"

Peer::Peer(Session* session) : m_session(session)
{
//	m_session = session;
}

Peer::~Peer()
{
}

void Peer::InitPeer()
{
}

void Peer::InitCallback()
{
}

void Peer::OnOperationResponse(OperationResponse* operationResponse)
{
}

void Peer::OnStatusChanged(PeerStatusCode code)
{
}

Session* Peer::GetSession()
{
	return m_session;
}
