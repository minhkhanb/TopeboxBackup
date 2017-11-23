#if !_PMATRIX_H
#define _PMATRIX_H 1
class PMatrix
{
public:
	PMatrix();
	float _vertor[4*4];

	void Mul(PMatrix *matrix);
	void Mul(float *matrix);
	void Identity();
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);
	void Rotate2D(float angle);
    void SetOrtho(float left, float right, float bottom, float top, float near, float far);
};
#endif