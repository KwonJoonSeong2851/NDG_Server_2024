#pragma once
#include "../stdafx.h"



class StreamBuffer;
/// <summary>
/// Protocol Interface class
/// </summary>
class Protocol
{
	

public:
	Protocol() {};
	virtual ~Protocol() {};
	virtual void SerializeOperationRequest(StreamBuffer& stream, const PK_OperationRequest& serObject, bool setType) = 0;
	virtual void SerializeOperationResponse(StreamBuffer& stream, const PK_OperationResponse& serObject, bool setType) = 0;

	virtual void SerializeEventData(StreamBuffer& stream, const PK_EventData& serObject, bool setType) = 0;

	virtual PK_OperationRequest* DeserializeOperationRequest(StreamBuffer& stream) = 0;
	virtual PK_OperationResponse* DeserializeOperationResponse(StreamBuffer& stream) = 0;

	virtual unordered_map<byte, Object*>* ReadParameterTable(StreamBuffer& stream, unordered_map<byte, Object*>* target = NULL) = 0;


	void Serialize(int value, char* target, int& offset);
	void Deserialize(int& value,unsigned char* source, int& offset);
    void SerializePacket(StreamBuffer& stream, Packet* packet);
	Packet* DeserializePacket(StreamBuffer& stream);

};