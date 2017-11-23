#if !defined(_H_PLATFORM_INTERFACE_CAMERA)
#define _H_PLATFORM_INTERFACE_CAMERA
#include "../../Math/CMatrix4.h"
namespace Pine {
	namespace Platform {
		namespace Interface {
			class PE_EXPORT ICamera {
			public:
				enum class Type
				{
					PERSPECTIVE = 1,
					ORTHOGRAPHIC = 2
				};
				virtual const char* getId() = 0;
				virtual const Math::CMatrix4* getViewMatrix() = 0;
				virtual Type getType() = 0;
			};
		}
	}
}

#endif