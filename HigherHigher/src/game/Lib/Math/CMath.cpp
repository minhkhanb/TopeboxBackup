#include "pch.h"
#include "CMath.h"
#include "../../GameCore/CGame.h"

unsigned int CMath::srand_number = 0xffffffff;
unsigned int _rand_count = 0;

int CMath::RANDOM(int min, int max)
{
	int random_range = max - min + 1;
#ifdef USE_CPP_RANDOM_UNDER_1000
    if(random_range <= 1000)
    {
        #ifdef MAC_OS
        return (int)((min + (arc4random()%random_range)));
        #else
        return (int)((min + (USE_CPP_FUN_RANDOM()%random_range)));
        #endif
        //return (int)((min + (arc4random()%random_range)));
    }
#endif
#ifdef MAC_OS
    /*
    for(int i= 0; i < 10; i++)
    {
        arc4random_uniform(random_range);
    }
     */
    return (int)((min + (arc4random_uniform(random_range))));
#else
	return (int)((min + (rand()%random_range)));
#endif
}

float CMath::RANDOM_PERCENT(int _countDecimal)
{	
	_countDecimal += 2;
	double pow = CMath::POW(10, _countDecimal);	
	float ret = 0.0f;
	ret = CMath::RANDOM(1, pow) / (pow  *  0.01);
	return ret;	
}

//#ifndef MAC_OS
int CMath::MAXIMUM(int a, int b)
{
	return (a>b ? a : b);
}

int CMath::MINIMUM(int a, int b)
{
	return (a>b ? b : a);
}

float CMath::MinFloat(float* arr, int len)
{
	float min = arr[0];
	for (int i = 0; i < len; i++)
	{
		min = min < arr[i] ? min : arr[i];
	}
	return min;
}
int CMath::MinInt(int * arr, int len)
{
	int min = arr[0];
	for (int i = 0; i < len; i++)
	{
		min = min < arr[i] ? min : arr[i];
	}
	return min;
}

float CMath::MaxFloat(float* arr, int len)
{
	float max = arr[0];
	for (int i = 0; i < len; i++)
	{
		max = max > arr[i] ? max : arr[i];
	}
	return max;
}

int CMath::MaxInt(int* arr, int len)
{
	int max = arr[0];
	for (int i = 0; i < len; i++)
	{
		max = max > arr[i] ? max : arr[i];
	}
	return max;
}
//#endif

void CMath::PRE_RANDOM()
{
#ifdef USE_CPP_RANDOM_UNDER_1000
    //time_t t = USE_CPP_RANDOM_UNDER_1000;
    //printf("\nUSE_CPP_RANDOM_UNDER_1000 = %ld",t);
    //1452748550
    srand(USE_CPP_RANDOM_UNDER_1000);
#else
	srand(time(0) + srand_number);
#endif
}

int CMath::ABSOLUTE_VALUE(int x)
{
	return abs(x);
}

float CMath::ABSOLUTE_VALUE(float x)
{
    return x < 0?-x:x;
}

float CMath::SQRT(float x)
{
	return sqrt(x);
}

float CMath::POW(float x, int y)
{
	return pow(x, y);
}

float CMath::SIN(float angle)
{
#if USE_TABLE_MATH
    return CGame::Math_Sin(angle);
#else
	return sin((float)(angle * PI/180));
#endif
}

float CMath::COS(float angle)
{
#if USE_TABLE_MATH
    return CGame::Math_Cos(angle);
#else
	return cos((float)(angle * PI/180));
#endif
}

float CMath::TAN(float angle)
{
	return tan((float)(angle * PI/180));
}

float CMath::ARCSIN(float sin)
{
	return (float)(asin(sin) * 180.0/PI);
}

float CMath::ARCCOS(float cos)
{
	return (float)(acos(cos) * 180.0/PI);
}

float CMath::ARCTAN(float tan)
{
	return (float)(atan(tan) * 180.0/PI);
}

float CMath::DEG2RAD(float degrees)
{
	return (degrees * PI/180);
}

float CMath::RAD2DEG(float radians)
{
	return (radians * 180/PI);
}

float CMath::DISTANCE(float x1, float y1, float x2, float y2)
{
	float dx = fabs(x1 - x2);
	float dy = fabs(y1 - y2);

	return SQRT(dx * dx + dy * dy);
}

void CMath::NORMALIZE(float &vx, float &vy)
{
	if (vx == 0 && vy ==0)
		return;
		
	float distance = SQRT(vx * vx + vy * vy);
	vx /= distance;
	vy /= distance;
}

float CMath::ANGLE(float xA, float yA, float xB, float yB)
{
	if (xB == xA)
	{
		if ((yB - yA) < 0)
			return  -90;
		else
			return 90;
			
	}
	
	double angle = ARCTAN( (yB-yA)/(xB-xA) );
	
	if ((xB - xA) < 0)
	{
		angle += 180;
	}
	
	if (angle > 180)
	{
		angle -= 360;
	}
	
	return angle;
}

float CMath::ANGLE(float vx, float vy)
{
	if (vx == 0)
	{
		if ((vy) < 0)
			return  -90;
		else
			return 90;
		
	}
	
	double angle = ARCTAN( vy/vx );
	
	if (vx < 0)
	{
		angle += 180;
	}
	
	if (angle > 180)
	{
		angle -= 360;
	}
	
	return angle;
}

float CMath::ANGLE_OF_2_VECTOR(float v1x, float v1y, float v2x, float v2y)
{
	float a1 = 0.0;
	
	if (v1x == 0)
	{
		a1 = 90.0;
	}
	else
	{
		a1 = ARCTAN(v1y/v1x);
	}

	if (a1 > 0)
	{
		if (v1y < 0)
		{
			a1 += 180;
		}
	}
	else
	{
		if (v1y > 0)
		{
			a1 += 180;
		}
		else
		{
			a1 += 360;
		}
	}
	
	float a2 = 0.0;
	
	if (v2x == 0)
	{
		a2 = 90.0;
	}
	else
	{
		a2 = ARCTAN(v2y/v2x);
	}
	
	if (a2 > 0)
	{
		if (v2y < 0)
		{
			a2 += 180;
		}
	}
	else
	{
		if (v2y > 0)
		{
			a2 += 180;
		}
		else
		{
			a2 += 360;
		}
	}
	
	return (a2-a1);
}

float CMath::DISTANCE(float vx, float vy)
{
	return SQRT(vx * vx + vy * vy);
}

float CMath::DistancePOW2(float x1, float y1, float x2, float y2)
{
	float dx = CMath::ABSOLUTE_VALUE(x1 - x2);
	float dy = CMath::ABSOLUTE_VALUE(y1 - y2);
	return (dx * dx + dy * dy);
}

void CMath::ROTATE_VECTOR(float &vx, float &vy, float angle)
{
	float dd = sqrt(vx * vx + vy * vy);
	
	float a = 0.0;
	
	if (vx == 0)
	{
		a = 90.0;
	}
	else
	{
		a = ARCTAN(vy/vx);
	}
	
	if (a > 0)
	{
		if (vy < 0)
		{
			a += 180;
		}
	}
	else
	{
		if (vy > 0)
		{
			a += 180;
		}
	}
	
	a += angle;
	
	vx = dd * COS(a);
	vy = dd * SIN(a);
}

// collision checking

bool CMath::LINE_ON_LINE(int xA1, int yA1, int xB1, int yB1, int xA2, int yA2, int xB2, int yB2)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = xB1 - xA1;     s1_y = yB1 - yA1;
    s2_x = xB2 - xA2;     s2_y = yB2 - yA2;

    float s, t;
    s = (-s1_y  *  (xA1 - xA2) + s1_x  *  (yA1 - yA2)) / (-s2_x  *  s1_y + s1_x  *  s2_y);
    t = ( s2_x  *  (yA1 - yA2) - s2_y  *  (xA1 - xA2)) / (-s2_x  *  s1_y + s1_x  *  s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        //if (i_x != NULL)
        //     * i_x = xA1 + (t  *  s1_x);
        //if (i_y != NULL)
        //     * i_y = yA1 + (t  *  s1_y);
        return true;
    }

    return false; // No collision
}

bool CMath::RECT_ON_LINE(int box_left, int box_top, int box_right, int box_bottom, int x1, int y1, int x2, int y2) 
{
//this used to be global, move it out if we use it in more places but here.
	int tmpBox[4];
	tmpBox[0] = box_left;
	tmpBox[1] = box_top;
	tmpBox[2] = box_right;
	tmpBox[3] = box_bottom;
	// check if the line is totally outside the box
	if ((x1 < tmpBox[0] && x2 < tmpBox[0]) ||
		(x1 > tmpBox[2] && x2 > tmpBox[2]) ||
		(y1 < tmpBox[1] && y2 < tmpBox[1]) ||
		(y1 > tmpBox[3] && y2 > tmpBox[3])) {
		return false;
	}
	tmpBox[0] -= x1;
	tmpBox[1] -= y1;
	tmpBox[2] -= x1;
	tmpBox[3] -= y1;
	x2 -= x1;
	y2 -= y1;
	int res1 = ((x2  *  -tmpBox[1]) + (y2  *  tmpBox[0]));
	int res2 = ((x2  *  -tmpBox[3]) + (y2  *  tmpBox[0]));
	int res3 = ((x2  *  -tmpBox[1]) + (y2  *  tmpBox[2]));
	int res4 = ((x2  *  -tmpBox[3]) + (y2  *  tmpBox[2]));
	if ((res1 < 0 && res2 < 0 && res3 < 0 && res4 < 0) ||
		(res1 > 0 && res2 > 0 && res3 > 0 && res4 > 0)) {
		return false;
	}

	return true;
}

bool CMath::POINT_IN_RECT(int px, int py, int box_left, int box_top, int box_right, int box_bottom)
{
	if (px < box_left)
		return false;
	if (px > box_right)
		return false;
	if (py < box_top)
		return false;
	if (py > box_bottom)
		return false;
	
	return true;
}

int CMath::DISTANCE_LINE_POINT(int x1, int y1, int x2, int y2, int px, int py)
{
	//int v1 = CMath::ANGLE(x3, y3, x1, y1);
	//int v2 = CMath::ANGLE(x3, y3, x2, y2);
	//int angle = CMath::ABSOLUTE_VALUE(v2 - v1);
	double a = CMath::DISTANCE(px, py, x2, y2);
	double b = CMath::DISTANCE(x1, y1, px, py);
	double c = CMath::DISTANCE(x1, y1, x2, y2);
	double p = (a + b + c)/2;

	double area = CMath::SQRT( p * (p-a) * (p-b) * (p-c));

	
	//int area = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);

	//int len_of_line = CMath::DISTANCE(x1, y1, x2, y2);
	//area = CMath::ABSOLUTE_VALUE(area);
	//printf("Area %d    l: %d     res: %d\n", area, c, area * 2/c);
	return area * 2/c;
}





PineMatrix4 CMath::Matrix4Identitfies()
{
	return PineMatrix4();
}

PineMatrix4 CMath::Matrix4FromRectangle(float x, float y, float w, float h)
{
	PineMatrix4 mat;
	mat.m0 = x;
	mat.m1 = y;
	mat.m2 = 0;
	mat.m3 = 1;

	mat.m4 = x + w;
	mat.m5 = y;
	mat.m6 = 0;
	mat.m7 = 1;

	mat.m8 = x + w;
	mat.m9 = y + h;
	mat.m10 = 0;
	mat.m11 = 1;

	mat.m12 = x;
	mat.m13 = y + h;
	mat.m14 = 0;
	mat.m15 = 1;
	return mat;
}
PineMatrix4 CMath::Matrix4FromRectangle(PineRectangle rect)
{
	PineMatrix4 mat;
	mat.m0 = rect.X;
	mat.m1 = rect.Y;
	mat.m2 = 0;
	mat.m3 = 1;

	mat.m4 = rect.X + rect.W;
	mat.m5 = rect.Y;
	mat.m6 = 0;
	mat.m7 = 1;

	mat.m8 = rect.X + rect.W;
	mat.m9 = rect.Y + rect.H;
	mat.m10 = 0;
	mat.m11 = 1;

	mat.m12 = rect.X;
	mat.m13 = rect.Y + rect.H;
	mat.m14 = 0;
	mat.m15 = 1;
	return mat;
}

PineMatrix4 CMath::Matrix4FromParam(PineTransformParam param)
{
	PineMatrix4 mat;
	mat.m0 = param.m11;
	mat.m1 = param.m12;
	mat.m4 = param.m21;
	mat.m5 = param.m22;
	mat.m12 = param.TranslateX;
	mat.m13 = param.TranslateY;
	return mat;
}

PineMatrix4 CMath::Matrix4Multiply(PineMatrix4 left, PineMatrix4 right)
{
	PineMatrix4 mat;

	mat.m0 = left.m0 * right.m0 + left.m1 * right.m4 + left.m2 * right.m8 + left.m3 * right.m12;
	mat.m1 = left.m0 * right.m1 + left.m1 * right.m5 + left.m2 * right.m9 + left.m3 * right.m13;
	mat.m2 = left.m0 * right.m2 + left.m1 * right.m6 + left.m2 * right.m10 + left.m3 * right.m14;
	mat.m3 = left.m0 * right.m3 + left.m1 * right.m7 + left.m2 * right.m11 + left.m3 * right.m15;

	mat.m4 = left.m4 * right.m0 + left.m5 * right.m4 + left.m6 * right.m8 + left.m7 * right.m12;
	mat.m5 = left.m4 * right.m1 + left.m5 * right.m5 + left.m6 * right.m9 + left.m7 * right.m13;
	mat.m6 = left.m4 * right.m2 + left.m5 * right.m6 + left.m6 * right.m10 + left.m7 * right.m14;
	mat.m7 = left.m4 * right.m3 + left.m5 * right.m7 + left.m6 * right.m11 + left.m7 * right.m15;

	mat.m8 = left.m8 * right.m0 + left.m9 * right.m4 + left.m10 * right.m8 + left.m11 * right.m12;
	mat.m9 = left.m8 * right.m1 + left.m9 * right.m5 + left.m10 * right.m9 + left.m11 * right.m13;
	mat.m10 = left.m8 * right.m2 + left.m9 * right.m6 + left.m10 * right.m10 + left.m11 * right.m14;
	mat.m11 = left.m8 * right.m3 + left.m9 * right.m7 + left.m10 * right.m11 + left.m11 * right.m15;

	mat.m12 = left.m12 * right.m0 + left.m13 * right.m4 + left.m14 * right.m8 + left.m15 * right.m12;
	mat.m13 = left.m12 * right.m1 + left.m13 * right.m5 + left.m14 * right.m9 + left.m15 * right.m13;
	mat.m14 = left.m12 * right.m2 + left.m13 * right.m6 + left.m14 * right.m10 + left.m15 * right.m14;
	mat.m15 = left.m12 * right.m3 + left.m13 * right.m7 + left.m14 * right.m11 + left.m15 * right.m15;

	return mat;
}
PineMatrix4 CMath::Matrix4Translate2D(float x, float y)
{
	PineMatrix4 mat;
	mat.m12 = x;
	mat.m13 = y;
	mat.m14 = 0;
	return mat;
}

PineMatrix4 CMath::Matrix4Translate(float x, float y, float z)
{
	PineMatrix4 mat;
	mat.m12 = x;
	mat.m13 = y;
	mat.m14 = x;
	return mat;
}


PineMatrix4 CMath::Matrix4Translate2D(PinePoint p)
{
	PineMatrix4 mat;
	mat.m12 = p.X;
	mat.m13 = p.Y;
	mat.m14 = 0;
	return mat;
}

PineMatrix4 CMath::Matrix4Translate(PinePoint3D p)
{
	PineMatrix4 mat;
	mat.m12 = p.X;
	mat.m13 = p.Y;
	mat.m14 = p.Z;
	return mat;
}

PineMatrix4 CMath::Matrix4RotateZ2D(float angle, float x, float y)
{
	PineMatrix4  rotate, mat;
	rotate.m0 = CMath::COS(angle);
	rotate.m1 = CMath::SIN(angle);
	rotate.m4 = -CMath::SIN(angle);
	rotate.m5 = CMath::COS(angle);

	mat = Matrix4Multiply(mat, Matrix4Translate2D(-x, -y));
	mat = Matrix4Multiply(mat, rotate);
	mat = Matrix4Multiply(mat, Matrix4Translate2D(x, y));

	return mat;
}

PineMatrix4 CMath::Matrix4RotateZ2D(float angle, PinePoint point)
{
	PineMatrix4  rotate, mat;
	rotate.m0 = CMath::COS(angle);
	rotate.m1 = CMath::SIN(angle);
	rotate.m4 = -CMath::SIN(angle);
	rotate.m5 = CMath::COS(angle);

	mat = Matrix4Multiply(mat, Matrix4Translate2D(-point.X, -point.Y));
	mat = Matrix4Multiply(mat, rotate);
	mat = Matrix4Multiply(mat, Matrix4Translate2D(point.X, point.Y));

	return mat;
}

PineMatrix4 CMath::Matrix4RotateZ(float angle, PinePoint3D point)
{
	PineMatrix4  rotate, mat;
	rotate.m0 = CMath::COS(angle);
	rotate.m1 = CMath::SIN(angle);
	rotate.m4 = -CMath::SIN(angle);
	rotate.m5 = CMath::COS(angle);
	
	mat = Matrix4Multiply(mat, Matrix4Translate(-point.X, -point.Y, -point.Z));
	mat = Matrix4Multiply(mat, rotate);
	mat = Matrix4Multiply(mat, Matrix4Translate(point.X, point.Y, point.Z));

	return mat;
}


PineMatrix4 CMath::Matrix4Scale2D(float scalex, float scaley, float atx, float aty)
{
	PineMatrix4  scale, mat;

	scale.m0 = scalex;
	scale.m5 = scaley;
	scale.m10 = 1.0f;

	mat = CMath::Matrix4Multiply(mat, Matrix4Translate2D(-atx, -aty));
	mat = CMath::Matrix4Multiply(mat, scale);
	mat = CMath::Matrix4Multiply(mat, Matrix4Translate2D(atx, aty));

	return mat;
}

PineMatrix4 CMath::Matrix4Scale2D(float scalex, float scaley, PinePoint p)
{
	PineMatrix4  scale, mat;

	scale.m0 = scalex;
	scale.m5 = scaley;
	scale.m10 = 1.0f;

	mat = CMath::Matrix4Multiply(mat, Matrix4Translate2D(-p.X, -p.Y));
	mat = CMath::Matrix4Multiply(mat, scale);
	mat = CMath::Matrix4Multiply(mat, Matrix4Translate2D(p.X, p.Y));

	return mat;
}

PineMatrix4 CMath::Matrix4Scale(float scalex, float scaley,float scalez, PinePoint3D p)
{
	PineMatrix4  scale, mat;

	scale.m0 = scalex;
	scale.m5 = scaley;
	scale.m10 = scalez;

	mat = CMath::Matrix4Multiply(mat, Matrix4Translate(-p.X, -p.Y, -p.Z));
	mat = CMath::Matrix4Multiply(mat, scale);
	mat = CMath::Matrix4Multiply(mat, Matrix4Translate(p.X, p.Y, p.Z));

	return mat;
}

PineMatrix4 CMath::Matrix4Transpose(PineMatrix4 mat)
{
	PineMatrix4 mattr;
	mattr.m0 = mat.m0;
	mattr.m1 = mat.m4;
	mattr.m2 = mat.m8;
	mattr.m3 = mat.m12;

	mattr.m4 = mat.m1;
	mattr.m5 = mat.m5;
	mattr.m6 = mat.m9;
	mattr.m7 = mat.m13;

	mattr.m8 = mat.m2;
	mattr.m9 = mat.m6;
	mattr.m10 = mat.m10;
	mattr.m11 = mat.m14;

	mattr.m12 = mat.m3;
	mattr.m13 = mat.m7;
	mattr.m14 = mat.m11;
	mattr.m15 = mat.m15;

	return mattr;
}

PineRectangle CMath::BoundFromMatrix4(PineMatrix4 mat)
{
	/*printf("ori\n");
	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\n", mat.m0, mat.m1, mat.m2, mat.m3);
	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\n", mat.m4, mat.m5, mat.m6, mat.m7);
	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\n", mat.m8, mat.m9, mat.m10, mat.m11);
	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\n",mat.m12, mat.m13, mat.m14, mat.m15);
	*/
	
	PineMatrix4 mattr = CMath::Matrix4Transpose(mat);

	/*printf("transpose\n");
	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\n", mattr.m0, mattr.m1, mattr.m2, mattr.m3);
	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\n", mattr.m4, mattr.m5, mattr.m6, mattr.m7);
	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\n", mattr.m8, mattr.m9, mattr.m10, mattr.m11);
	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\n", mattr.m12, mattr.m13, mattr.m14, mattr.m15);

	printf("end\n");*/


	float minx = CMath::MinFloat((float*)&mattr, 4);
	float maxx = CMath::MaxFloat((float*)&mattr, 4);
	float miny = CMath::MinFloat(((float*)&mattr) + 4, 4);
	float maxy = CMath::MaxFloat(((float*)&mattr) + 4, 4);

	PineRectangle rect;
	rect.X = minx;
	rect.Y = miny;
	rect.W = maxx - minx;
	rect.H = maxy - miny;

	return rect;
}

PineTransformParam CMath::Matrix4ToParam(PineMatrix4 mat)
{
	PineTransformParam param;

	param.m11 = mat.m0;
	param.m12 = mat.m1;
	param.m21 = mat.m4;
	param.m22 = mat.m5;
	param.TranslateX = mat.m12;
	param.TranslateY = mat.m13;

	return param;
}

PineVector3 CMath::Vector3Transform(PineVector3 vec3, PineMatrix4 mat)
{
	PineVector3 result;
	result.x = mat.m0 * vec3.x + mat.m4 * vec3.y + mat.m8 * vec3.z + mat.m12 * 1.0f;
	result.y = mat.m1 * vec3.x + mat.m5 * vec3.y + mat.m9 * vec3.z + mat.m13 * 1.0f;
	result.z = mat.m2 * vec3.x + mat.m6 * vec3.y + mat.m10 * vec3.z + mat.m14 * 1.0f;
	return result;
}

PineVector3 CMath::Vector3Scale(PineVector3 vec3, PineVector3 scaleVec, PineVector3 center)
{
	PineVector3 result;
	result.x = vec3.x - center.x;
	result.y = vec3.y - center.y;
	result.z = vec3.z - center.z;

	result.x = result.x*(scaleVec.x);
	result.y = result.y*(scaleVec.y);
	result.z = result.z*(scaleVec.z);

	result.x = result.x + center.x;
	result.y = result.y + center.y;
	result.z = result.z + center.z;

	return result;
}

PineVector3 CMath::Vector3RotateZ(PineVector3 vec3, float angle, PineVector3 center)
{
	float cosA = CMath::COS(angle);
	float sinA = CMath::SIN(angle);

	PineVector3 result;
	result.x = vec3.x - center.x;
	result.y = vec3.y - center.y;

	float xx = result.x*cosA - result.y*sinA;
	result.y = result.x*sinA + result.y*cosA;
	result.x = xx;

	result.x = result.x + center.x;
	result.y = result.y + center.y;

	return result;
}

PineRectangle CMath::RectFromVectors(PineVector3 x0y0, PineVector3 x0y1, PineVector3 x1y1, PineVector3 x1y0)
{
	float xArray[4];
	xArray[0] = x0y0.x;
	xArray[1] = x1y0.x;
	xArray[2] = x1y1.x;
	xArray[3] = x1y0.x;

	float yArray[4];
	yArray[0] = x0y0.y;
	yArray[1] = x1y0.y;
	yArray[2] = x1y1.y;
	yArray[3] = x1y0.y;

	float minx = CMath::MinFloat(&xArray[0], 4);
	float maxx = CMath::MaxFloat(&xArray[0], 4);
	float miny = CMath::MinFloat(&yArray[0], 4);
	float maxy = CMath::MaxFloat(&yArray[0], 4);

	PineRectangle rect;
	rect.X = minx;
	rect.Y = miny;
	rect.W = maxx - minx;
	rect.H = maxy - miny;

	return rect;
}
