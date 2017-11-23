#if !defined(_H_PINE_ENGINE_PLATFORM_EDEPTH_FUNCTION)
#define _H_PINE_ENGINE_PLATFORM_EDEPTH_FUNCTION
#include "../common.h"
namespace Pine
{
	namespace Platform
	{
		enum class PE_EXPORT EDepthFunction
		{
			NEVER,
			LESS,
			EQUAL,
			LEQUAL,
			GREATER,
			NOTEQUAL,
			GEQUAL,
			ALWAYS
		};
	}
}
#endif//_H_PINE_ENGINE_PLATFORM_EDEPTH_FUNCTION