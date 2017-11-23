#pragma once
#if !defined(_H_MATH_CMATRIX4)
#define _H_MATH_CMATRIX4
#include "../common.h"
namespace Pine {
	namespace Math {
		class PE_EXPORT CMatrix4 {
		public:
			float _m[16];

			CMatrix4();
			CMatrix4(const CMatrix4& copy);

			CMatrix4& operator+=(const CMatrix4& m);
			const CMatrix4 operator+(const CMatrix4& m);
			CMatrix4& operator*=(const CMatrix4& m);
			const CMatrix4 operator*(const CMatrix4& m);

			static CMatrix4 createOrthoGraphic(float width, float height, float near, float far);
			static CMatrix4 createOrthoGraphicOffCenter(float left, float right, float bottom, float top, float near, float far);
			static CMatrix4 createPerspective(float fov, float aspect, float near, float far);
		};
	}
}
#endif//_H_MATH_CMATRIX4
