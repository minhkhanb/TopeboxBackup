#include "Matrix4x4.h"
#include "matrix.h"
#include <math.h>
void Matrix4x4::SetMatrix(float* t)
{
    for(int i= 0; i<16; i++)
    {
        m[i] = t[i];
    }
}

namespace Matrix4x4Utils
{

// http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
void SetOrthoLH(Matrix4x4& m,float width, float height, float near, float far)
{

    m.m[0] = 2/width;
	m.m[1] = 0;
	m.m[2] = 0;
	m.m[3] = 0;
	
	m.m[4] = 0;
	m.m[5] = 2/height;
	m.m[6] = 0;
	m.m[7] = 0;
	
	m.m[8] = 0;
	m.m[9] = 0;
	m.m[10] = 1/(far-near) ;
	m.m[11] = 0;
	
	m.m[12] = 0;
	m.m[13] = 0;
	m.m[14] = near/(near-far);
	m.m[15] = 1;
        
}
void SetOrtho(Matrix4x4& m, float left, float right, float bottom, float top, float near, float far)
{
	const float tx = - (right + left)/(right - left);
	const float ty = - (top + bottom)/(top - bottom);
	const float tz = - (far + near)/(far - near);

	m.m[0] = 2.0f/(right-left);
	m.m[1] = 0;
	m.m[2] = 0;
	m.m[3] = tx;
	
	m.m[4] = 0;
	m.m[5] = 2.0f/(top-bottom);
	m.m[6] = 0;
	m.m[7] = ty;
	
	m.m[8] = 0;
	m.m[9] = 0;
	m.m[10] = -2.0/(far-near);
	m.m[11] = tz;
	
	m.m[12] = 0;
	m.m[13] = 0;
	m.m[14] = 0;
	m.m[15] = 1;
}


void SetRotY(Matrix4x4& m, float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	m.m[0] = c;
	m.m[1] = 0;
	m.m[2] = -s;
	m.m[3] = 0;

	m.m[4] = 0;
	m.m[5] = 1;
	m.m[6] = 0;
	m.m[7] = 0;

	m.m[8] = s;
	m.m[9] = 0;
	m.m[10] = c;
	m.m[11] = 0;
	
	m.m[12] = 0;
	m.m[13] = 0;
	m.m[14] = 0;
	m.m[15] = 1;	
}


void LoadRotateZ(Matrix4x4& m, float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	m.m[0] = c;
	m.m[1] = -s;
	m.m[2] = 0;
	m.m[3] = 0;

	m.m[4] = s;
	m.m[5] = c;
	m.m[6] = 0;
	m.m[7] = 0;

	m.m[8] = 0;
	m.m[9] = 0;
	m.m[10] = 1;
	m.m[11] = 0;
	
	m.m[12] = 0;
	m.m[13] = 0;
	m.m[14] = 0;
	m.m[15] = 1;	
}
    
void SetRotateZ(Matrix4x4& m, float angle)
{
    Matrix4x4 trans;
    Matrix4x4 ret;
    LoadRotateZ(trans,angle);
    Mul(ret,trans,m);
    m = ret;
}


void Mul(Matrix4x4& m, const Matrix4x4& a, const Matrix4x4& b)
{
	m.m[0] = a.m[0]*b.m[0] + a.m[1]*b.m[4] + a.m[2]*b.m[8] + a.m[3]*b.m[12];
	m.m[1] = a.m[0]*b.m[1] + a.m[1]*b.m[5] + a.m[2]*b.m[9] + a.m[3]*b.m[13];
	m.m[2] = a.m[0]*b.m[2] + a.m[1]*b.m[6] + a.m[2]*b.m[10] + a.m[3]*b.m[14];
	m.m[3] = a.m[0]*b.m[3] + a.m[1]*b.m[7] + a.m[2]*b.m[11] + a.m[3]*b.m[15];

	m.m[4] = a.m[4]*b.m[0] + a.m[5]*b.m[4] + a.m[6]*b.m[8] + a.m[7]*b.m[12];
	m.m[5] = a.m[4]*b.m[1] + a.m[5]*b.m[5] + a.m[6]*b.m[9] + a.m[7]*b.m[13];
	m.m[6] = a.m[4]*b.m[2] + a.m[5]*b.m[6] + a.m[6]*b.m[10] + a.m[7]*b.m[14];
	m.m[7] = a.m[4]*b.m[3] + a.m[5]*b.m[7] + a.m[6]*b.m[11] + a.m[7]*b.m[15];

	m.m[8] = a.m[8]*b.m[0] + a.m[9]*b.m[4] + a.m[10]*b.m[8] + a.m[11]*b.m[12];
	m.m[9] = a.m[8]*b.m[1] + a.m[9]*b.m[5] + a.m[10]*b.m[9] + a.m[11]*b.m[13];
	m.m[10] = a.m[8]*b.m[2] + a.m[9]*b.m[6] + a.m[10]*b.m[10] + a.m[11]*b.m[14];
	m.m[11] = a.m[8]*b.m[3] + a.m[9]*b.m[7] + a.m[10]*b.m[11] + a.m[11]*b.m[15];

	m.m[12] = a.m[12]*b.m[0] + a.m[13]*b.m[4] + a.m[14]*b.m[8] + a.m[15]*b.m[12];
	m.m[13] = a.m[12]*b.m[1] + a.m[13]*b.m[5] + a.m[14]*b.m[9] + a.m[15]*b.m[13];
	m.m[14] = a.m[12]*b.m[2] + a.m[13]*b.m[6] + a.m[14]*b.m[10] + a.m[15]*b.m[14];
	m.m[15] = a.m[12]*b.m[3] + a.m[13]*b.m[7] + a.m[14]*b.m[11] + a.m[15]*b.m[15];
}

void LoadIdentity(Matrix4x4&m)
{
    m.m[0] = 1.0f;
	m.m[1] = 0.0f;
	m.m[2] = 0.0f;
	m.m[3] = 0.0f;
	
	m.m[4] = 0.0f;
	m.m[5] = 1.0f;
	m.m[6] = 0.0f;
	m.m[7] = 0.0f;
	
	m.m[8] = 0.0f;
	m.m[9] = 0.0f;
	m.m[10] = 1.0f;
	m.m[11] = 0.0f;
    
	m.m[12] = 0.0f;
	m.m[13] = 0.0f;
	m.m[14] = 0.0f;
	m.m[15] = 1.0f;

    //mat4f_LoadIdentity(m.m);
}
void LoadTranslate(Matrix4x4& m, float x, float y, float z)
{
    m.m[0] = 1.0f;
	m.m[1] = 0.0f;
	m.m[2] = 0.0f;
	m.m[3] = 0.0f;
	
	m.m[4] = 0.0f;
	m.m[5] = 1.0f;
	m.m[6] = 0.0f;
	m.m[7] = 0.0f;
	
	m.m[8] = 0.0f;
	m.m[9] = 0.0f;
	m.m[10] = 1.0f;
	m.m[11] = 0.0f;
	
	m.m[12] = x;
	m.m[13] = y;
	m.m[14] = z;
	m.m[15] = 1.0f;
}
void Translate(Matrix4x4& m, float x, float y, float z)
{
    Matrix4x4 trans;
    Matrix4x4 ret;
    LoadTranslate(trans,x,y,z);
    Mul(ret,trans,m);
    m = ret;
   
}

    void Scale(Matrix4x4& m,float x, float y, float z)
    {
        float matrix[16];
        Matrix4x4 ret;
        matrix[0] = x; matrix[1] = 0; matrix[2] = 0; matrix[3] = 0;
        matrix[4] = 0; matrix[5] = y; matrix[6] = 0; matrix[7] = 0;
        matrix[8] = 0; matrix[9] = 0; matrix[10] = z; matrix[11] = 0;
        matrix[12] = 0; matrix[13] = 0; matrix[14] = 0; matrix[15] = 1;
        Matrix4x4 trans;
        trans.SetMatrix(matrix);
        Mul(ret,trans,m);
        m = ret;
    }
    void DisableZ(Matrix4x4&m)
    {
        m.m[2] = 0.0f;
        m.m[6] = 0.0f;
        m.m[10] = 1.0f;
        m.m[14] = 0.0f;
    }
}
