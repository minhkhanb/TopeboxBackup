// ** nhut.hoang **
#if !defined(MATRIX4X4_H)
#define MATRIX4X4_H
class SMatrix4x4
{
public:
	float _m0;
	float _m1;
	float _m2;
	float _m3;
	float _m4;
	float _m5;
	float _m6;
	float _m7;
	float _m8;
	float _m9;
	float _m10;
	float _m11;
	float _m12;
	float _m13;
	float _m14;
	float _m15;

	void Init(PineTransformParam param);
	SMatrix4x4();

	void Identified();
	void IdentifiedPoint();
	void IdentifiedFromRect(float x, float y, float w, float h);
	void IdentifiedTransplate(float x, float y, float z);
	void IdentifiedScale(float scaleX, float scaleY, float scaleZ);
	void IdentifiedRotateZ(float angle);
	void IdentifiedSkewX(float angle);
	void IdentifiedSkewY(float angle);

	PineTransformParam GetParam();
	PinePoint GetScale();
	PinePoint GetTranslate();

	static SMatrix4x4 RotateZ(float angle, float cx, float cy);
	static SMatrix4x4 Scale(float scaleX, float scaleY, float cx, float cy);
	static SMatrix4x4 Tranplate(float x, float y);
	static SMatrix4x4 SkewX(float angle, float cx, float cy);
	static SMatrix4x4 SkewY(float angle, float cx, float cy);

	static void Mul(SMatrix4x4 &result, SMatrix4x4 left, SMatrix4x4 right);
	static SMatrix4x4 Mul(SMatrix4x4 left, SMatrix4x4 right);

};

#endif