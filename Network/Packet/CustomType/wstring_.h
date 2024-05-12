#pragma once
#include "Object.h"



class wstring_ : public Object
{
private:
	wstring data;

public:

	wstring_()
	{
		typeInfo = &typeid(wstring_);
		this->dataPointer = (void*)data.c_str();
		size = 0;
		isPrimitive = false;
	}

	wstring_(wstring ws)
	{
		data = ws;
		typeInfo = &typeid(wstring_);
		this->dataPointer = (void*)data.c_str();
		size = data.length() * sizeof(wchar_t);
		isPrimitive = false;

	}

	~wstring_()
	{
		size = 0;
		this->typeInfo = NULL;
		this->dataPointer = NULL;
	}

	size_t Hash() const override
	{
		return std::hash<wstring>()(data);
	}

	Object* Copy() override
	{
		return new wstring_(data);
	}

	void operator=(const wstring& other)
	{
		typeInfo = &typeid(wstring_);
		data = other;
		this->dataPointer = (void*)data.c_str();
		size = other.size() * sizeof(wchar_t);
	}

	operator wstring() const
	{
		return data;
	}

	wstring& operator*()
	{
		return data;
	};
};