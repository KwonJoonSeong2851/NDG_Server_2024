#pragma once


#include "../Object.h"


class Quaternion : public Object
{

public:
	float x,y,z,w;

	Quaternion(float x_, float y_, float z_, float w_)
	{
		this->typeInfo = &typeid(Quaternion);
		this->dataPointer = &x;
		size = 16;

		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	~Quaternion() {};

	size_t Hash() const override
	{
		throw runtime_error("Quaternion cannot be key");
		return 0;
	}

	Object* Copy() override
	{
		return new Quaternion(x, y, z, w);
	}


};