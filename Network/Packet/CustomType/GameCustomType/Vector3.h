#pragma once
#include "../Object.h"


class Vector3 : public Object
{
private:
	

public:
	float x;
	float y;
	float z;

	Vector3(float x_, float y_, float z_)
	{
		this->typeInfo = &typeid(Vector3);
		this->dataPointer = &x;
		size = 12;

		x = x_;
		y = y_;
		z = z_;
	}

	~Vector3() {};

	size_t Hash() const override
	{
		runtime_error("Vector3 cannot be key");
		return 0;
	}

	Object* Copy() override
	{
		return new Vector3(x, y, z);
	}


};