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
		this->typeInfo = &typeid(T);
		this->isArray = true;

		data = new vector<T>(1);
		if (is_pointer<T>::value)
		{
			this->element = (const Object*)new T();
		} 
		else if (typeid(T) == typeid(wstring_))
		{
			this->element = (const Object*)new wstring_();
			this->isPrimitive = false;
		}
		else
		{
			this->isPrimitive = true;
		}

		this->dataPointer = data;
	}

	vector_(int size)
	{
		//if (size <= 0)
		//{
		//	size = 1;
		//}

		this->typeInfo = &typeid(T);
		this->isArray = true;

		data = new vector<T>(size);

		if(is_pointer<T>::value)
		{ 
			this->element = (const Object*)new T();
		}
		else if (typeid(T) == typeid(wstring_))
		{
			this->element = (const Object*)new wstring_();
			this->isPrimitive = false;
		}
		else
		{
			this->isPrimitive = true;
		}

		this->dataPointer = data;
	}

	vector_(vector<T> value)
	{
		//data = value;
		this->typeInfo = &typeid(T);
		this->isArray = true;

		data = new vector<T>(value);

		if (is_pointer<T>::value)
		{
			this->element = (const Object*)new T();
		}
		else if (typeid(T) == typeid(wstring_))
		{
			this->element = (const Object*)new wstring_();
			this->isPrimitive = false;
		}
		else
		{
			this->isPrimitive = true;
		}


		this->dataPointer = data;
	}

	~vector_()
	{
		delete data;
		if (this->element != nullptr)
			delete this->element;
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