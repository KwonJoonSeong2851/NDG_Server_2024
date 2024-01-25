#pragma once
#include "../stdafx.h"
#include "Packet/PacketClass.h"


/// <summary>
/// Protocol Interface class
/// </summary>
class Protocol
{
	

public:
	virtual void SerializeOperationRequest(StreamBuffer& stream, const OperationRequest& serObject, bool setType) = 0;
	virtual void SerializeOperationResponse(StreamBuffer& stream, const OperationResponse& serObject, bool setType) = 0;

	virtual OperationRequest* DeserializeOperationRequest(StreamBuffer& stream) = 0;
	virtual OperationResponse* DeserializeOperationResponse(StreamBuffer& stream) = 0;

    void SerializePacket(StreamBuffer& stream, Packet* packet);
	Packet* DeserializePacket(StreamBuffer& stream);

};