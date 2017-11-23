#if !defined(_PINE_ENGINE_PLATFORM_CAPPLICATION)
#define _PINE_ENGINE_PLATFORM_CAPPLICATION

#include <memory>
#include "Interface/IApplication.h"

namespace Pine
{
	namespace Os
	{
		class PE_EXPORT OS;
		class PE_EXPORT OsInside;
	}
	namespace Platform
	{
		class PE_EXPORT CApplicationInside;
		class PE_EXPORT CApplication : public std::enable_shared_from_this<CApplication>
		{
		protected:
			CApplicationInside* _inside;
		public:
			CApplication();
			~CApplication();

			friend class Os::OS;
			friend class Os::OsInside;
			friend class CDeviceContext;
			friend class CDeviceContextInside;
		};
	}
}

#endif //_PINE_ENGINE_PLATFORM_CAPPLICATION


