#pragma once
#if !defined(_ENGINE_LIB3D_CCAMERA_PERSPECTIVE)
#define _ENGINE_LIB3D_CCAMERA_PERSPECTIVE
#include "../common.h"
#include "Interface/ICameraPerspective.h"
#include "../Math/CMatrix4.h"

namespace Pine 
{
	namespace Lib3D
	{

		class PE_EXPORT CCameraPerspective : public Interface::ICameraPerspective
		{
		protected:
			Math::CMatrix4 _perspective_matrix;
		public:
			//implement ICameraPerspective
			virtual const Math::CMatrix4* getViewMatrix();

			void set(float fov, float aspect, float near, float far);
		};

	}
}

#endif//_ENGINE_LIB3D_CCAMERA_PERSPECTIVE
