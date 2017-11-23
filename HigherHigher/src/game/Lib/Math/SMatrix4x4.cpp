#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
#include "SMatrix4x4.h"
SMatrix4x4::SMatrix4x4()
{
	Identified();
}

void SMatrix4x4::Init(PineTransformParam param)
{
	Identified();

	_m0 = param.m11;
	_m1 = param.m12;
	_m4 = param.m21;
	_m5 = param.m22;
	_m12 = param.TranslateX;
	_m13 = param.TranslateY;
}

void SMatrix4x4::Identified()
{
	_m0 = 1;
	_m1 = 0;
	_m2 = 0;
	_m3 = 0;

	_m4 = 0;
	_m5 = 1;
	_m6 = 0;
	_m7 = 0;

	_m8 = 0;
	_m9 = 0;
	_m10 = 1;
	_m11 = 0;

	_m12 = 0;
	_m13 = 0;
	_m14 = 0;
	_m15 = 1;
}

void SMatrix4x4::IdentifiedPoint()
{
	_m0 = 0;
	_m1 = 0;
	_m2 = 0;
	_m3 = 1;

	_m4 = 0;
	_m5 = 0;
	_m6 = 0;
	_m7 = 1;

	_m8 = 0;
	_m9 = 0;
	_m10 = 0;
	_m11 = 1;

	_m12 = 0;
	_m13 = 0;
	_m14 = 0;
	_m15 = 1;
}

void SMatrix4x4::IdentifiedFromRect(float x, float y, float w, float h)
{
	_m0 = x;
	_m1 = y;
	_m2 = 0;
	_m3 = 1;

	_m4 = x + w;
	_m5 = y;
	_m6 = 0;
	_m7 = 1;

	_m8 = x + w;
	_m9 = y + h;
	_m10 = 0;
	_m11 = 1;

	_m12 = x;
	_m13 = y + h;
	_m14 = 0;
	_m15 = 1;
}

PineTransformParam SMatrix4x4::GetParam()
{
	PineTransformParam param;

	param.m11 = _m0;
	param.m12 = _m1;
	param.m21 = _m4;
	param.m22 = _m5;
	param.TranslateX = _m12;
	param.TranslateY = _m13;

	return param;
}

PinePoint SMatrix4x4::GetScale()
{
	PinePoint scaleRotate;
	scaleRotate.X = _m1;
	scaleRotate.Y = _m5;
	return scaleRotate;
}

PinePoint SMatrix4x4::GetTranslate()
{
	PinePoint translate;
	translate.X = _m12;
	translate.Y = _m13;
	return translate;
}

void SMatrix4x4::IdentifiedTransplate(float x, float y, float z)
{
	Identified();

	_m12 = x;
	_m13 = y;
	_m14 = z;
}

void SMatrix4x4::IdentifiedScale(float scaleX, float scaleY, float scaleZ)
{
	Identified();

	_m0 = scaleX;
	_m5 = scaleY;
	_m10 = scaleZ;
}

void SMatrix4x4::IdentifiedRotateZ(float angle)
{
	Identified();

	_m0 = CMath::COS(angle);
	_m1 = CMath::SIN(angle);
	_m4 = -CMath::SIN(angle);
	_m5 = CMath::COS(angle);
}

void SMatrix4x4::IdentifiedSkewX(float angle)
{
	Identified();
	_m1 = CMath::TAN(angle);
}

void SMatrix4x4::IdentifiedSkewY(float angle)
{
	Identified();
	_m4 = -CMath::TAN(angle);
}

SMatrix4x4 SMatrix4x4::RotateZ(float angle, float cx, float cy)
{
	SMatrix4x4 trans, reTrans, rotate, result;

	result.Identified();
	trans.IdentifiedTransplate(-cx, -cy, 0);
	reTrans.IdentifiedTransplate(cx, cy, 0);
	rotate.IdentifiedRotateZ(angle);

	result = Mul(result, trans);
	result = Mul(result, rotate);
	result = Mul(result, reTrans);

	return result;
}

SMatrix4x4 SMatrix4x4::Scale(float scaleX, float scaleY, float cx, float cy)
{
	SMatrix4x4 trans, reTrans, scale, result;

	result.Identified();
	trans.IdentifiedTransplate(-cx, -cy, 0);
	reTrans.IdentifiedTransplate(cx, cy, 0);
	scale.IdentifiedScale(scaleX, scaleY, 1.0f);

	result = Mul(result, trans);
	result = Mul(result, scale);
	result = Mul(result, reTrans);

	return result;
}

SMatrix4x4 SMatrix4x4::Tranplate(float x, float y)
{
	SMatrix4x4 trans;
	trans.IdentifiedTransplate(x, y, 0);
	return trans;
}

SMatrix4x4 SMatrix4x4::SkewX(float angle, float cx, float cy)
{
	SMatrix4x4 trans, reTrans, skew, result;

	result.Identified();
	trans.IdentifiedTransplate(-cx, -cy, 0);
	reTrans.IdentifiedTransplate(cx, cy, 0);
	skew.IdentifiedSkewX(angle);

	result = Mul(result, trans);
	result = Mul(result, skew);
	result = Mul(result, reTrans);

	return result;
}

SMatrix4x4 SMatrix4x4::SkewY(float angle, float cx, float cy)
{
	SMatrix4x4 trans, reTrans, skew, result;

	result.Identified();
	trans.IdentifiedTransplate(-cx, -cy, 0);
	reTrans.IdentifiedTransplate(cx, cy, 0);
	skew.IdentifiedSkewY(angle);

	result = Mul(result, trans);
	result = Mul(result, skew);
	result = Mul(result, reTrans);

	return result;
}

void SMatrix4x4::Mul(SMatrix4x4 &result, SMatrix4x4 left, SMatrix4x4 right)
{
	result._m0 = left._m0*right._m0 + left._m1*right._m4 + left._m2*right._m8 + left._m3*right._m12;
	result._m1 = left._m0*right._m1 + left._m1*right._m5 + left._m2*right._m9 + left._m3*right._m13;
	result._m2 = left._m0*right._m2 + left._m1*right._m6 + left._m2*right._m10 + left._m3*right._m14;
	result._m3 = left._m0*right._m3 + left._m1*right._m7 + left._m2*right._m11 + left._m3*right._m15;

	result._m4 = left._m4*right._m0 + left._m5*right._m4 + left._m6*right._m8 + left._m7*right._m12;
	result._m5 = left._m4*right._m1 + left._m5*right._m5 + left._m6*right._m9 + left._m7*right._m13;
	result._m6 = left._m4*right._m2 + left._m5*right._m6 + left._m6*right._m10 + left._m7*right._m14;
	result._m7 = left._m4*right._m3 + left._m5*right._m7 + left._m6*right._m11 + left._m7*right._m15;

	result._m8 = left._m8*right._m0 + left._m9*right._m4 + left._m10*right._m8 + left._m11*right._m12;
	result._m9 = left._m8*right._m1 + left._m9*right._m5 + left._m10*right._m9 + left._m11*right._m13;
	result._m10 = left._m8*right._m2 + left._m9*right._m6 + left._m10*right._m10 + left._m11*right._m14;
	result._m11 = left._m8*right._m3 + left._m9*right._m7 + left._m10*right._m11 + left._m11*right._m15;

	result._m12 = left._m12*right._m0 + left._m13*right._m4 + left._m14*right._m8 + left._m15*right._m12;
	result._m13 = left._m12*right._m1 + left._m13*right._m5 + left._m14*right._m9 + left._m15*right._m13;
	result._m14 = left._m12*right._m2 + left._m13*right._m6 + left._m14*right._m10 + left._m15*right._m14;
	result._m15 = left._m12*right._m3 + left._m13*right._m7 + left._m14*right._m11 + left._m15*right._m15;
}

SMatrix4x4 SMatrix4x4::Mul(SMatrix4x4 left, SMatrix4x4 right)
{
	SMatrix4x4 result;

	result._m0 = left._m0*right._m0 + left._m1*right._m4 + left._m2*right._m8 + left._m3*right._m12;
	result._m1 = left._m0*right._m1 + left._m1*right._m5 + left._m2*right._m9 + left._m3*right._m13;
	result._m2 = left._m0*right._m2 + left._m1*right._m6 + left._m2*right._m10 + left._m3*right._m14;
	result._m3 = left._m0*right._m3 + left._m1*right._m7 + left._m2*right._m11 + left._m3*right._m15;

	result._m4 = left._m4*right._m0 + left._m5*right._m4 + left._m6*right._m8 + left._m7*right._m12;
	result._m5 = left._m4*right._m1 + left._m5*right._m5 + left._m6*right._m9 + left._m7*right._m13;
	result._m6 = left._m4*right._m2 + left._m5*right._m6 + left._m6*right._m10 + left._m7*right._m14;
	result._m7 = left._m4*right._m3 + left._m5*right._m7 + left._m6*right._m11 + left._m7*right._m15;

	result._m8 = left._m8*right._m0 + left._m9*right._m4 + left._m10*right._m8 + left._m11*right._m12;
	result._m9 = left._m8*right._m1 + left._m9*right._m5 + left._m10*right._m9 + left._m11*right._m13;
	result._m10 = left._m8*right._m2 + left._m9*right._m6 + left._m10*right._m10 + left._m11*right._m14;
	result._m11 = left._m8*right._m3 + left._m9*right._m7 + left._m10*right._m11 + left._m11*right._m15;

	result._m12 = left._m12*right._m0 + left._m13*right._m4 + left._m14*right._m8 + left._m15*right._m12;
	result._m13 = left._m12*right._m1 + left._m13*right._m5 + left._m14*right._m9 + left._m15*right._m13;
	result._m14 = left._m12*right._m2 + left._m13*right._m6 + left._m14*right._m10 + left._m15*right._m14;
	result._m15 = left._m12*right._m3 + left._m13*right._m7 + left._m14*right._m11 + left._m15*right._m15;

	return result;
}