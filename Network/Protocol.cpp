#include "../stdafx.h"
#include "Protocol.h"

void Protocol::Serialize(int value, unsigned char* target, int& offset)
{
	target[offset++] = (unsigned char)(value >> 24);
	target[offset++] = (unsigned char)(value >> 16);
	target[offset++] = (unsigned char)(value >> 8);
	target[offset++] = (unsigned char)(value);

}

void Protocol::Deserialize(int& value, unsigned char* source, int& offset)
{
}

void Protocol::SerializePacket(StreamBuffer& stream, Packet* packet)
{
	switch (packet->Type())
	{
	case E_InitResponse:
		stream.GetBuffer()[8] = 1;
		break;
	case E_C_OperationRequest:
		SerializeOperationRequest(stream, *(OperationRequest*)packet, true);
		break;

	case E_S_OperationResponse:
		SerializeOperationResponse(stream, *(OperationResponse*)packet, true);
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
