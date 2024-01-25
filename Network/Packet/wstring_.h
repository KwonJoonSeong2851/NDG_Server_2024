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
	}

	wstring_(wstring ws)
	{
		data = ws;
		typeInfo = &typeid(wstring_);
		this->dataPointer = (void*)data.c_str();
		size = data.length() * sizeof(wchar_t);
	}

	//wstring_(const wchar_t* ws)
	//{
	//	data = *ws;
	//	typeInfo = &typeid(wstring_);
	//	this->dataPointer = (void*)ws;
	//	size = wcslen(ws);
	//}


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