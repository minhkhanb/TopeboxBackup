#if !defined(_PINE_ENGINE_PLATFORM_IREF)
#define _PINE_ENGINE_PLATFORM_IREF
#include "../../common.h"
namespace Pine
{
	namespace Platform
	{
		namespace Interface
		{
			class PE_EXPORT IRef {
			protected:
				unsigned int _ref_count;
			public:
				IRef();
				void retainRef();
				void releaseRef();
			};
		}
	}
}
#endif//_PINE_ENGINE_PLATFORM_IREF
