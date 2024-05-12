#pragma once
#include "Object.h"


template<typename T>
class vector_ : public Object
{
private:
	vector<T>* data;

public:
	vector_()
	{
		this->typeInfo = &typeid(vector_<T>);
		this->isArray = true;

		data = new vector<T>();

		if (is_pointer<T>::value || typeid(T) == typeid(wstring_))
		{
			this->isPrimitive = false;
		}

		this->dataPointer = data;
	}

	vector_(int size)
	{

		this->typeInfo = &typeid(vector_<T>);
		this->isArray = true;

		data = new vector<T>(size);

		if(is_pointer<T>::value || typeid(T) == typeid(wstring_))
		{ 
			this->isPrimitive = false;
		}

		this->dataPointer = data;
	}

	vector_(vector<T> value)
	{
		this->typeInfo = &typeid(vector_<T>);
		this->isArray = true;

		data = new vector<T>(value);

		if (is_pointer<T>::value || typeid(T) == typeid(wstring_))
		{
			this->isPrimitive = false;
		}


		this->dataPointer = data;
	}

	~vector_()
	{
		if constexpr (is_pointer<T>::value)
		{
			for (auto it = data->begin(); it != data->end(); it++)
			{
				delete (Object*)(*it);
			}
		}

		delete data;
	}

	 const int GetSize() const
	{
		 return data->size();
	}

	 T operator[](int i)
	 {
		 return (*data)[i];
	 }

	size_t Hash() const override
	{
		throw runtime_error("Arrays cannot be key");
		return 0;
	}
		
	Object* Copy() override
	{
		return new vector_<T>(*data);
	}

	vector<T>& operator*()
	{
		return *data;
	}

};