#pragma once
#include "Object.h"
#include "wstring_.h"
#include "generic_.h"
#include <unordered_map>

class Dictionary : public Object
{
private:
	unordered_map<Object*, Object*, ObjectHash, ObjectEqual> table;

public:
	Dictionary()
	{
		this->typeInfo = &typeid(Dictionary);
		this->dataPointer = (void*)this;
	}

	~Dictionary()
	{
		for (auto it = table.begin(); it != table.end(); it++)
		{
			delete it->first;
			delete it->second;
		}
	}

	auto begin() const
	{
		return table.begin();
	}

	auto end() const
	{
		return table.end();
	}

	void Insert(pair<Object*, Object*> data)
	{
		table.insert(data);
	}

	int Count() const
	{
		return table.size();
	}

	Object* operator[](Object* key)
	{
		return table[key];
	}


	template <typename T>
	Object* operator[](T&& key)
	{
		Object* temp = new generic_<T>(key);
		Object* value = table[temp];
		delete temp;
		return value;
	}

	Object* operator[](wstring&& key)
	{
		Object* temp = new wstring_(key);
		Object* value = table[temp];
		delete temp;
		return value;
	}

	Object* operator[](wstring key)
	{
		Object* temp = new wstring_(key);
		Object* value = table[temp];
		delete temp;
		return value;
	}
};
