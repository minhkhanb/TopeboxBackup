#include "pch.h"
#include "PMatrix.h"
#include <math.h>
#include <string.h>
PMatrix::PMatrix()
{
	Identity();
}
void PMatrix::Rotate2D(float r)
{
	
	float matrix[16];
	//r = r*3.14159/180;
	matrix[0] = cos(r); matrix[1] = sin(r); matrix[2] = 0; matrix[3] = 0;
	matrix[4] = -sin(r); matrix[5] = cos(r); matrix[6] = 0; matrix[7] = 0;
	matrix[8] = 0; matrix[9] = 0; matrix[10] = 1; matrix[11] = 0;
	matrix[12] = 0; matrix[13] = 0; matrix[14] = 0; matrix[15] = 1;
	Mul(matrix);
}
void PMatrix::Scale(float x, float y, float z)
{
	float matrix[16];
	matrix[0] = x; matrix[1] = 0; matrix[2] = 0; matrix[3] = 0;
	matrix[4] = 0; matrix[5] = y; matrix[6] = 0; matrix[7] = 0;
	matrix[8] = 0; matrix[9] = 0; matrix[10] = z; matrix[11] = 0;
	matrix[12] = 0; matrix[13] = 0; matrix[14] = 0; matrix[15] = 1;
	Mul(matrix);
}
void PMatrix::Translate(float x, float y, float z)
{
	float matrix[16];
	matrix[0] = 1; matrix[1] = 0; matrix[2] = 0; matrix[3] = 0;
	matrix[4] = 0; matrix[5] = 1; matrix[6] = 0; matrix[7] = 0;
	matrix[8] = 0; matrix[9] = 0; matrix[10] = 1; matrix[11] = 0;
	matrix[12] = x; matrix[13] = y; matrix[14] = z; matrix[15] = 1;
	Mul(matrix);

}
void PMatrix::Identity()
{
	_vertor[0] = 1; _vertor[1] = 0; _vertor[2] = 0; _vertor[3] = 0;
	_vertor[4] = 0; _vertor[5] = 1; _vertor[6] = 0; _vertor[7] = 0;
	_vertor[8] = 0; _vertor[9] = 0; _vertor[10] = 1; _vertor[11] = 0;
	_vertor[12] = 0; _vertor[13] = 0; _vertor[14] = 0; _vertor[15] = 1;
}
void PMatrix::Mul(float *matrix)
{
	float result[16];
	float *b = matrix;
	float *a = _vertor;
	result[0] = a[0]*b[0] + a[1]*b[4] + a[2]*b[8] + a[3]*b[12];
	result[1] = a[0]*b[1] + a[1]*b[5] + a[2]*b[9] + a[3]*b[13];
	result[2] = a[0]*b[2] + a[1]*b[6] + a[2]*b[10] + a[3]*b[14];
	result[3] = a[0]*b[3] + a[1]*b[7] + a[2]*b[11] + a[3]*b[15];

	result[4] = a[4]*b[0] + a[5]*b[4] + a[6]*b[8] + a[7]*b[12];
	result[5] = a[4]*b[1] + a[5]*b[5] + a[6]*b[9] + a[7]*b[13];
	result[6] = a[4]*b[2] + a[5]*b[6] + a[6]*b[10] + a[7]*b[14];
	result[7] = a[4]*b[3] + a[5]*b[7] + a[6]*b[11] + a[7]*b[15];

	result[8] = a[8]*b[0] + a[9]*b[4] + a[10]*b[8] + a[11]*b[12];
	result[9] = a[8]*b[1] + a[9]*b[5] + a[10]*b[9] + a[11]*b[13];
	result[10] = a[8]*b[2] + a[9]*b[6] + a[10]*b[10] + a[11]*b[14];
	result[11] = a[8]*b[3] + a[9]*b[7] + a[10]*b[11] + a[11]*b[15];

	result[12] = a[12]*b[0] + a[13]*b[4] + a[14]*b[8] + a[15]*b[12];
	result[13] = a[12]*b[1] + a[13]*b[5] + a[14]*b[9] + a[15]*b[13];
	result[14] = a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14];
	result[15] = a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15];


	memcpy(_vertor,result,16*sizeof(float));
}
void PMatrix::Mul(PMatrix *matrix)
{
	
	//double result[16];
	Mul(matrix->_vertor);
}
void PMatrix::SetOrtho(float left, float right, float bottom, float top, float near, float far)
{
    const float tx = - (right + left)/(right - left);
	const float ty = - (top + bottom)/(top - bottom);
	const float tz = - (far + near)/(far - near);
    float m[16];
	m[0] = 2.0f/(right-left);
	m[1] = 0;
	m[2] = 0;
	m[3] = tx;
	
	m[4] = 0;
	m[5] = 2.0f/(top-bottom);
	m[6] = 0;
	m[7] = ty;
	
	m[8] = 0;
	m[9] = 0;
	m[10] = -2.0/(far-near);
	m[11] = tz;
	
	m[12] = 0;
    m[13] = 0;
	m[14] = 0;
	m[15] = 1;
    Mul(m);
}