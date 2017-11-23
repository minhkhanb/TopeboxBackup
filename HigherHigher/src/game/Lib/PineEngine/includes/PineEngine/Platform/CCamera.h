#if !defined(_PINE_ENGINE_PLATFORM_CAMERA)
#define _PINE_ENGINE_PLATFORM_CAMERA

//this class implement a default camera that support both otho and pers camera

#include "Interface/ICamera.h"
#include "Interface/INamespace.h"

namespace Pine {
	namespace Platform {
		class PE_EXPORT CCamera : public Interface::ICamera, public Interface::INamespace {
		public:
			
			//implement INamespace interface
			virtual bool isName(std::string name);
			virtual void setName(std::string name);
			virtual std::string getName();
			virtual void* findChildName(std::string name);

			//implement ICamera
			virtual const Math::CMatrix4* getViewMatrix();
		};
	}
}

#endif//_PINE_ENGINE_PLATFORM_CAMERA