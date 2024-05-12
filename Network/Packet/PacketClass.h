#pragma once
#include "../../stdafx.h"
class Packet;
class Object;



class Packet
{
public:
	virtual PacketType Type() = 0;
};


class PK_InitRequest : public Packet
{
private:

public:
	PK_InitRequest() {}
	~PK_InitRequest() {}
	PacketType Type() { return E_InitRequest; }

	int size = 0;
	byte* data = nullptr;

};

class PK_InitResponse : public Packet
{
public:
	PacketType Type() { return E_InitResponse; }

};


class PK_OperationRequest : public  Packet
{
public:
	byte m_operationCode;
	unordered_map<byte, Object*>* m_parameters;
	PK_OperationRequest() 
	{
		//typeInfo = &typeid(OperationRequest);
		//this->dataPointer = (void*)this;
		//size = sizeof(*this);
		m_operationCode = (byte)0;
		m_parameters = new unordered_map<byte, Object*>;
	}

	~PK_OperationRequest() 
	{
		for (auto it = m_parameters->begin(); it != m_parameters->end(); it++)
		{
			delete it->second;
		}
		delete m_parameters;
	}

	Object* operator[](ParameterCode paramCode)
	{
		return (*m_parameters)[(byte)paramCode];
	}

	PacketType Type() override
	{
		return E_OperationRequest;
	}
};

class PK_OperationResponse : public Packet
{
public:
	byte m_operationCode;
	short m_returnCode;
	wstring m_debugMessage;
	
	unordered_map<byte, Object*>* m_parameters;

	PK_OperationResponse()
	{
		m_operationCode = 0;
		m_returnCode = 0;
		m_debugMessage = L"";
		m_parameters = new unordered_map<byte, Object*>;
	}

	~PK_OperationResponse()
	{
		for (auto it = m_parameters->begin(); it != m_parameters->end(); it++)
		{
			delete it->second;
		}
		delete m_parameters;
	}

	Object* operator[](ParameterCode paramCode)
	{
		return (*m_parameters)[(byte)paramCode];
	}

	PacketType Type() override
	{
		return E_OperationResponse;
	}
};

class PK_EventData : public Packet
{
private:
	const byte SENDER_KEY = (byte)254;
	const byte CUSTOM_DATA_KEY = (byte)245;

	int m_sender = -1;
public:
	byte m_eventCode;
	unordered_map<byte,Object*>* m_parameters;

	PK_EventData()
	{
		m_eventCode = 0;
		m_parameters = new unordered_map<byte, Object*>();
	}

	~PK_EventData()
	{
		for (auto p : *m_parameters)
		{
			delete p.second;
		}
	}

	int GetSender()
	{
		if (m_sender == -1)
		{
			Object* obj = (*this)[SENDER_KEY];
			m_sender = obj != nullptr ? **(generic_<int>*)obj : -1;
		}
		return m_sender;
	}

	void SetSender(int sender)
	{
		m_sender = sender;
		if (m_parameters->find(SENDER_KEY) != m_parameters->end())
		{
			Object* value = (*m_parameters)[SENDER_KEY];
			if (value != nullptr)
				delete value;

			m_parameters->insert(make_pair(SENDER_KEY, new generic_<int>(sender)));
		}
		else
		{
			m_parameters->insert(make_pair(SENDER_KEY, new generic_<int>(sender)));
		}
	}

	void SetCustomData(Object* data)
	{
		if (m_parameters->find(CUSTOM_DATA_KEY) != m_parameters->end())
		{
			delete (*m_parameters)[CUSTOM_DATA_KEY];
		}
		m_parameters->insert(make_pair(CUSTOM_DATA_KEY, data));
	}

	Object* operator[](byte key)
	{
		return (*m_parameters)[key];
	}

	void Reset()
	{
		this->m_eventCode = (byte)0;
		this->m_parameters->clear();
		this->m_sender = -1;
	}

	PacketType Type() override
	{
		return E_EventData;
	}

};