#ifndef _CMATH_H_
#define _CMATH_H_

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "PineMatrix4.h"
#include "PineVector3.h"

#define PI 3.141592653589793

class CMath
{
public:
    CMath(){};
    ~CMath(){};
	
	static unsigned int  srand_number;

	static int RANDOM(int min, int max);
//hao.ly add
	static float RANDOM_PERCENT(int _countDecimal);
//#ifndef MAC_OS
	static int MAXIMUM(int a, int b);
	static int MINIMUM(int a, int b);

	static float MinFloat(float* arr, int len);
	static int MinInt(int * arr, int len);

	static float MaxFloat(float* arr, int len);
	static int MaxInt(int* arr, int len);


//#endif
	static void PRE_RANDOM();
	static int ABSOLUTE_VALUE(int x);
	static float ABSOLUTE_VALUE(float x);
	static float SQRT(float x);
	static float POW(float x, int y);
	static float SIN(float angle);
	static float COS(float angle);
	static float TAN(float angle);
	
	static float ARCSIN(float sin);
	static float ARCCOS(float cos);
	static float ARCTAN(float tan);
	
	static float DEG2RAD(float degrees);
	static float RAD2DEG(float radians);

	/// get distance of between 2 points
	static float DISTANCE(float x1, float y1, float x2, float y2);
	static float DistancePOW2(float x1, float y1, float x2, float y2);

	static void NORMALIZE(float &vx, float &vy);
	/// get angle of line (A,B)
	static float ANGLE(float xA, float yA, float xB, float yB);
	static float ANGLE(float vx, float vy);
	/// get angle of 2 vector V1(v1x, v1y) and V2(v2x, v2y)
	static float ANGLE_OF_2_VECTOR(float v1x, float v1y, float v2x, float v2y);
	static float DISTANCE(float vx, float vy);
	static void ROTATE_VECTOR(float &Vx, float &Vy, float angle);

	// collision
	static bool	LINE_ON_LINE(int xA1, int yA1, int xB1, int yB1, int xA2, int yA2, int xB2, int yB2);
	static bool RECT_ON_LINE(int box_left, int box_top, int box_right, int box_bottom, int x1, int y1, int x2, int y2);
	static bool POINT_IN_RECT(int px, int py, int box_left, int box_top, int box_right, int box_bottom);
	static int DISTANCE_LINE_POINT(int x1, int y1, int x2, int y2, int px, int py);

	//matrix4
	static PineMatrix4 Matrix4Identitfies();
	static PineMatrix4 Matrix4FromRectangle(float x, float y, float w, float h);
	static PineMatrix4 Matrix4FromRectangle(PineRectangle rect);
	static PineMatrix4 Matrix4FromParam(PineTransformParam param);
	static PineMatrix4 Matrix4Multiply(PineMatrix4 mat1, PineMatrix4 mat2);

	static PineMatrix4 Matrix4Translate2D(float x, float y);
	static PineMatrix4 Matrix4Translate(float x, float y, float z);
	static PineMatrix4 Matrix4Translate2D(PinePoint p);
	static PineMatrix4 Matrix4Translate(PinePoint3D p);

	static PineMatrix4 Matrix4RotateZ2D(float angle, float x, float y);
	static PineMatrix4 Matrix4RotateZ2D(float angle, PinePoint point);
	static PineMatrix4 Matrix4RotateZ(float angle, PinePoint3D point);

	static PineMatrix4 Matrix4Scale2D(float scalex, float scaley, float atx, float aty);
	static PineMatrix4 Matrix4Scale2D(float scalex, float scaley, PinePoint p);
	static PineMatrix4 Matrix4Scale(float scalex, float scaley, float scalez, PinePoint3D p);

	static PineMatrix4 Matrix4Transpose(PineMatrix4 mat);
	static PineRectangle BoundFromMatrix4(PineMatrix4 mat);
	static PineTransformParam Matrix4ToParam(PineMatrix4 mat);

	//[NDT] Add some funcs for simplify the calc of transformation of vertex
	static PineVector3 Vector3Transform(PineVector3 vec, PineMatrix4 mat);
	static PineVector3 Vector3Scale(PineVector3 vec, PineVector3 scaleVec, PineVector3 center);
	static PineVector3 Vector3RotateZ(PineVector3 vec, float angle, PineVector3 center);
	static PineRectangle RectFromVectors(PineVector3 x0y0, PineVector3 x0y1, PineVector3 x1y1, PineVector3 x1y0);	
};

#endif