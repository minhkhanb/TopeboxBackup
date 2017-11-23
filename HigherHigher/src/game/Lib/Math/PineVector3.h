#pragma once
#include "../DataType/CType.h"
#if !defined(_H_PINE_VECTOR3)

#define _H_PINE_VECTOR3
class PineVector3
{
public:
	float x, y, z;
	PineVector3();
	void SetValue(float x, float y, float z);
	friend class CMath;
};

#endif//_H_PINE_VECTOR3