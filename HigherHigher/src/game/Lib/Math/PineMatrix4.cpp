#include "pch.h"
#include "PineMatrix4.h"

PineMatrix4::PineMatrix4() 
{
	m0 = 1, m1 = 0, m2 = 0, m3 = 0;
	m4 = 0, m5 = 1, m6 = 0, m7 = 0;
	m8 = 0, m9 = 0, m10 = 1, m11 = 0;
	m12 = 0, m13 = 0, m14 = 0, m15 = 1;
};