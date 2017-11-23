#pragma once
#if !defined(_H_PLATFORM_ICAMERA3D)
#define _H_PLATFORM_ICAMERA3D
#include "../../Math/CMatrix4.h"
#include "ICamera.h"
namespace Pine {
	namespace Platform {
		namespace Interface {
			class PE_EXPORT ICamera3D: ICamera {
			public:
				//inheritance from ICamera
				virtual const Math::CMatrix4* getViewMatrix() = 0;
			};
		}
	}
}

#endif//_H_PLATFORM_ICAMERA3D
