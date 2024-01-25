#include "Protocol.h"

void Protocol::SerializePacket(StreamBuffer& stream, Packet* packet)
{
	switch (packet->Type())
	{
	case E_OperationRequest:
		SerializeOperationRequest(stream, *(OperationRequest*)packet, true);
			break;

	case E_OperationResponse:
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
