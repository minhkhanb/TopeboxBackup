#ifndef st_Matrix4x4_h_
#define st_Matrix4x4_h_

struct Matrix4x4
{
	float m[16];
public:
    void SetMatrix(float* t);
};


namespace Matrix4x4Utils
{
    void SetOrthoLH(Matrix4x4& m,float width, float height, float near, float far);
	void SetOrtho(Matrix4x4& m, float left, float right, float bottom, float top, float near, float far);
	void SetRotY(Matrix4x4& m, float angle);
    void LoadRotateZ(Matrix4x4& m, float angle);
	void SetRotateZ(Matrix4x4& m, float angle);
	void Mul(Matrix4x4& m, const Matrix4x4& a, const Matrix4x4& b);
    void LoadIdentity(Matrix4x4&m);
    void DisableZ(Matrix4x4&m);
    void Translate(Matrix4x4& m, float x, float y, float z);
    void Scale(Matrix4x4& m,float x, float y, float z);
}


#endif
