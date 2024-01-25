#pragma once
#include <string>
#include <iostream>
#include <type_traits>

using namespace std;

class Object
{
protected:
	const type_info* typeInfo = NULL;
	const Object* element = NULL;
	void* dataPointer;
	unsigned int size = 0;
	bool isArray = false;
	bool isPrimitive = false;

public:
    Object() 
	{
		dataPointer = nullptr;
	}
	virtual ~Object() {}

	virtual size_t Hash() const { wcout << L"ºÎ¸ð hash" << endl; return 0; }

	bool operator == (const Object& other) const
	{
		if (this->typeInfo != other.typeInfo)
			return false;

		if (this->size != other.size)
			return false;

		return memcmp(this->dataPointer, other.dataPointer, this->size ) == 0 ? true : false;
	}

	const type_info* GetType() const
	{
		return typeInfo;
	}

	const void* GetData() const
	{
		return dataPointer;
	}

	virtual const int GetSize() const
	{
		return size;
	}

	const bool IsArray() const
	{
		return isArray;
	}

	const bool IsPrimitive() const
	{
		return isPrimitive;
	}

	const Object* GetElement() const
	{
		return element;
	}
	
};

struct ObjectHash
{
	size_t operator()(const Object* object) const
	{
		return object->Hash();
	}
};

struct ObjectEqual
{
	bool operator()(const Object* a,const Object* b) const
	{
		return *a == *b;
	}
};
