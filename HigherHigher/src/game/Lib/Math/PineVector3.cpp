#include "pch.h"
#include "PineVector3.h"

PineVector3::PineVector3()
{
	x = 0;
	y = 0;
	z = 0;
}

void PineVector3::SetValue(float vx, float vy, float vz)
{
	x = vx;
	y = vy;
	z = vz;
}