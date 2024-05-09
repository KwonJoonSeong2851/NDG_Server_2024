#pragma once
//#include "../../stdafx.h"
#include "Object.h"
//#include "../../../Util/Logger.h"
//#include "wstring_.h"
//#include "generic_.h"
//#include <unordered_map>

class Hashtable : public Object
{
private:
	unordered_map<Object*, Object*,ObjectHash,ObjectEqual> table;

public:
	Hashtable()
	{
		this->typeInfo = &typeid(Hashtable);
		this->dataPointer = (void*)this;
	}

	Hashtable(Hashtable& other)
	{
		this->typeInfo = &typeid(Hashtable);
		this->dataPointer = (void*)this;
		for (auto p : other.GetTable())
		{
			table.insert(p);
		}
	}

	~Hashtable()
	{
		for (auto it = table.begin(); it != table.end(); it++)
		{
			delete it->first;
			delete it->second;
		}
	}


	unordered_map<Object*, Object*, ObjectHash, ObjectEqual>& GetTable()
	{
		return table;
	}

	int Count() const
	{
		return table.size();
	}

	auto Begin() const
	{
		return table.begin();
	}

	auto End() const
	{
		return table.end();
	}

	template<typename T>
	unordered_map<Object*, Object*, ObjectHash, ObjectEqual>::iterator Find(T key)
	{
		Object* obj = new generic_<T>(key);
		auto iter = table.find(obj);
		delete obj;
		return iter;
	}

	unordered_map<Object*, Object*, ObjectHash, ObjectEqual>::iterator Find(Object* key)
	{
		auto iter = table.find(key);
		return iter;
	}

	void Insert(pair<Object*, Object*> data)
	{
		//기존에 값이있다면 해당 컨테이너를 삭제하고 값을 삽입합니다.
		auto iter = this->Find(data.first);
		if (iter != table.end())
		{
			Object* temp1 = iter->first;
			Object* temp2 = iter->second;
			table.erase(iter);
			delete temp1;
			delete temp2;
		}
		
		table.insert(data);
	}


	/// <summary>
	/// key 값이 wstring_인 값들을 해당 hash와 합칩니다.
	/// </summary>
	/// <param name="hash"></param>
	void MergeStringKey(const Hashtable* hash)
	{
		for (auto p : hash->table)
		{
			if (*(p.first->GetType()) == typeid(wstring_))
			{
				this->Insert(make_pair(p.first->Copy(), p.second->Copy()));
			}
		}
	}



	Object* operator[](Object* key)
	{
		return table[key];
	}
	
	Object* operator[](wstring&& key)
	{
		Object* temp = new wstring_(key);
		Object* value = table[temp];
		delete temp;
		return value;
	}

	template <typename T>
	Object* operator[](T&& key)
	{
		Object* temp = new generic_<T>(key);
		Object* value = table[temp];
		delete temp;
		return value;
	}

	Hashtable& operator=(Hashtable& other)
	{
		if (this != &other)
		{
			table.clear();
			for (auto p : other.GetTable())
			{
				table.insert(p);
			}
		}

		return *this;
	}

	size_t Hash() const override
	{
		throw runtime_error("Hashtable cannot be key");
		return 0;
	}

	Object* Copy() override
	{
		Hashtable* hash = new Hashtable();
		for (auto p : table)
		{
			Object* key = p.first->Copy();
			Object* value = p.second->Copy();
			hash->Insert(make_pair(key, value));
		}

		return hash;
	}
};
