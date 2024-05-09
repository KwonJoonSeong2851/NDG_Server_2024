#include "../stdafx.h"
#include "Protocol.h"

void Protocol::Serialize(int value,  char* target, int& offset)
{
	target[offset++] = (char)(value >> 24);
	target[offset++] = (char)(value >> 16);
	target[offset++] = (char)(value >> 8);
	target[offset++] = (char)(value);

}

void Protocol::Deserialize(int& value, unsigned char* source, int& offset)
{
	value = (int)source[offset++] << 24 | (int)source[offset++] << 16 | (int)source[offset++] << 8 | (int)source[offset++];
}

void Protocol::SerializePacket(StreamBuffer& stream, Packet* packet)
{
	switch (packet->Type())
	{
	case E_InitResponse:
		stream.GetBuffer()[8] = 1;
		break;
	case E_OperationRequest:
		stream.GetBuffer()[8] = 2;
		SerializeOperationRequest(stream, *(PK_OperationRequest*)packet, false);
		break;

	case E_OperationResponse:
		stream.GetBuffer()[8] = 3;
		SerializeOperationResponse(stream, *(PK_OperationResponse*)packet, false);
		break;

	case E_EventData:
		stream.GetBuffer()[8] = 4;
		SerializeEventData(stream, *(PK_EventData*)packet, false);
		break;

	}
}

Packet* Protocol::DeserializePacket(StreamBuffer& stream)
{
	switch (stream.ReadByte())
	{
	case (byte)GpType::OperationRequest:
		return this->DeserializeOperationRequest(stream);

	case (byte)GpType::OperationResponse:
		return this->DeserializeOperationResponse(stream);
	}
	return nullptr;
}
