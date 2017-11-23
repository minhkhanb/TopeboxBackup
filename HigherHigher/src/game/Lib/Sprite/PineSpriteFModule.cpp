#include "pch.h"
#include "PineSpriteFModule.h"


PinePoint PineSpriteFModule::GetRealPos() 
{
	PineTransformParam param;
	param.TranslateX = _tx;
	param.TranslateY = _ty;
	param.m11 = _m11;
	param.m12 = _m12;
	param.m21 = _m21;
	param.m22 = _m22;

	return param.Transform(0, 0);
}