#pragma once
#include "../../stdafx.h"
class Packet;
class Object;

enum class OperationCode : unsigned char
{
	Join = 255,
	JoinLobby = 229,
	LeaveLobby = 228,
	CreateGame = 227,
	JoinGame = 226,
	JoinRandomGame = 225,
	Leave = 254,
	RaiseEvent = 253,
	SetProperties = 252,
	GetProperties = 251,
	ChangeGroups = 248,
	GetLobbyStats = 221,
	GetRegions = 220,
	ServerSettings = 218,
	GetGameList = 217,
};

class OperationRequest : public  Packet
{
public:
	byte OperationCode;
	unordered_map<byte, Object*>* Parameters;
	OperationRequest() 
	{
		//typeInfo = &typeid(OperationRequest);
		//this->dataPointer = (void*)this;
		//size = sizeof(*this);
		OperationCode = (byte)0;
		Parameters = new unordered_map<byte, Object*>;
	}

	~OperationRequest() 
	{
		for (auto it = Parameters->begin(); it != Parameters->end(); it++)
		{
			delete it->second;
		}
		delete Parameters;
	}

	Object* operator[](byte paramCode)
	{
		return (*Parameters)[paramCode];
	}

	PacketType Type() override
	{
		return E_C_OperationRequest;
	}
};

class OperationResponse : public Packet
{
public:
	byte OperatoinCode;
	short ReturnCode;
	wstring DebugMessage;
	
	unordered_map<byte, Object*>* Parameters;

	Object* operator[](byte paramCode)
	{
		return (*Parameters)[paramCode];
	}

	PacketType Type() override
	{
		return E_S_OperationResponse;
	}
};

