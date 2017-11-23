#pragma once
#if !defined(_H_PINE_ENGINE_CMATH)
#define _H_PINE_ENGINE_CMATH
#include "../common.h"
namespace Pine 
{
	namespace Math 
	{
		class PE_EXPORT CMath {
		public:
			//matrix 4
			static void addMatrix4(const float* m, float scalar, float* dst);
			static void addMatrix4(const float* m1, const float* m2, float* dst);
			static void subMatrix4(const float* m1, const float* m2, float* dst);
			static void mulMatrix4(const float* m, float scalar, float* dst);
			static void mulMatrix4(const float* m1, const float* m2, float* dst);
			static void negateMatrix4(const float* m, float* dst);
			static void transposeMatrix(const float* m, float* dst);
			static float convDegToRad(float deg);
			static const float PI;
		};
	}
}

#endif//_H_PINE_ENGINE_CMATH