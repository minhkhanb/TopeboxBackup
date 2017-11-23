#if !defined(_PINE_ENGINE_PLATFORM_EBLEND)
#define _PINE_ENGINE_PLATFORM_EBLEND

#include "../common.h"

namespace Pine
{
	namespace Platform
	{
		enum class PE_EXPORT EBlend
		{
			ZERO,
			ONE,
			SRC_COLOR,
			ONE_MINUS_SRC_COLOR,
			DST_COLOR,
			ONE_MINUS_DST_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_ALPHA,
			ONE_MINUS_DST_ALPHA
		};
	}
}
#endif//_PINE_ENGINE_PLATFORM_EBLEND