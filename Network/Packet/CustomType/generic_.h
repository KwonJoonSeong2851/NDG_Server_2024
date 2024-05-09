#pragma once
#include "Object.h"
template<typename T>
class generic_ : public Object
{
private:
	T data;

public:
	generic_(T value)
	{
		data = value;
		this->typeInfo = &typeid(T);
		this->dataPointer = (void*)&data;
		size = sizeof(data);
		isPrimitive = true;
	}

	~generic_() 
	{
		size = 0;
		this->typeInfo = NULL;
		this->dataPointer = NULL;
	}



	size_t Hash() const override
	{
		return std::hash<T>()(data);
	}

	Object* Copy() override
	{
		return new generic_<T>(data);
	}

	T& operator*()
	{
		return data;
	}


};