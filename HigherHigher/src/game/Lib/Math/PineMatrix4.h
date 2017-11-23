#pragma once
#include "../DataType/CType.h"
#if !defined(_H_PINE_MATRIX4)

#define _H_PINE_MATRIX4
class PineMatrix4
{
public:
	float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
	PineMatrix4();
	friend class CMath;
};

#endif//_H_PINE_MATRIX4